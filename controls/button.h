#ifndef IFCONTROL_BUTTON_H
#define IFCONTROL_BUTTON_H

#include "../interface_control.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

struct CButtonResources
{
    sf::Texture m_Texture;
    sf::Font m_Font;
    int m_FontSize;
    sf::Vector2<int> m_Size;
    sf::Sprite m_Idle;
    sf::Sprite m_Pressed;
    sf::Sprite m_Hover;

    int load(const char* Filename);
};

class CButton : public IControl
{
public:
	enum Event
	{
		PRESSED=1,
		//HOVER,
		//RELEASED,
	};
private:
    std::string m_Title;

    bool m_Held;
    bool m_Hover;

    CButtonResources m_BasicResources; // this probably should be a reference or deleted
public:
    CButton(CInterface* Interface, CButtonResources &Resources); // move most of this to seperate functions
    ~CButton(){}

	void setTitle(const std::string& Title);

    void update();

    virtual void handleInput(sf::Event) override;
protected:
    virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
};

#endif // IFCONTROL_BUTTON_H
