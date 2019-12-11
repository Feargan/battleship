#include "designerui.h"

#include "dragndropship.h"

#include <SFML/Graphics/Sprite.hpp>

CDesignerUi::CDesignerUi() : m_BoardPos{200, 200}, m_Preset(nullptr), m_CanPlace(false)
{
	m_RadioResources.load("button.irc");
}


CDesignerUi::~CDesignerUi()
{
}

void CDesignerUi::preparePreset(CExtendedPreset * Preset)
{
	if (!Preset)
		return;
	for (unsigned int i = 0; i < Preset->numTemplates(); i++)
	{
		CDragNDropShip* Entry = new CDragNDropShip(&m_ShipToolbar);
		float y = static_cast<float>(i) * 80;
		Entry->setPosition({ 0, y, 0, 0 });
		Entry->setTemplate(Preset, i);
		Entry->addListener(this);
	}
	m_Board = CGameBoard(Preset);
	m_Preset = Preset;
}

void CDesignerUi::handleInput(sf::Event Event)
{
	m_ShipToolbar.handleInput(Event);
	if (Event.type == sf::Event::MouseMoved)
	{
		auto Next = CExtendedPreset::getTilePos({ m_Preset->getBoardSize().first, m_Preset->getBoardSize().second }, m_Preset->getBasicAssets().m_TileSize, m_BoardPos, { Event.mouseMove.x, Event.mouseMove.y });
		if (m_DraggedTemplate && (Next && !m_CurrentTile) || (Next && m_CurrentTile && *Next != *m_CurrentTile))
		{
			m_CanPlace = m_Board.canPlace(Next->x, Next->y, *m_DraggedTemplate);
		}
		m_CurrentTile = Next;

	}
	else if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (m_CurrentTile)
			m_Board.remove(m_CurrentTile->x, m_CurrentTile->y);
	}
	else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Key::R)
	{
		if (m_DraggedTemplate)
		{
			m_DraggedTemplate->rotate(CRotation::CDir::CLOCKWISE);
			m_Drag->rotate(CRotation::CDir::CLOCKWISE);
			m_CanPlace = m_Board.canPlace(m_CurrentTile->x, m_CurrentTile->y, *m_DraggedTemplate);
		}
	}

}

void CDesignerUi::run()
{
	m_ShipToolbar.update();
}

void CDesignerUi::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
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
			if (m_CurrentTile && m_CurrentTile->x == i && m_CurrentTile->y == j)
				Spr.setColor(sf::Color(128, 255, 128));
			if (m_CurrentTile && m_DraggedTemplate)
			{
				const auto& Layout = m_DraggedTemplate->getLayout();
				int PosX = i - m_CurrentTile->x;
				int PosY = j - m_CurrentTile->y;
				if (Layout.checkPoint(PosX, PosY) && Layout.at(PosX, PosY) == 1)
					m_CanPlace ? Spr.setColor(sf::Color(128, 255, 128)) : Spr.setColor(sf::Color(255, 128, 128));
			}
			Target.draw(Spr, States);
		}
	}
	Target.draw(m_ShipToolbar);
}

void CDesignerUi::onEvent(IControl * Control, int EventId)
{
	if (Control->getPanel() == &m_ShipToolbar)
	{
		CDragNDropShip* Drag = static_cast<CDragNDropShip*>(Control);
		if (EventId == CDragNDrop::CEvent::PICK_UP && Drag->getTemplate())
		{
			m_DraggedTemplate = *Drag->getTemplate();
			m_Drag = Drag;
		}
		else if (EventId == CDragNDrop::CEvent::RELEASE)
		{
			if(m_CurrentTile)
				m_Board.place(m_CurrentTile->x, m_CurrentTile->y, *m_DraggedTemplate);
			m_DraggedTemplate = {};
			m_Drag->resetRotation();
		}
	}
}
