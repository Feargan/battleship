#pragma once
#include "interface_control.h"

#include <SFML/Graphics.hpp>

class CSlider : public IControl
{
public:
	enum CEvent
	{
		VALUE_CHANGED,
	};
	struct CResources
	{
		sf::Texture m_TxtBar;
		sf::Texture m_TxtPointer;
		bool load(const char* Filename);
	};
private:
	CResources m_Resources;
	int m_Min;
	int m_Max;
	int m_Div;
	int m_Value;
	bool m_Hover;
public:
	CSlider(CPanel* Panel);
	virtual ~CSlider();

	void setResources(const CResources& Resources);

	void setMin(int Min);
	int getMin() const;
	void setMax(int Max);
	int getMax() const;
	void setDiv(int Div);
	int getDiv() const;

	int getValue() const;
	void setValue(int Value);

	virtual void handleInput(sf::Event Event) override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
};

