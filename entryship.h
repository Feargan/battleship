#pragma once

#include "radiobutton.h"
#include "shiptemplate.h"
#include "extendedpreset.h"

class CEntryShip : public CRadioButton
{
	const CShipTemplate* m_Template;
public:
	CEntryShip(CPanel* Panel);
	virtual ~CEntryShip();

	void setTemplate(const CShipTemplate* Template);
	const CShipTemplate* getTemplate() const;

	virtual void update() override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates States) const override;
};