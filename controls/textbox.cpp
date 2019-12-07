#include "textbox.h"

#include <fstream>
#include <vector>

int CTextboxResources::Load(const char* Filename) // clean it up: font filename with ws, general remake, change delimiter
{
    const std::string FormatSignature = "IFCTRL_RESOURCE_TEXTBOX";
    int Error = 0;
    std::ifstream File;
    File.open(Filename, std::ios_base::binary);
    if(File.is_open() && File.good())
    {
        std::string Buffer;
        File >> Buffer;
        if(Buffer == FormatSignature)
        {
            File >> m_Size.x >> m_Size.y;
            File >> Buffer;
            m_Font.loadFromFile(Buffer);
            File >> m_FontSize;

            File.get();
            auto PngPos = File.tellg();
            File.seekg(0, File.end);
            int BytesNum = File.tellg() - PngPos;
            File.seekg(PngPos);
            std::vector<char> Bytes(BytesNum);
            File.read(&Bytes[0], BytesNum);
            if(Bytes.size() > 0)
            {
                if(m_Texture.loadFromMemory(static_cast<void*>(&Bytes[0]), Bytes.size()))
                {
                    m_Texture.setSmooth(true);
                    sf::Rect<int> Pos(0, 0, m_Size.x, m_Size.y);
                    m_Sprite = sf::Sprite(m_Texture, Pos);
                }
                else
                    Error = 1;
            }
            else
                Error = 2;
        }
        else
            Error = 3;
    }
    else
        Error = 4;
    File.close();
    return Error;
}

CTextbox::CTextbox(CInterface* Interface, CTextboxResources &Resources)
       : IControl(Interface)
{
    m_Text = "";
    m_BasicResources = Resources;

    m_Active = false;
}

void CTextbox::update()
{
	// animation update

	// animation->play();

	// sprite pos update, or make a texture and draw it depending on the pos - updatePos() not needed
}

void CTextbox::handleInput(sf::Event Event)
{
    if(Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left
       && getInterface()->GetCurrentFocus() == this)
    {
        m_Active = true;
		event(Event::PRESSED);
    }
    else if(Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left
            && getInterface()->GetCurrentFocus() != this)
    {
        m_Active = false;
    }
    else if(Event.type == sf::Event::TextEntered && m_Active)
    {
        if(Event.text.unicode >= 32 && Event.text.unicode <= 126)
        {
            m_Text += static_cast<char>(Event.text.unicode);
			event(Event::TEXT_CHANGED);
        }
        else if(Event.text.unicode == 8 && m_Text.size() > 0)
        {
            m_Text.pop_back();
			event(Event::TEXT_CHANGED);
        }
    }
}

void CTextbox::draw(sf::RenderTarget& Target, sf::RenderStates states) const
{
	sf::Vector2f Pos(getPosition().left, getPosition().top);
	sf::Sprite Spr(m_BasicResources.m_Texture);
	Spr.setPosition(Pos);
	Spr.setScale((float)getPosition().width / m_BasicResources.m_Size.x, (float)getPosition().height / m_BasicResources.m_Size.y);
	sf::Text Content(m_Text, m_BasicResources.m_Font, m_BasicResources.m_FontSize);
	Content.setPosition(Pos);
	Content.setFillColor(sf::Color(0, 0, 0));
    Target.draw(Spr, states);
    Target.draw(Content, states);
}
