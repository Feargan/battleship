#include "radiogroup.h"


CRadioGroup::CRadioGroup() : m_LastActive(nullptr)
{
}

void CRadioGroup::add(CRadioButton* Control)
{
	m_Controls.insert(Control);
	Control->addListener(this);
}

CRadioButton* CRadioGroup::getActive() const
{
	if (m_LastActive && m_LastActive->isActive())
		return m_LastActive;
	return nullptr;
}

void CRadioGroup::onEvent(IControl * Control, int Event)
{
	if (Event != CRadioButton::SWITCH_ON)
		return;

	for (auto c : m_Controls)
	{
		if (c != Control)
			c->switchOff();
		else
			m_LastActive = c;
	}
}
