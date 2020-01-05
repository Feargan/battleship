#pragma once
#include "button.h"

class CCheckBox : public CButton
{
	bool m_Lock;
public:
	CCheckBox(CPanel* Panel);
	virtual ~CCheckBox();

	virtual void handleInput(sf::Event) override;

	void setState(bool Enable);
	bool getState() const;
};

