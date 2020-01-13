#pragma once
#include "radiobutton.h"

#include <unordered_set>

class CRadioGroup : private IControl::IEventListener
{
	std::unordered_set<CRadioButton*> m_Controls;
	CRadioButton* m_LastActive;
public:
	CRadioGroup();

	void add(CRadioButton* Control);
	CRadioButton* getActive() const;
private:
	virtual void onEvent(IControl* Control, int Event) override;
};

