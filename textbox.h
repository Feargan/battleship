#pragma once
#include "interface_control.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

struct CTextboxResources // generic template
{
    sf::Texture m_Texture;
    sf::Font m_Font;
    int m_FontSize;
    sf::Vector2<int> m_Size;
    sf::Sprite m_Sprite;

    int Load(const char* Filename);
};

class CTextbox : public IControl
{
public:
	enum Event
	{
		PRESSED=1,
		TEXT_CHANGED,
	};
private:
    std::string m_Text;

    bool m_Active;

    CTextboxResources m_Resources;
public:
    CTextbox(CPanel* Panel, CTextboxResources &Resources); // move rcs to static section? or no
    ~CTextbox(){}

    const std::string& GetText(){return m_Text;}

    void update();

    virtual void handleInput(sf::Event) override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
};