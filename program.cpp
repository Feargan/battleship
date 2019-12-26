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
	m_GamePreset.setMaxShipSize(6);
	m_GamePreset.setShipAmount(1, 2);
	m_GamePreset.setShipAmount(2, 2);
	m_GamePreset.setShipAmount(3, 2);
	m_GamePreset.setShipAmount(4, 2);
	m_GamePreset.setShipAmount(5, 1);
	m_GamePreset.setShipAmount(6, 0);
	
	// global config
	m_WindowWidth = 800;
	m_WindowHeight = 600;

	
	//m_GameController = IGameController(&m_GamePreset);
	//m_GameUi = CGameUi(&m_GameController, &m_GamePreset);
    return !Error;
}

void CProgram::run()
{
	m_GamePreset.setShipAmount(1, 1);
	m_GamePreset.setShipAmount(2, 0);
	m_GamePreset.setShipAmount(3, 0);
	m_GamePreset.setShipAmount(4, 0);
	m_GamePreset.setShipAmount(5, 0);
	m_GamePreset.setShipAmount(6, 0);
	CGameBoard Board(&m_GamePreset);
	struct CValues
	{
		float Min = 0.f;
		float Max = 1.f;
		float Avg = 0.f;
		int Count = 0;
	};
	std::map<int, CValues> m_Computed;
	while (m_GamePreset.getShipAmount(3) != 3)
	{
		int s = 0, f = 0;
		if (m_GamePreset.indicator() != std::numeric_limits<float>::infinity())
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
			std::cout << (float)s / 10000 << " vs " << m_GamePreset.successChance() << std::endl;
		}


		m_GamePreset.setShipAmount(1, m_GamePreset.getShipAmount(1) + 1);
		std::cout << ":: ";
		for (int i = 1; i <= m_GamePreset.getMaxShipSize(); i++)
		{
			if (m_GamePreset.getShipAmount(i) > 4)
			{
				m_GamePreset.setShipAmount(i, 0);
				m_GamePreset.setShipAmount(i + 1, m_GamePreset.getShipAmount(i + 1) + 1);
			}
			std::cout << m_GamePreset.getShipAmount(i) << " ";
		}
		std::cout << "\n";

		if (m_GamePreset.indicator() == std::numeric_limits<float>::infinity())
		{
			continue;
		}
		auto& Entry = m_Computed[static_cast<int>(m_GamePreset.indicator() * 100)];
		float Chance = s / 10000.f;
		Entry.Min = std::min(Entry.Min, Chance);
		Entry.Max <= std::max(Entry.Max, Chance);
		Entry.Avg += Chance;
		Entry.Count++;
	}
	std::ofstream File("result.txt");
	for (auto &p : m_Computed)
	{
		File << p.first << " " << p.second.Min << " " << p.second.Max << " " << p.second.Avg / p.second.Count << std::endl;
	}
	File.close();
	return;
    sf::RenderWindow m_Window(sf::VideoMode(m_WindowWidth, m_WindowHeight), "bs", sf::Style::Titlebar | sf::Style::Close);
    m_Window.setFramerateLimit(60);
	IGameController Controller = IGameController(&m_GamePreset);
	CAiPlayer AiPlayer(&Controller);
	CLocalPlayer Player(&Controller);

	std::unique_ptr<CGameUi> GameUi;
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
