#pragma once
#include "interface_control.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class CText :
	public IControl
{
public:
	enum class CMode
	{
		STATIC,
		FLASH,
	};
private:
	sf::Text m_Text;
	sf::Font m_Font;
	sf::Clock m_Clock;
	int m_Timer;
public:
	CText(CPanel* Panel);
	virtual ~CText();

	void setProperties(const sf::Text& Text);
	void setText(const char* Text);
	void setTimer(int Millis);
	void disableTimer();
	void flash();

	void update();

	virtual void handleInput(sf::Event) override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates States) const override;
};

