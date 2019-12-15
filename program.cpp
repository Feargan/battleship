#include "program.h"

#include <iostream>
#include <ctime>

#include "commandui.h"
#include "boardbuilderui.h"
#include "designerui.h"

bool CProgram::init()
{
	srand((unsigned int)time(NULL));
    bool Error = false;


	m_GamePreset.load("preset.bsp");
	if (m_GamePreset.numTemplates() <= 0)
		return true;
	//m_GamePreset.save("preset.bsp");
	/*m_CommandButtons.m_GameUi = &m_GameUi;
	m_CommandButtons.m_Preset = &m_GamePreset;*/
	m_CommandButtons.build(&m_Panel);

    m_TextboxResources.Load("textbox.ifc");

    m_Textbox_Message = new CTextbox(&m_Panel, m_TextboxResources);
	m_Textbox_Message->setPosition({ 400, 200, 180, 30 });
	
	// global config
	m_WindowWidth = 800;
	m_WindowHeight = 600;

	
	//m_GameController = IGameController(&m_GamePreset);
	//m_GameUi = CGameUi(&m_GameController, &m_GamePreset);
    return !Error;
}

void CProgram::run()
{
    sf::RenderWindow m_Window(sf::VideoMode(m_WindowWidth, m_WindowHeight), "bs", sf::Style::Titlebar | sf::Style::Close);
    m_Window.setFramerateLimit(60);
	IGameController Controller = IGameController(&m_GamePreset);
	CAiPlayer AiPlayer(&Controller);
	Controller.seat(&AiPlayer, AiPlayer.buildBoard());
	CAiPlayer AiPlayer2(&Controller);
	Controller.seat(&AiPlayer2, AiPlayer.buildBoard());
	CLocalPlayer Player(&Controller);
	std::unique_ptr<CGameUi> GameUi;
	CBoardBuilderUi BuilderUi;
	BuilderUi.preparePreset(&m_GamePreset); // merge ^

	CDesignerUi DesignerUi;
	IScreenContext* Screen = &BuilderUi;

    while(m_Window.isOpen())
    {
        sf::Event Event;
		while (m_Window.pollEvent(Event))
		{
			Screen->handleInput(Event);
		}

		if (Screen == &BuilderUi && BuilderUi.isLockedIn())
		{
			auto Board = Controller.seat(&Player, BuilderUi.getBoard());
			Controller.start();
			GameUi = std::unique_ptr<CGameUi>(new CGameUi(&Controller, &m_GamePreset, &Player, Board));
			Screen = GameUi.get();
		}
		//Designer.run();
		//m_Panel.update();
		Screen->run();
		m_Window.clear(sf::Color(255, 255, 255, 255));
		m_Window.draw(*Screen);
		//m_Window.draw(Designer);
		m_Window.display();
    }
}
