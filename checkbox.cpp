#include "checkbox.h"


CCheckBox::CCheckBox(CPanel* Panel) : CButton(Panel), m_Lock(false)
{
}


CCheckBox::~CCheckBox()
{
}

void CCheckBox::handleInput(sf::Event Event)
{
	if (Event.type == sf::Event::MouseMoved)
	{
		if (getPosition().contains(Event.mouseMove.x, Event.mouseMove.y))
			m_Hover = true;
		else
		{
			if(!m_Held)
				m_Hover = false;
		}
	}
	else if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left
		&& getPanel()->getCurrentFocus() == this && !m_Lock)
	{
		m_Held = !m_Held;

		m_Lock = true;
		event(PRESSED);
	}
	else if (Event.type == sf::Event::MouseButtonReleased && Event.mouseButton.button == sf::Mouse::Button::Left && getPanel()->getLastReleased() == this)
	{
		m_Lock = false;
	}
}

void CCheckBox::setState(bool Enable)
{
	m_Held = Enable;
	m_Hover = m_Held;
}

bool CCheckBox::getState() const
{
	return m_Held;
}
