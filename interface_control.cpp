#include "interface_control.h"

IControl::IControl(CInterface* Interface)
{
    m_Interface = Interface;
    m_Interface->addControl(CInterface::ControlKey(), this);
}

IControl::~IControl()
{
    m_Interface->deleteControl(CInterface::ControlKey(), this);
}

void IControl::setPosition(const sf::Rect<int>& Pos)
{
	m_Pos = Pos;
}

void IControl::addListener(CEventListener* Listener)
{
	if (std::find(m_Listeners.begin(), m_Listeners.end(), Listener) == m_Listeners.end())
		m_Listeners.push_back(Listener);
}

void IControl::removeListener(CEventListener* Listener)
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