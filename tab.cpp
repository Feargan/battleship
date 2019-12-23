#include "tab.h"


CTab::CTab(CPanel* Panel) : IControl(Panel), m_Active(nullptr)
{
}


CTab::~CTab()
{
}

void CTab::link(const std::string & TabName, CPanel * Panel)
{
	const auto& Pos = getPosition();
	CButton* Button = new CButton(&m_TabButtons);
	Button->setResources(m_Resources);
	Button->setPosition({ static_cast<int>(m_Panels.size()) * 100, 0, 100, 20 }); // 100 - button width, 20 button height
	Button->setTitle(TabName);
	Button->addListener(this);
	m_TabButtons.autoSize();
	Panel->setPosition({ Pos.left - Panel->getPosition().x , Pos.top + 20 - Panel->getPosition().y });
	m_Panels[Button] = Panel;
}

void CTab::setResources(const CButton::CResources& Resources)
{
	m_Resources = Resources;
}

void CTab::handleInput(sf::Event Event)
{
	m_TabButtons.handleInput(Event);
	if (m_Active)
		m_Active->handleInput(Event);
}

void CTab::update()
{
	m_TabButtons.update();
	if (m_Active)
		m_Active->update();
}

void CTab::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	Target.draw(m_TabButtons);
	if (m_Active)
		Target.draw(*m_Active, States);
}

void CTab::onEvent(IControl * Control, int EventId)
{
	m_Active = m_Panels[static_cast<CButton*>(Control)];
}
