#include "gameui.h"
#include "interfaceutils.h"

#include <algorithm>

const unsigned int CGameUi::MaxVoices;
const sf::Vector2i CGameUi::PLAYER_BOARD_POS = sf::Vector2i{ 300, 50 };
const sf::Vector2i CGameUi::HELPER_BOARD_POS = sf::Vector2i{ 300, 300 };

CGameUi::CGameUi(CGameController* Controller, CIntelligentPlayer* Player, const CGameBoard* PlayerBoard, const CGameResources& GameResources, const CUiResources& UiResources)
	: m_Controller(Controller), m_NextVoice(0), m_Player(Player), m_PlayerBoard(PlayerBoard), m_GameResources(GameResources), m_PlayerBoardPos(PLAYER_BOARD_POS), m_PlayerHelperPos(HELPER_BOARD_POS), m_Completed(false)
{
	if (!m_Controller->isInProgress())
		throw std::exception("game not in progress");

	m_Controller->addObserver(this);

	for (auto p : m_Controller->getPlayers())
	{
		if (p != m_Player)
			m_Enemies.push_back(p);
	}

	m_ExitButton = new CButton(&m_Panel);
	m_ExitButton->setResources(UiResources.m_ButtonResources);
	m_ExitButton->setPosition({ 10, 40, 100, 30 });
	m_ExitButton->setTitle("Surrender");
	m_ExitButton->addListener(this);

	sf::Text TemplateText("", UiResources.m_ButtonResources.m_Font, 30);
	m_VictoryText = new CText(&m_Panel);
	m_VictoryText->setProperties(TemplateText);
	m_VictoryText->setPosition({ 50, 250, 200, 40 });

	TemplateText.setCharacterSize(12);
	TemplateText.rotate(270.f);

	CText* Text = new CText(&m_Panel);
	Text->setProperties(TemplateText);
	Text->setPosition({ m_PlayerBoardPos.x - 15, m_PlayerBoardPos.y + 75, 15, 100 });
	Text->setText("Your board");

	m_Panel.autoSize();
}


CGameUi::~CGameUi()
{
}

bool CGameUi::isCompleted() const
{
	return m_Completed;
}

void CGameUi::run()
{
	m_Controller->run();
	m_Panel.update();
}

void CGameUi::handleInput(sf::Event Event)
{
	m_Panel.handleInput(Event);
	if (Event.type == sf::Event::MouseMoved)
	{
		for (unsigned i = 0; i < m_Enemies.size(); i++)
		{
			m_CurrentTile = CInterfaceUtils::getTilePos({ m_PlayerBoard->getField().getWidth(), m_PlayerBoard->getField().getHeight() }, m_GameResources.m_TileSize, m_PlayerHelperPos + sf::Vector2i(i * 300, 0), { Event.mouseMove.x, Event.mouseMove.y });
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
	Target.draw(m_Panel, States);
	Target.draw(m_Panel, States);
	CInterfaceUtils::drawField(Target, States, m_GameResources, m_PlayerBoard->getField(), m_PlayerBoardPos, {});
	for (unsigned i = 0; i < m_Enemies.size(); i++)
		CInterfaceUtils::drawField(Target, States, m_GameResources, *m_Player->getEnemyField(m_Enemies[i]), m_PlayerHelperPos + sf::Vector2i(i * 300, 0), m_Enemies[i] == m_CurrentEnemy ? m_CurrentTile : std::optional<sf::Vector2i>());
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
		case CTile::CState::DESTROYED:
		case CTile::CState::HIT:
			playSound(m_GameResources.m_SndHit);
			break;
		case CTile::CState::MISS:
			playSound(m_GameResources.m_SndMiss);
			break;
		}

		break;
	}
	case CType::PLAYER_LOST:
		if (Event.m_PlayerLostEvent.m_Loser != m_Player)
			break;
		
		m_VictoryText->setText("You lost");
		//playSound(m_GameResources.m_SndVictory);
		m_ExitButton->setTitle("Exit");
		break;
	case CType::GAME_FINISHED:
		if (Event.m_GameFinishedEvent.m_Winner == m_Player)
		{
			m_VictoryText->setText("You won");
			playSound(m_GameResources.m_SndVictory);
		}
		m_ExitButton->setTitle("Exit");
		break;
	}
}

void CGameUi::onEvent(IControl * Control, int Id)
{
	if (Control == m_ExitButton && Id == CButton::CEvent::PRESSED)
		m_Completed = true;
}
