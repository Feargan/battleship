#include "fieldeditor.h"
#include "interfaceutils.h"

CFieldEditor::CFieldEditor(CPanel* Panel, CExtendedPreset* Preset) : IControl(Panel), m_Preset(Preset)
{
}


CFieldEditor::~CFieldEditor()
{
}

const CGameBoard::CField & CFieldEditor::getField()
{
	return m_Field;
}

void CFieldEditor::resize(int x, int y)
{
	m_Field.resize(x, y, CTile::CState::EMPTY);
	//setPosition()
}

void CFieldEditor::handleInput(sf::Event Event)
{
	sf::Vector2i Pos = { getPosition().left, getPosition().top };
	if (Event.type == sf::Event::MouseMoved)
	{
		m_CurrentTile = CInterfaceUtils::getTilePos({ m_Field.getWidth(), m_Field.getHeight() }, m_Preset->getBasicAssets().m_TileSize, Pos, { Event.mouseMove.x, Event.mouseMove.y });
	}
	else if (Event.type == sf::Event::MouseButtonPressed)
	{
		m_CurrentTile = CInterfaceUtils::getTilePos({ m_Field.getWidth(), m_Field.getHeight() }, m_Preset->getBasicAssets().m_TileSize, Pos, { Event.mouseButton.x, Event.mouseButton.y });
		if (m_CurrentTile)
		{
			if (Event.mouseButton.button == sf::Mouse::Button::Left)
			{
				m_Field[{m_CurrentTile->x, m_CurrentTile->y}] = CTile::CState::TAKEN;
			}
			else if (Event.mouseButton.button == sf::Mouse::Button::Right)
			{
				m_Field[{m_CurrentTile->x, m_CurrentTile->y}] = CTile();
			}
		}
	}
}

void CFieldEditor::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	CInterfaceUtils::drawField(Target, States, *m_Preset, m_Field, { getPosition().left, getPosition().top }, m_CurrentTile);
}
