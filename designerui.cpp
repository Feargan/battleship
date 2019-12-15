#include "designerui.h"
#include "interfaceutils.h"
#include "tab.h"

CDesignerUi::CDesignerUi()
{
	if (m_Preset.load("preset.bsp") != CExtendedPreset::CState::OK) // preset.bsp
		throw std::exception("failed to open template preset");

	m_ButtonResources.load("button.irc");
	//m_RadioResources.load("button.irc");

	m_FieldPos = { 170, 50 };

	CButton* Button;
	Button = new CButton(&m_Buttons);
	Button->setResources(m_ButtonResources);
	Button->setPosition({0, 0, 100, 60});
	Button->setTitle("New");
	Button->addListener(this);
	
	Button = new CButton(&m_Buttons);
	Button->setResources(m_ButtonResources);
	Button->setPosition({ 0, 70, 100, 60 });
	Button->setTitle("Load");
	Button->addListener(this);

	Button = new CButton(&m_Buttons);
	Button->setResources(m_ButtonResources);
	Button->setPosition({ 0, 150, 100, 60 });
	Button->setTitle("Save");
	Button->addListener(this);

	Button = new CButton(&m_Buttons);
	Button->setResources(m_ButtonResources);
	Button->setPosition({ 0, 230, 100, 60 });
	Button->setTitle("Save as");
	Button->addListener(this);

	CRadioButton* Radio;
	Radio = new CRadioButton(&m_BrushPanel);
	Radio->setResources(m_ButtonResources);
	Radio->setPosition({ 0, 0, 100, 30 });
	Radio->setTitle("Draw");
	Radio->addListener(this);
	m_BrushGroup.add(Radio);
	Radio->switchOn();

	Radio = new CRadioButton(&m_BrushPanel);
	Radio->setResources(m_ButtonResources);
	Radio->setPosition({ 0, 40, 100, 30 });
	Radio->setTitle("Erase");
	Radio->addListener(this);
	m_BrushGroup.add(Radio);

	m_Buttons.setPosition({ 10, 10 });
	m_Buttons.autoSize();
	m_BrushPanel.setPosition({ 10, 10 });
	m_BrushPanel.autoSize();

	CTab* Tab = new CTab(&m_Editor);
	Tab->setResources(m_ButtonResources);
	Tab->setPosition({ 10, 10, 300, 200 });
	Tab->link("File", &m_Buttons);
	Tab->link("Brush", &m_BrushPanel);

	m_Editor.setArea({ 0, 0, 200, 400 });

	m_EditedField = CGameBoard::CField(5, 5);
}


CDesignerUi::~CDesignerUi()
{
}

void CDesignerUi::run()
{
	//m_BrushPanel.update();
	//m_Buttons.update();
	m_Editor.update();
}

void CDesignerUi::handleInput(sf::Event Event)
{
	//m_Buttons.handleInput(Event);
	//m_BrushPanel.handleInput(Event);
	m_Editor.handleInput(Event);
	if (Event.type == sf::Event::MouseMoved)
	{
		m_CurrentTile = CInterfaceUtils::getTilePos({ m_EditedField.getWidth(), m_EditedField.getHeight() }, m_Preset.getBasicAssets().m_TileSize, m_FieldPos, { Event.mouseMove.x, Event.mouseMove.y });
	}
	else if (Event.type == sf::Event::MouseButtonPressed)
	{
		m_CurrentTile = CInterfaceUtils::getTilePos({ m_EditedField.getWidth(), m_EditedField.getHeight() }, m_Preset.getBasicAssets().m_TileSize, m_FieldPos, { Event.mouseButton.x, Event.mouseButton.y });
		if (m_CurrentTile)
		{
			if (Event.mouseButton.button == sf::Mouse::Button::Left)
			{
				m_EditedField[{m_CurrentTile->x, m_CurrentTile->y}] = CTile::CState::TAKEN;
			}
			else if (Event.mouseButton.button == sf::Mouse::Button::Right)
			{
				m_EditedField[{m_CurrentTile->x, m_CurrentTile->y}] = CTile();
			}
		}
			//m_Board.remove(m_CurrentTile->x, m_CurrentTile->y);
	}
}

void CDesignerUi::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	//Target.draw(m_BrushPanel, States);
	//Target.draw(m_Buttons, States);
	Target.draw(m_Editor, States);
	CInterfaceUtils::drawField(Target, States, m_Preset, m_EditedField, m_FieldPos, m_CurrentTile);
}

void CDesignerUi::onEvent(IControl * Control, int Id)
{
}
