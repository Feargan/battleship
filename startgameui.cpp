#include "startgameui.h"
#include "interfaceutils.h"

#include <SFML/Graphics.hpp>

const sf::Vector2i CStartGameUi::BOARD_POS = sf::Vector2i{ 260, 120 };
const sf::Vector2i CStartGameUi::SLIDER_POS = sf::Vector2i{ 10, 50 };

CStartGameUi::CStartGameUi(const CGameResources& GameResources, const CUiResources& UiResources)
	: m_BoardPos(BOARD_POS), m_Controller(&m_Preset), m_AiPlayer(&m_Controller), m_Board(&m_Preset), m_GameResources(GameResources), m_UiResources(UiResources)
{
	m_Preset.setBoardSize({ 10, 10 });
	m_Preset.setMaxShipSize(5);
	m_Preset.setShipAmount(1, 2);
	m_Preset.setShipAmount(2, 2);
	m_Preset.setShipAmount(3, 2);
	m_Preset.setShipAmount(4, 2);
	m_Preset.setShipAmount(5, 1);
	m_Preset.setShipAmount(5, 0);
	m_Board.rebuild();

	auto& ButtonResources = UiResources.m_ButtonResources;
	auto& SliderResources = UiResources.m_SliderResources;

	m_StartButton = new CButton(&m_Panel);
	m_StartButton->setResources(ButtonResources);
	m_StartButton->setPosition({ 200, 10, 100, 100 });
	m_StartButton->setTitle("Start");
	m_StartButton->addListener(this);

	m_AutoButton = new CButton(&m_Panel);
	m_AutoButton->setResources(ButtonResources);
	m_AutoButton->setPosition({ 320, 10, 100, 100 });
	m_AutoButton->setTitle("Auto");
	m_AutoButton->addListener(this);

	m_ClearButton = new CButton(&m_Panel);
	m_ClearButton->setResources(ButtonResources);
	m_ClearButton->setPosition({ 440, 10, 100, 100 });
	m_ClearButton->setTitle("Clear");
	m_ClearButton->addListener(this);

	m_SurroundCheck = new CCheckBox(&m_Panel);
	m_SurroundCheck->setResources(ButtonResources);
	m_SurroundCheck->setPosition({ 10, 270, 200, 40 });
	m_SurroundCheck->setTitle("Surround destroyed");
	m_SurroundCheck->setState(true);

	m_BotCheck = new CCheckBox(&m_Panel);
	m_BotCheck->setResources(ButtonResources);
	m_BotCheck->setPosition({ 10, 320, 200, 40 });
	m_BotCheck->setTitle("Play as a bot");

	sf::Vector2i SliderPos = SLIDER_POS;
	sf::Text TemplateText("", ButtonResources.m_Font, 12);
	for (int i = 1; i <= m_Preset.getMaxShipSize(); i++)
	{
		CSlider* Slider = new CSlider(&m_Panel);
		Slider->setResources(SliderResources);
		Slider->setPosition({ SliderPos.x+20, SliderPos.y+i*30, 100, 15 });
		Slider->setMin(0);
		Slider->setMax(4);
		Slider->setDiv(1);
		Slider->setValue(m_Preset.getShipAmount(i));
		Slider->addListener(this);
		
		CText* Amount = new CText(&m_Panel);
		Amount->setProperties(TemplateText);
		Amount->setPosition({ SliderPos.x + 130, SliderPos.y + i * 30, 15, 15 });
		Amount->setText(std::to_string(m_Preset.getShipAmount(i)).c_str());

		CText* Remaining = new CText(&m_Panel);
		Remaining->setProperties(TemplateText);
		Remaining->setPosition({ SliderPos.x + 150, SliderPos.y + i * 30, 15, 15 });
		Remaining->setText(std::to_string(m_Board.remaining(i)).c_str());

		CText* Size = new CText(&m_Panel);
		Size->setProperties(TemplateText);
		Size->setPosition({ SliderPos.x, SliderPos.y + i * 30, 15, 15 });
		Size->setText(std::to_string(i).c_str());

		m_ShipSliders.push_back({ Slider, Amount, Remaining });
	}
	m_ChanceText = new CText(&m_Panel);
	m_ChanceText->setProperties(TemplateText);
	m_ChanceText->setPosition({ SliderPos.x, SliderPos.y + (m_Preset.getMaxShipSize()+1) * 30, 15, 15 });
	updateChance();

	m_ErrorText = new CText(&m_Panel);
	m_ErrorText->setProperties(TemplateText);
	m_ErrorText->setPosition({ 10, 400, 200, 15 });
	m_ErrorText->setTimer(3000);

	TemplateText.rotate(90.f);
	CText* InfoText = new CText(&m_Panel);
	InfoText->setPosition({ SliderPos.x+15, SliderPos.y - 40, 30, 15 });
	InfoText->setProperties(TemplateText);
	InfoText->setText("Size");

	InfoText = new CText(&m_Panel);
	InfoText->setPosition({ SliderPos.x + 145, SliderPos.y - 40, 30, 15 });
	InfoText->setProperties(TemplateText);
	InfoText->setText("Required");

	InfoText = new CText(&m_Panel);
	InfoText->setPosition({ SliderPos.x + 165, SliderPos.y - 40, 30, 15 });
	InfoText->setProperties(TemplateText);
	InfoText->setText("Remaining");

	m_Panel.autoSize();
}


CStartGameUi::~CStartGameUi()
{
}

void CStartGameUi::handleInput(sf::Event Event)
{
	if (m_GameUi)
	{
		m_GameUi->handleInput(Event);
		return;
	}
	m_Panel.handleInput(Event);
	if (Event.type == sf::Event::MouseMoved)
	{
		m_CurrentTile = CInterfaceUtils::getTilePos({ m_Preset.getBoardSize().first, m_Preset.getBoardSize().second }, m_GameResources.m_TileSize, m_BoardPos, { Event.mouseMove.x, Event.mouseMove.y });
	}
	else if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (m_CurrentTile)
		{
			auto State = m_Board.getField().at(m_CurrentTile->x, m_CurrentTile->y).getState();
			switch (State)
			{
			case CTile::CState::RESERVED:
				m_Board.place(m_CurrentTile->x, m_CurrentTile->y);
				break;
			case CTile::CState::TAKEN:
				m_Board.remove(m_CurrentTile->x, m_CurrentTile->y);
				break;
			case CTile::CState::EMPTY:
				m_Board.commit();
				m_Board.place(m_CurrentTile->x, m_CurrentTile->y);
				break;
			}
			updateRemaining();
		}
	}
	else if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Right)
	{
		m_Board.commit();
		updateRemaining();
	}
}

void CStartGameUi::run()
{
	if (m_GameUi)
	{
		if (!m_GameUi->isCompleted())
			m_GameUi->run();
		else
		{
			m_GameUi = {};
			m_Board.clear();
		}
		return;
	}
	m_Panel.update();
}

void CStartGameUi::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	if (m_GameUi)
	{
		Target.draw(*m_GameUi);
		return;
	}

	const auto& Field = m_Board.getField();
	for (int i = 0; i < Field.getWidth(); i++)
	{
		for (int j = 0; j < Field.getHeight(); j++)
		{
			sf::Sprite Spr;
			switch (Field.at(i, j).getState())
			{
			case CTile::CState::RESERVED:
			case CTile::CState::MISS:
				Spr = sf::Sprite(m_GameResources.m_TxtTileMiss);
				break;
			case CTile::CState::DESTROYED:
			case CTile::CState::HIT:
				Spr = sf::Sprite(m_GameResources.m_TxtTileHit);
				break;
			case CTile::CState::TAKEN:
				Spr = sf::Sprite(m_GameResources.m_TxtTileTaken);
				break;
			default:
				Spr = sf::Sprite(m_GameResources.m_TxtTileEmpty);
			}
			Spr.setPosition(i*m_GameResources.m_TileSize.x + static_cast<float>(m_BoardPos.x), j*m_GameResources.m_TileSize.y + static_cast<float>(m_BoardPos.y));

			if (Field.at(i, j).getState() == CTile::CState::RESERVED)
				Spr.setColor(sf::Color(255, 200, 255));

			if (m_CurrentTile && m_CurrentTile->x == i && m_CurrentTile->y == j)
				Spr.setColor(sf::Color(128, 255, 128));
			Target.draw(Spr, States);
		}
	}
	Target.draw(m_Panel);
}

void CStartGameUi::onEvent(IControl * Control, int EventId)
{
	if (Control == m_StartButton && EventId == CButton::CEvent::PRESSED)
	{
		if (!m_Board.isReady())
		{
			m_ErrorText->setText("Your board is not ready");
			m_ErrorText->flash();
			return;
		}
		auto AiBoard = CGameBoardBuilder(&m_Preset);
		if (m_Preset.minSuccessChance() <= 0.0025f)
		{
			m_ErrorText->setText("Minimum autofill chance too low, aborted.");
			m_ErrorText->flash();
			return;
		}
		do
		{
			AiBoard.clear();
			AiBoard.autoFill();
		} while (!AiBoard.isReady());
		m_Controller = CGameController(&m_Preset);
		m_AiPlayer = CAiPlayer(&m_Controller);
		m_Controller.seat(&m_AiPlayer, AiBoard);
		if (m_BotCheck->getState())
			m_Player = std::make_unique<CAiPlayer>(&m_Controller);
		else
			m_Player = std::make_unique<CLocalPlayer>(&m_Controller);
		auto BoardPtr = m_Controller.seat(m_Player.get(), m_Board);
		m_Player->surroundDestroyed(m_SurroundCheck->getState());
		m_Controller.start();
		m_GameUi = std::unique_ptr<CGameUi>(new CGameUi(&m_Controller, m_Player.get(), BoardPtr, m_GameResources, m_UiResources));
	}
	else if (Control == m_AutoButton && EventId == CButton::CEvent::PRESSED)
	{
		if (m_Preset.minSuccessChance() <= 0.0025f)
		{
			m_ErrorText->setText("Minimum autofill chance too low, aborted.");
			m_ErrorText->flash();
			return;
		}
		if (!m_Board.isEmpty())
		{
			auto Restore = m_Board;
			for (int i = 0; i < 1000; i++)
			{
				m_Board.autoFill();
				if (m_Board.isReady())
					break;
				m_Board = Restore;
			}
			
			if (!m_Board.isReady())
			{
				m_ErrorText->setText("Failed to fill the board, try again");
				m_ErrorText->flash();
			}
		}
		else
		{
			do
			{
				m_Board.clear();
				m_Board.autoFill();
			} while (!m_Board.isReady());
		}

		updateRemaining();
	}
	else if (Control == m_ClearButton && EventId == CButton::CEvent::PRESSED)
	{
		m_Board.clear();
		updateRemaining();
	}
	else if(EventId == CSlider::CEvent::VALUE_CHANGED)
	{
		auto It = std::find_if(m_ShipSliders.begin(), m_ShipSliders.end(), [Control](const CShipSlider& s) {return s.m_Slider == Control;});
		if (It != m_ShipSliders.end())
		{
			int Index = It - m_ShipSliders.begin() + 1;
			m_Preset.setShipAmount(Index, It->m_Slider->getValue());
			It->m_Amount->setText(std::to_string(m_Preset.getShipAmount(Index)).c_str());
			It->m_Remaining->setText(std::to_string(m_Board.remaining(Index)).c_str());
			updateChance();
		}
	}
}

void CStartGameUi::updateRemaining()
{
	for (unsigned int i = 0; i<m_ShipSliders.size(); i++)
		m_ShipSliders[i].m_Remaining->setText(std::to_string(m_Board.remaining(i+1)).c_str());
}

void CStartGameUi::updateChance()
{
	if(m_Preset.minSuccessChance() != 1.f)
		m_ChanceText->setText((std::string("Autofill chance: ") + std::to_string(m_Preset.minSuccessChance()*100) + "-" + std::to_string(m_Preset.maxSuccessChance()*100) + "%").c_str());
	else
		m_ChanceText->setText((std::string("Autofill chance: 100%").c_str()));
}
