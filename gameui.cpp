#include "gameui.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <algorithm>

const unsigned int CGameUi::MaxVoices;

CGameUi::CGameUi(IGameController* Controller, CExtendedPreset* Preset)
	: m_Controller(Controller), m_Preset(Preset), m_Player(Controller), m_CPU(Controller), m_NextVoice(0)
{
	if (!m_Controller || !m_Preset)
		return;

	if (m_Preset != m_Controller->getPreset())
		throw;

	CGameBoard Board1 = CGameBoard(m_Preset);
	Board1.place(0, 0, *m_Preset->getTemplate(0));
	Board1.place(0, 2, *m_Preset->getTemplate(0));
	Board1.place(3, 5, *m_Preset->getTemplate(1));
	Board1.place(4, 8, *m_Preset->getTemplate(2));
	Board1.place(9, 1, *m_Preset->getTemplate(3));

	CGameBoard Board2(m_Preset);
	Board2.place(1, 9, *m_Preset->getTemplate(0));
	Board2.place(4, 1, *m_Preset->getTemplate(0));
	Board2.place(0, 0, *m_Preset->getTemplate(1));
	Board2.place(0, 7, *m_Preset->getTemplate(2));
	Board2.place(9, 1, *m_Preset->getTemplate(3));

	m_PlayerBoardPos = { 0, 0 };
	m_EnemyBoardPos = { 400, 0 };
	m_PlayerHelperPos = { 0, 300 };
	m_EnemyHelperPos = { 400, 300 };

	m_PlayerBoard = m_Controller->seat(&m_Player, std::move(Board1));
	m_EnemyBoard = m_Controller->seat(&m_CPU, std::move(Board2));

	m_Player.setName("test1");
	m_CPU.setName("aea2");
	m_Controller->addObserver(this);
	m_Controller->start();
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
	if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left)
	{
		int x = Event.mouseButton.x;
		int y = Event.mouseButton.y;
		int TileWidth = m_Preset->getBasicAssets().m_TileSize.x;
		int TileHeight = m_Preset->getBasicAssets().m_TileSize.y;
		int BoardWidth = m_Preset->getBoardSize().first;
		int BoardHeight = m_Preset->getBoardSize().second;
		if (m_Controller->whoseTurn() == &m_Player
			&& x > m_PlayerHelperPos.x && y > m_PlayerHelperPos.y
			&& x < m_PlayerHelperPos.x + TileWidth * BoardWidth && y < m_PlayerHelperPos.y + TileHeight * BoardHeight)
		{
			x -= m_PlayerHelperPos.x;
			y -= m_PlayerHelperPos.y;
			x /= TileWidth;
			y /= TileHeight;
			m_Controller->attack(&m_Player, &m_CPU, x, y);
		}
		else if (m_Controller->whoseTurn() == &m_CPU
				&& x > m_EnemyHelperPos.x && y > m_EnemyHelperPos.y
				&& x < m_EnemyHelperPos.x + TileWidth * BoardWidth && y < m_EnemyHelperPos.y + TileHeight * BoardHeight)
		{
			x -= m_EnemyHelperPos.x;
			y -= m_EnemyHelperPos.y;
			x /= TileWidth;
			y /= TileHeight;
			m_Controller->attack(&m_CPU, &m_Player, x, y);
		}
	}
	else if (Event.type == sf::Event::MouseButtonReleased && Event.mouseButton.button == sf::Mouse::Button::Left)
	{

	}
}

void CGameUi::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	if (!m_Controller || !m_Preset)
		return;
	drawField(Target, States, m_PlayerBoard->getField(), m_PlayerBoardPos);
	drawShips(Target, States, m_PlayerBoard->getShips(), m_PlayerBoardPos);
	drawField(Target, States, (*m_Player.getEnemyField(&m_CPU)).m_Field, m_PlayerHelperPos);
	drawShips(Target, States, (*m_Player.getEnemyField(&m_CPU)).m_DestroyedShips, m_PlayerHelperPos);

	drawField(Target, States, m_EnemyBoard->getField(), m_EnemyBoardPos);
	drawField(Target, States, *m_CPU.getEnemyField(&m_Player), m_EnemyHelperPos);

	Target.draw(m_VictoryInfo);
}

void CGameUi::drawField(sf::RenderTarget& Target, sf::RenderStates States, const CGameBoard::CField& Field, CScreenPos Pos) const
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
}

void CGameUi::drawField(sf::RenderTarget& Target, sf::RenderStates States, const CAiPlayer::CField& Field, CScreenPos Pos) const
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
}

template<typename T>
void CGameUi::drawShips(sf::RenderTarget & Target, sf::RenderStates States, const std::vector<T>& Ships, CScreenPos Pos) const
{
	auto TileSize = m_Preset->getBasicAssets().m_TileSize;
	for (const auto p : Ships)
	{
		const CShip& s = *p;
		const auto& Meta = s.getMeta();
		if (!m_Preset->getShipAsset(Meta.m_TemplateId))
			continue;
		sf::Sprite Spr(s.isDestroyed() ? m_Preset->getShipAsset(Meta.m_TemplateId)->m_TxtDestroyed : m_Preset->getShipAsset(Meta.m_TemplateId)->m_TxtAlive);
		float Width = Spr.getLocalBounds().width / 2.f, Height = Spr.getLocalBounds().height / 2.f;
		Spr.setOrigin(TileSize.x/2.f, TileSize.y/2.f);
		Spr.setPosition(Pos.x + Meta.m_Pos.first*TileSize.x + TileSize.x / 2.f, Pos.y + Meta.m_Pos.second*TileSize.y + TileSize.y / 2.f);
		Spr.rotate(static_cast<float>(Meta.m_Rotation.degrees()));
		
		
		Target.draw(Spr, States);
	}
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
		m_VictoryInfo.setPosition(200, 500);
		m_Font.loadFromFile("arial.ttf");
		m_VictoryInfo.setFont(m_Font);
		m_VictoryInfo.setFillColor(sf::Color());
		m_VictoryInfo.setString("przegral XD");
		playSound(m_Preset->getBasicAssets().m_SndVictory);
		break;
	}
}
