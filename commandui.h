#pragma once

#include "interface_control.h"
#include "button.h"
#include "entryship.h"

#include <iostream>

class : private IControl::IEventListener
{
	CButton::CResources m_ButtonResources;
public:
	void build(CPanel* Interface)
	{
		m_ButtonResources.load("button.irc");

		m_ButtonNico = new CButton(Interface);
		m_ButtonNico->setResources(m_ButtonResources);
		m_ButtonNico->setTitle("nico");
		m_ButtonNico->setPosition({ 100, 100, 120, 40 });
		m_ButtonNico->addListener(this);

		m_ButtonExit = new CButton(Interface);
		m_ButtonExit->setResources(m_ButtonResources);
		m_ButtonExit->setTitle("wyjscie");
		m_ButtonExit->setPosition({ 300, 100, 120, 40 });
		m_ButtonExit->addListener(this);

		m_ButtonEgg = new CButton(Interface);
		m_ButtonEgg->setResources(m_ButtonResources);
		m_ButtonEgg->setTitle("jajco");
		m_ButtonEgg->setPosition({ 100, 200, 180, 60 });
		m_ButtonEgg->addListener(this);

	}
	CButton* m_ButtonNico, *m_ButtonExit, *m_ButtonEgg;
	//CRadioButton* m_Radio1, m_Radio2;
	CEntryShip* m_ShipEntry;
	//CGameUi* m_GameUi;
	//CExtendedPreset* m_Preset;
private:
	virtual void onEvent(IControl* Control, int eventId) override
	{
		if (eventId != CButton::CEvent::PRESSED)
			return;

		if (Control == m_ButtonNico)
		{
			std::cout << "button nico" << std::endl;
		}
		else if (Control == m_ButtonExit)
		{
			std::cout << "button exit" << std::endl;
		}
		else if (Control == m_ButtonEgg)
		{
			/*delete m_Controller;
			m_Controller = new IGameController(m_Preset);
			*m_GameUi = CGameUi(m_Controller, m_Preset);*/
		}
	}
}m_CommandButtons;