#include "program.h"

#include <iostream>
#include <ctime>

#include "startgameui.h"

bool CProgram::init()
{
	std::ios_base::sync_with_stdio(false);
	srand((unsigned int)time(NULL));

	m_GameResources.m_TileSize = { 20, 20 };
	if (!m_GameResources.m_TxtTileEmpty.loadFromFile("tile_empty.png"))
		return false;
	if (!m_GameResources.m_TxtTileMiss.loadFromFile("tile_miss.png"))
		return false;
	if (!m_GameResources.m_TxtTileHit.loadFromFile("tile_hit.png"))
		return false;
	if (!m_GameResources.m_TxtTileDestroyed.loadFromFile("tile_destroyed.png"))
		return false;
	if (!m_GameResources.m_TxtTileTaken.loadFromFile("tile_taken.png"))
		return false;
	m_GameResources.m_SndHit.loadFromFile("hit.ogg");
	m_GameResources.m_SndMiss.loadFromFile("miss.ogg");
	m_GameResources.m_SndDestroy.loadFromFile("hit.ogg");
	m_GameResources.m_SndVictory.loadFromFile("victory.ogg");

	if (!m_UiResources.m_ButtonResources.load("button.irc"))
		return false;
	if (!m_UiResources.m_SliderResources.load("slider.irc"))
		return false;
	
	// global config
	m_WindowWidth = 800;
	m_WindowHeight = 600;
    return true;
}

void CProgram::run()
{
    sf::RenderWindow m_Window(sf::VideoMode(m_WindowWidth, m_WindowHeight), "Battleship", sf::Style::Titlebar | sf::Style::Close);
    m_Window.setFramerateLimit(60);

	CStartGameUi StartUi(m_GameResources, m_UiResources);
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
