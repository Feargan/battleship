#pragma once
#include "interface_control.h"
#include "button.h"

#include <unordered_map>

class CTab : public IControl, private IControl::IEventListener
{
	CButton::CResources m_Resources;
	/*
	struct CResources : public CButton::CResources
	{
		sf::Texture m_AreaTxt;
	}
	*/
	std::unordered_map<CButton*, CPanel*> m_Panels;
	CPanel* m_Active;
	CPanel m_TabButtons;
public:
	CTab(CPanel* Panel);
	virtual ~CTab();

	void link(const std::string& TabName, CPanel* Panel);

	void setResources(const CButton::CResources& Resources);

	virtual void handleInput(sf::Event) override;
	virtual void update() override;

	virtual void draw(sf::RenderTarget& Target, sf::RenderStates States) const override;
private:
	virtual void onEvent(IControl* Control, int EventId) override;
};

