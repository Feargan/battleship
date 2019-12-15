#pragma once

#include "interface_control.h"
#include "button.h"

#include <SFML/Graphics.hpp>

class CRadioButton : public IControl
{
public:
	enum CEvent
	{
		PRESSED = 1,
		SWITCH_ON,
		SWITCH_OFF,
	};
	/*struct CResources
	{
		sf::Texture m_TxtInactive;
		sf::Texture m_TxtActive;
		sf::Font m_Font;
		int m_FontSize;

		bool load(const char* Filename);
	};*/
private:
	bool m_Active;
	std::string m_Title;
	CButton::CResources m_Resources;
public:
	CRadioButton(CPanel* Panel);
	virtual ~CRadioButton();

	void switchOn();
	void switchOff();
	bool isActive() const;

	void setResources(const CButton::CResources& Resources);

	void setTitle(const std::string& Text);
	const std::string& getTitle() const;

	virtual void handleInput(sf::Event) override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
};

