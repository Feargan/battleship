#include "gameui.h"
#include "interfaceutils.h"

#include <algorithm>

const unsigned int CGameUi::MaxVoices;

CGameUi::CGameUi(IGameController* Controller, CExtendedPreset* Preset, CLocalPlayer* Player, const CGameBoard* PlayerBoard)
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
		if (m_CurrentTile && m_Controller->whoseTurn() == m_Player)
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
	CInterfaceUtils::drawShips(Target, States, *m_Preset, m_PlayerBoard->getShips(), m_PlayerBoardPos);
	for (unsigned i = 0; i < m_Enemies.size(); i++)
	{
		CInterfaceUtils::drawField(Target, States, *m_Preset, m_Player->getEnemyField(m_Enemies[i])->m_Field, m_PlayerHelperPos + sf::Vector2i(i * 300, 0), m_Enemies[i] == m_CurrentEnemy ? m_CurrentTile : std::optional<sf::Vector2i>());
		CInterfaceUtils::drawShips(Target, States, *m_Preset, m_Player->getEnemyField(m_Enemies[i])->m_DestroyedShips, m_PlayerHelperPos + sf::Vector2i(i * 300, 0));
	}

	Target.draw(m_VictoryInfo);
}

/*void CGameUi::drawField(sf::RenderTarget& Target, sf::RenderStates States, const CGameBoard::CField& Field, sf::Vector2i Pos) const
{
	const auto& Assets = m_Preset->getBasicAssets();
	for (int i = 0; i < Field.getWidth(); i++)
	{
		for (int j = 0; j < Field.getHeight(); j++)
		{
			sf::Sprite Spr;
			switch (Field.at(i, j).getState())
			{
			case CTile::CState::MISS:
				Spr = sf::Sprite(Assets.m_TxtTileMiss);
				break;
			case CTile::CState::DESTROYED:
			case CTile::CState::HIT:
				Spr = sf::Sprite(Assets.m_TxtTileHit);
				break;
			case CTile::CState::TAKEN:
				Spr = sf::Sprite(Assets.m_TxtTileTaken);
				break;
			default:
				Spr = sf::Sprite(Assets.m_TxtTileEmpty);
			}
			Spr.setPosition(static_cast<float>(i*Assets.m_TileSize.x + Pos.x), static_cast<float>(j*Assets.m_TileSize.y + Pos.y));
			Target.draw(Spr, States);
		}
	}
}*/

/*void CGameUi::drawField(sf::RenderTarget& Target, sf::RenderStates States, const CAiPlayer::CField& Field, sf::Vector2i Pos) const
{
	const auto& Assets = m_Preset->getBasicAssets();
	for (int i = 0; i < Field.getWidth(); i++)
	{
		for (int j = 0; j < Field.getHeight(); j++)
		{
			sf::Sprite Spr;
			switch (Field.at(i, j).getState())
			{
			case CTile::CState::MISS:
				Spr = sf::Sprite(Assets.m_TxtTileMiss);
				break;
			case CTile::CState::DESTROYED:
			case CTile::CState::HIT:
				Spr = sf::Sprite(Assets.m_TxtTileHit);
				break;
			case CTile::CState::TAKEN:
				Spr = sf::Sprite(Assets.m_TxtTileTaken);
				break;
			default:
				Spr = sf::Sprite(Assets.m_TxtTileEmpty);
			}
			Spr.setPosition(static_cast<float>(i*Assets.m_TileSize.x + Pos.x), static_cast<float>(j*Assets.m_TileSize.y + Pos.y));
			Target.draw(Spr, States);
		}
	}
}*/

/*template<typename T>
void CGameUi::drawShips(sf::RenderTarget & Target, sf::RenderStates States, const std::vector<T>& Ships, sf::Vector2i Pos) const
{
	auto TileSize = m_Preset->getBasicAssets().m_TileSize;
	for (const auto p : Ships)
	{
		const CShip& s = *p;
		const auto& Meta = s.getMeta();
		if (!m_Preset->getShipAsset(Meta.m_TemplateId))
			continue;
		sf::Sprite Spr(s.isDestroyed() ? m_Preset->getShipAsset(Meta.m_TemplateId)->m_TxtDestroyed : m_Preset->getShipAsset(Meta.m_TemplateId)->m_TxtAlive);
		Spr.rotate(static_cast<float>(Meta.m_Rotation.degrees()));
		Spr.setPosition(Spr.getPosition().x - (Spr.getGlobalBounds().left - (Pos.x+Meta.m_Pos.first*TileSize.x)), Spr.getPosition().y - (Spr.getGlobalBounds().top - (Pos.y + Meta.m_Pos.second*TileSize.y)));	
		Target.draw(Spr, States);
	}
}*/

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
		m_VictoryInfo.setPosition(200, 500);
		m_Font.loadFromFile("arial.ttf");
		m_VictoryInfo.setFont(m_Font);
		m_VictoryInfo.setFillColor(sf::Color());
		m_VictoryInfo.setString("przegral XD");
		playSound(m_Preset->getBasicAssets().m_SndVictory);
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
