#include "program.h"

#include <iostream>
#include <ctime>

#include "commandui.h"
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
	IGameController m_Controller = IGameController(&m_GamePreset);
	CGameUi GameUi(&m_Controller, &m_GamePreset);
	CDesignerUi Designer;
	Designer.preparePreset(&m_GamePreset);
    while(m_Window.isOpen())
    {
        sf::Event Event;
		while (m_Window.pollEvent(Event))
		{
			//m_Panel.handleInput(Event);
			//GameUi.handleInput(Event);
			Designer.handleInput(Event);
		}
		Designer.run();
		/*m_Panel.update();
		GameUi.run();
        m_Window.draw(m_Panel);
		m_Window.draw(GameUi);*/
		m_Window.clear(sf::Color(255, 255, 255));
		m_Window.draw(Designer);
		m_Window.display();
    }
}
