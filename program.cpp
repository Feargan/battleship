#include "program.h"

#include <iostream>
#include <ctime>

#include "startgameui.h"
#include "designerui.h"


bool CProgram::init()
{
	std::ios_base::sync_with_stdio(false);
	srand((unsigned int)time(NULL));
    bool Error = false;


	m_GamePreset.load("preset.bsp");
	m_GamePreset.setMaxShipSize(5);
	m_GamePreset.setShipAmount(1, 2);
	m_GamePreset.setShipAmount(2, 2);
	m_GamePreset.setShipAmount(3, 2);
	m_GamePreset.setShipAmount(4, 2);
	m_GamePreset.setShipAmount(5, 1);
	m_GamePreset.setShipAmount(5, 0);
	
	// global config
	m_WindowWidth = 800;
	m_WindowHeight = 600;

	
	//m_GameController = IGameController(&m_GamePreset);
	//m_GameUi = CGameUi(&m_GameController, &m_GamePreset);
    return !Error;
}

void CProgram::run()
{
	/*m_GamePreset.setMaxShipSize(6);
	m_GamePreset.setShipAmount(1, 1);
	m_GamePreset.setShipAmount(2, 0);
	m_GamePreset.setShipAmount(3, 0);
	m_GamePreset.setShipAmount(4, 0);
	m_GamePreset.setShipAmount(5, 0);
	m_GamePreset.setShipAmount(6, 0);
	CGameBoard Board(&m_GamePreset);
	struct CValues
	{
		float Min = 1.1f;
		float Max = -0.1f;
		float Avg = 0.f;
		int Count = 0;
		CGamePreset MinPreset;
		CGamePreset MaxPreset;
	};
	std::map<int, CValues> m_Computed;
	while (m_GamePreset.getShipAmount(6) != 1)
	{
		int s = 0, f = 0;
		if (m_GamePreset.indicator() != std::numeric_limits<float>::infinity())// && m_GamePreset.indicator() < 0.42f)
		{
			std::cout << "i=" << m_GamePreset.indicator() << " ";
			while (s + f < 10000)
			{
				Board.clear();
				Board.autoFill();
				Board.isReady() ? s++ : f++;
			}
			std::cout << "s=" << s << " ";
			std::cout << "f=" << f << " :::: ";
			std::cout << (float)s / 10000 << " vs " << std::endl;

			auto& Entry = m_Computed[static_cast<int>(m_GamePreset.indicator() * 100)];
			float Chance = s / 10000.f;

			auto PrevMin = Entry.Min;
			Entry.Min = std::min(Entry.Min, Chance);
			if (Entry.Min != PrevMin)
				Entry.MinPreset = m_GamePreset;
			auto PrevMax = Entry.Max;
			Entry.Max = std::max(Entry.Max, Chance);
			if (Entry.Max != PrevMax)
				Entry.MaxPreset = m_GamePreset;

			Entry.Avg += Chance;
			Entry.Count++;
		}


		m_GamePreset.setShipAmount(1, m_GamePreset.getShipAmount(1) + 1);
		std::cout << ":: ";
		for (int i = 1; i < m_GamePreset.getMaxShipSize(); i++)
		{
			if (m_GamePreset.getShipAmount(i) > 4)
			{
				m_GamePreset.setShipAmount(i, 0);
				m_GamePreset.setShipAmount(i + 1, m_GamePreset.getShipAmount(i + 1) + 1);
			}
			std::cout << m_GamePreset.getShipAmount(i) << " ";
		}
		std::cout << "\n";
		
	}
	std::ofstream File("result3.txt");
	for (auto &p : m_Computed)
	{
		File << p.first << " " << p.second.Min << " " << p.second.Max << " " << p.second.Avg / static_cast<float>(p.second.Count);
		File << " (";
		int i = 1;
		for (; i < p.second.MinPreset.getMaxShipSize(); i++)
		{
			File << p.second.MinPreset.getShipAmount(i) << ',';
		}
		File << p.second.MinPreset.getShipAmount(i) << ')';

		File << " (";
		i = 1;
		for (; i < p.second.MaxPreset.getMaxShipSize(); i++)
		{
			File << p.second.MaxPreset.getShipAmount(i) << ',';
		}
		File << p.second.MaxPreset.getShipAmount(i) << ')';
		File << std::endl;
	}
	File.close();
	return;*/
	//
    sf::RenderWindow m_Window(sf::VideoMode(m_WindowWidth, m_WindowHeight), "bs", sf::Style::Titlebar | sf::Style::Close);
    m_Window.setFramerateLimit(60);

	CStartGameUi StartUi(&m_GamePreset);
	IScreenContext* Screen = &StartUi;

    while(m_Window.isOpen())
    {
        sf::Event Event;
		while (m_Window.pollEvent(Event))
		{
			Screen->handleInput(Event);
		}
		Screen->run();
		m_Window.clear(sf::Color(255, 255, 255, 255));
		m_Window.draw(*Screen);
		m_Window.display();
    }
}
