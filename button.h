#pragma once
#include "interface_control.h"

#include <SFML/Graphics.hpp>

class CButton : public IControl
{
public:
	struct CResources
	{
		sf::Texture m_TxtNormal;
		sf::Texture m_TxtPressed;
		sf::Texture m_TxtHover;
		sf::Font m_Font;
		int m_FontSize;

		bool load(const char* Filename);
	};
	enum CEvent
	{
		PRESSED=1,
		//HOVER,
		//RELEASED,
	};
private:
    std::string m_Title;

    bool m_Held;
    bool m_Hover;

    CResources m_Resources;
public:
    CButton(CPanel* Panel);
    ~CButton(){}

	void setTitle(const std::string& Title);
	void setResources(const CResources& Resources);

    void update();

    virtual void handleInput(sf::Event) override;
protected:
    virtual void draw(sf::RenderTarget& Target, sf::RenderStates States) const override;
};
