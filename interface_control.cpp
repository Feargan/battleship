#include "interface_control.h"

IControl::IControl(CPanel* Interface)
{
    m_Panel = Interface;
    m_Panel->addControl(CPanel::ControlKey(), this);
}

IControl::~IControl()
{
    m_Panel->deleteControl(CPanel::ControlKey(), this);
}

const CPanel * IControl::getPanel() const
{
	return m_Panel;
}

void IControl::setPosition(const sf::Rect<int>& Pos)
{
	m_Pos = Pos;
}

const sf::Rect<int>& IControl::getPosition() const
{
	return m_Pos;
}

void IControl::addListener(IEventListener* Listener)
{
	if (std::find(m_Listeners.begin(), m_Listeners.end(), Listener) == m_Listeners.end())
		m_Listeners.push_back(Listener);
}

void IControl::removeListener(IEventListener* Listener)
{
	auto It = std::find(m_Listeners.begin(), m_Listeners.end(), Listener);
	if (It != m_Listeners.end())
		m_Listeners.erase(It);
}

void IControl::event(int eventId)
{
	for (auto l : m_Listeners)
		l->onEvent(this, eventId);
}