#include "gameui.h"
#include "interfaceutils.h"

#include <algorithm>

const unsigned int CGameUi::MaxVoices;

CGameUi::CGameUi(IGameController* Controller, CExtendedPreset* Preset, CIntelligentPlayer* Player, const CGameBoard* PlayerBoard)
	: m_Controller(Controller), m_Preset(Preset), m_NextVoice(0), m_Player(Player), m_PlayerBoard(PlayerBoard)
{
	if (!m_Controller || !m_Preset)
		throw std::exception("CGameUi(): game controller or preset not set");
	if (m_Preset != m_Controller->getPreset())
		throw std::exception("CGameUi(): game controller has incorrect preset");
	if (!m_Controller->isInProgress())
		throw std::exception("CGameUi(): game not in progress");

	m_PlayerBoardPos = { 0, 0 };
	m_PlayerHelperPos = { 0, 300 };

	m_Controller->addObserver(this);

	for (auto p : m_Controller->getPlayers())
	{
		if (p != m_Player)
			m_Enemies.push_back(p);
	}

	// cut ---v
	m_VictoryInfo.setPosition(200, 500);
	m_Font.loadFromFile("arial.ttf");
	m_VictoryInfo.setFont(m_Font);
	m_VictoryInfo.setFillColor(sf::Color());
}


CGameUi::~CGameUi()
{
}

void CGameUi::run()
{
	if(m_Controller)
		m_Controller->run();
}

void CGameUi::handleInput(sf::Event Event)
{
	if (!m_Preset)
		return;

	if (Event.type == sf::Event::MouseMoved)
	{
		for (unsigned i = 0; i < m_Enemies.size(); i++)
		{
			m_CurrentTile = CInterfaceUtils::getTilePos({ m_Preset->getBoardSize().first, m_Preset->getBoardSize().second }, m_Preset->getBasicAssets().m_TileSize, m_PlayerHelperPos + sf::Vector2i(i * 300, 0), { Event.mouseMove.x, Event.mouseMove.y });
			if (m_CurrentTile)
			{
				m_CurrentEnemy = m_Enemies[i];
				break;
			}
		}
		
	}
	else if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (m_CurrentTile && m_Controller->whoseTurn() == m_Player && m_Player->getEnemyField(m_CurrentEnemy)->at(m_CurrentTile->x, m_CurrentTile->y).getState() == CTile::CState::EMPTY)
		{
			m_Controller->attack(m_Player, m_CurrentEnemy, m_CurrentTile->x, m_CurrentTile->y);
		}
	}
}

void CGameUi::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	if (!m_Controller || !m_Preset)
		return;
	// if m_Controller has startd
	// continue
	CInterfaceUtils::drawField(Target, States, *m_Preset, m_PlayerBoard->getField(), m_PlayerBoardPos, {});
	//CInterfaceUtils::drawShips(Target, States, *m_Preset, m_PlayerBoard->getShips(), m_PlayerBoardPos);
	for (unsigned i = 0; i < m_Enemies.size(); i++)
	{
		CInterfaceUtils::drawField(Target, States, *m_Preset, *m_Player->getEnemyField(m_Enemies[i]), m_PlayerHelperPos + sf::Vector2i(i * 300, 0), m_Enemies[i] == m_CurrentEnemy ? m_CurrentTile : std::optional<sf::Vector2i>());
		//CInterfaceUtils::drawShips(Target, States, *m_Preset, m_Player->getEnemyField(m_Enemies[i])->m_DestroyedShips, m_PlayerHelperPos + sf::Vector2i(i * 300, 0));
	}

	Target.draw(m_VictoryInfo);
}

void CGameUi::playSound(const sf::SoundBuffer & Buffer)
{
	m_Sounds[m_NextVoice].setBuffer(Buffer);
	m_Sounds[m_NextVoice].play();
	m_NextVoice = (m_NextVoice+1) % MaxVoices;
}

void CGameUi::onEvent(const CGameEvent & Event)
{
	using CType = CGameEvent::CType;
	switch (Event.m_Type)
	{
	case CType::PLAYER_ATTACKED:
	{
		switch (Event.m_PlayerAttackedEvent.m_State)
		{
		case CTile::CState::HIT:
			playSound(m_Preset->getBasicAssets().m_SndHit);
			break;
		case CTile::CState::MISS:
			playSound(m_Preset->getBasicAssets().m_SndMiss);
			break;
		}

		break;
	}
	case CType::PLAYER_LOST:
		if (Event.m_PlayerLostEvent.m_Loser != m_Player)
			break;
		
		m_VictoryInfo.setString("przegra³eœ XD");
		playSound(m_Preset->getBasicAssets().m_SndVictory);
		break;
	case CType::GAME_FINISHED:
		if (Event.m_GameFinishedEvent.m_Winner == m_Player)
		{
			m_VictoryInfo.setString("wygra³eœ");
			playSound(m_Preset->getBasicAssets().m_SndVictory);
		}
		break;
	}
}

void CGameUi::onEvent(IControl * Control, int Id)
{
	/*if (m_Designer.getBoard().isReady())
	{
		m_PlayerBoard = m_Controller->seat(&m_Player, std::move(m_Designer.getBoard()));
		m_BuildPhase = false;
		m_Controller->start();
	}*/
}
