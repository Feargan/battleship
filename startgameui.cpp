#include "startgameui.h"
#include "interfaceutils.h"

#include <SFML/Graphics.hpp>

CStartGameUi::CStartGameUi(CExtendedPreset* Preset)
	: m_BoardPos{200, 220}, m_Preset(Preset), m_Controller(Preset),  m_AiPlayer(nullptr), m_Player(nullptr) //,m_CanPlace(false), m_Lock(false),
{
	m_Player = CLocalPlayer(&m_Controller);
	m_AiPlayer = CAiPlayer(&m_Controller);
	m_Board = CGameBoard(Preset);
	m_Preset = Preset;

	m_ButtonResources.load("button.irc");
	m_SliderResources.load("slider.irc");

	m_StartButton = new CButton(&m_Panel);
	m_StartButton->setResources(m_ButtonResources);
	m_StartButton->setPosition({ 200, 10, 100, 100 });
	m_StartButton->setTitle("Start");
	m_StartButton->addListener(this);

	m_AutoButton = new CButton(&m_Panel);
	m_AutoButton->setResources(m_ButtonResources);
	m_AutoButton->setPosition({ 320, 10, 100, 100 });
	m_AutoButton->setTitle("Auto");
	m_AutoButton->addListener(this);

	m_ClearButton = new CButton(&m_Panel);
	m_ClearButton->setResources(m_ButtonResources);
	m_ClearButton->setPosition({ 440, 10, 100, 100 });
	m_ClearButton->setTitle("Clear");
	m_ClearButton->addListener(this);

	sf::Vector2i SliderPos = { 10, 10 };
	sf::Text TemplateText("", m_ButtonResources.m_Font, 12);
	for (int i = 1; i <= m_Preset->getMaxShipSize(); i++)
	{
		CSlider* Slider = new CSlider(&m_Panel);
		Slider->setResources(m_SliderResources);
		Slider->setPosition({ SliderPos.x, SliderPos.y+i*30, 100, 15 });
		Slider->setMin(0);
		Slider->setMax(4);
		Slider->setDiv(1);
		Slider->setValue(m_Preset->getShipAmount(i));
		Slider->addListener(this);
		
		CText* Amount = new CText(&m_Panel);
		Amount->setProperties(TemplateText);
		Amount->setPosition({ SliderPos.x + 110, SliderPos.y + i * 30, 15, 15 });
		Amount->setText(std::to_string(m_Preset->getShipAmount(i)).c_str());

		CText* Remaining = new CText(&m_Panel);
		Remaining->setProperties(TemplateText);
		Remaining->setPosition({ SliderPos.x + 130, SliderPos.y + i * 30, 15, 15 });
		Remaining->setText(std::to_string(m_Board.remaining(i)).c_str());

		m_ShipSliders.push_back({ Slider, Amount, Remaining });
	}
	m_ChanceText = new CText(&m_Panel);
	m_ChanceText->setProperties(TemplateText);
	m_ChanceText->setPosition({ SliderPos.x, SliderPos.y + (m_Preset->getMaxShipSize()+1) * 30, 15, 15 });
	updateChance();
		//m_ShipSliders.push_back(std::make_unique<CShipSlider>(&m_Panel, sf::Vector2i{ 10, 10 + i * 30 }, m_SliderResources, m_ButtonResources.m_Font, m_Preset, i));
	m_ErrorText = new CText(&m_Panel);
	m_ErrorText->setProperties(TemplateText);
	m_ErrorText->setPosition({ 10, 400, 200, 15 });
	m_ErrorText->setTimer(3000);
	//m_ErrorText->setText((std::string("Chance to autofill a clear board: ") + std::to_string(m_Preset->successChance())).c_str());

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
		m_CurrentTile = CInterfaceUtils::getTilePos({ m_Preset->getBoardSize().first, m_Preset->getBoardSize().second }, m_Preset->getBasicAssets().m_TileSize, m_BoardPos, { Event.mouseMove.x, Event.mouseMove.y });
		//if (m_CurrentTile)
		//	m_CanPlace = m_Board.canPlace(m_CurrentTile->x, m_CurrentTile->y);

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
				updateRemaining();
				break;
			}
			
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
		m_GameUi->run();
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
	if (!m_Preset)
		return;
	const auto& Field = m_Board.getField();
	const auto& Assets = m_Preset->getBasicAssets();
	for (int i = 0; i < Field.getWidth(); i++)
	{
		for (int j = 0; j < Field.getHeight(); j++)
		{
			sf::Sprite Spr;
			switch (Field.at(i, j).getState())
			{
			case CTile::CState::RESERVED:
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
			Spr.setPosition(i*Assets.m_TileSize.x + static_cast<float>(m_BoardPos.x), j*Assets.m_TileSize.y + static_cast<float>(m_BoardPos.y));

			if (Field.at(i, j).getState() == CTile::CState::RESERVED)
				Spr.setColor(sf::Color(255, 200, 255));

			if (m_CurrentTile && m_CurrentTile->x == i && m_CurrentTile->y == j)
				Spr.setColor(sf::Color(128, 255, 128));
			Target.draw(Spr, States);
		}
	}
	//CInterfaceUtils::drawShips(Target, States, *m_Preset, m_Board.getShips(), m_BoardPos);
	Target.draw(m_Panel);
}

#include <iostream>

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
		CGameBoard AiBoard = CGameBoard(m_Preset);
		AiBoard.autoFill();
		if (!AiBoard.isReady())
		{
			m_ErrorText->setText("Failed to autofill CPU's board, try again");
			m_ErrorText->flash();
			return;
		}
		auto BoardPtr = m_Controller.seat(&m_Player, m_Board);
		m_Controller.seat(&m_AiPlayer, std::move(AiBoard));
		m_Controller.start();
		m_GameUi = std::unique_ptr<CGameUi>(new CGameUi(&m_Controller, m_Preset, &m_Player, BoardPtr));
		//Screen = GameUi.get();
	}
	else if (Control == m_AutoButton && EventId == CButton::CEvent::PRESSED)
	{
		int s = 0, f = 0;
		std::cout << "i=" <<m_Preset->indicator() << " ";
		while(s+f < 10000)
		{
			m_Board.clear();
			m_Board.autoFill();
			m_Board.isReady() ? s++ : f++;
		}
		std::cout << "s=" << s << " ";
		std::cout << "f=" << f << " :::: ";
		std::cout << (float)s / 10000 << " vs " << m_Preset->successChance() << std::endl;
		/*m_Board.autoFill();
		if (!m_Board.isReady())
		{
			m_ErrorText->setText("Failed to fill the board, try again");
			m_ErrorText->flash();
		}
		updateRemaining();*/
	}
	else if (Control == m_ClearButton && EventId == CButton::CEvent::PRESSED)
	{
		m_Board = CGameBoard(m_Preset);
		updateRemaining();
	}
	else if(EventId == CSlider::CEvent::VALUE_CHANGED)
	{
		auto It = std::find_if(m_ShipSliders.begin(), m_ShipSliders.end(), [Control](const CShipSlider& s) {return s.m_Slider == Control;});
		if (It != m_ShipSliders.end())
		{
			int Index = It - m_ShipSliders.begin() + 1;
			m_Preset->setShipAmount(Index, It->m_Slider->getValue());
			It->m_Amount->setText(std::to_string(m_Preset->getShipAmount(Index)).c_str());
			It->m_Remaining->setText(std::to_string(m_Board.remaining(Index)).c_str());
			updateChance();
		}
	}
}

void CStartGameUi::updateRemaining()
{
	for (int i = 0; i<m_ShipSliders.size(); i++)
	{
		//m_ShipSliders[i].m_Amount->setText(std::to_string(m_Preset->getShipAmount(i+1)).c_str());
		m_ShipSliders[i].m_Remaining->setText(std::to_string(m_Board.remaining(i+1)).c_str());
	}
}

void CStartGameUi::updateChance()
{
	float Min = m_Preset->successChance() * 100 - 15.f;
	Min = Min > 0.f ? Min : 0;
	float Max = m_Preset->successChance() * 100 + 15.f;
	Max = Max < 100.f ? Max : 100;
	m_ChanceText->setText((std::string("Chance to autofill a clear board: ") + std::to_string(Min) + "-" + std::to_string(Max) + "%").c_str());
}
