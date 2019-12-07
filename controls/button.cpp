#include "button.h"

#include <fstream>
#include <vector>

int CButtonResources::load(const char* Filename) // clean it up: font filename with ws, general remake, change delimiter WYWALIC TO XDDD
{
    const std::string FormatSignature = "IFCTRL_RESOURCE_BUTTON";
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
                    m_Idle = sf::Sprite(m_Texture, Pos);
                    Pos.top += m_Size.y;
                    m_Pressed = sf::Sprite(m_Texture, Pos);
                    Pos.top += m_Size.y;
                    m_Hover = sf::Sprite(m_Texture, Pos);
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

CButton::CButton(CInterface* Interface, CButtonResources &Resources)
       : IControl(Interface)
{
    m_Title = "";
    m_BasicResources = Resources;

    m_Held = false;
    m_Hover = false;
}

void CButton::setTitle(const std::string& Title)
{
	m_Title = Title;
}

void CButton::update()
{
	// change this to: m_PosChanged = true when setPosition() was called
	// then if(m_PosChanged) ... continue with updatePos()


	// animation update

	// animation->play();

	// sprite pos update, or make a texture and draw it depending on the pos - updatePos() not needed
	//if (m_Pos == m_LastPos)
	//	return;
	//m_LastPos = m_Pos;
	//updatePos();
}

void CButton::handleInput(sf::Event Event)
{
    if(Event.type == sf::Event::MouseMoved)
    {
        if(getPosition().contains(Event.mouseMove.x, Event.mouseMove.y))
            m_Hover = true;
        else
            m_Hover = false;
    }
    else if(Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left
       && getInterface()->GetCurrentFocus() == this)
    {
        m_Held = true;
    }
    else if(Event.type == sf::Event::MouseButtonReleased && Event.mouseButton.button == sf::Mouse::Button::Left && m_Held)
    {
        m_Held = false;
		if (getPosition().contains(Event.mouseButton.x, Event.mouseButton.y))
			event(Event::PRESSED);
    }
}

void CButton::draw(sf::RenderTarget& Target, sf::RenderStates states) const
{
	sf::Text TitleText(m_Title, m_BasicResources.m_Font, m_BasicResources.m_FontSize);
	TitleText.setPosition(getPosition().left + (float)getPosition().width / 2 - (float)TitleText.getGlobalBounds().width / 2, getPosition().top);
	sf::Sprite Spr(m_BasicResources.m_Texture);
	Spr.setPosition(getPosition().left, getPosition().top);
	Spr.setScale((float)getPosition().width / m_BasicResources.m_Size.x, (float)getPosition().height / m_BasicResources.m_Size.y);
	if (m_Held && m_Hover)
		Spr.setTextureRect(sf::Rect<int>(0, m_BasicResources.m_Size.y, m_BasicResources.m_Size.x, m_BasicResources.m_Size.y));
	else if (!m_Held && m_Hover)
		Spr.setTextureRect(sf::Rect<int>(0, m_BasicResources.m_Size.y*2, m_BasicResources.m_Size.x, m_BasicResources.m_Size.y));
	else
		Spr.setTextureRect(sf::Rect<int>(0,0, m_BasicResources.m_Size.x, m_BasicResources.m_Size.y));

	Target.draw(Spr, states);
	Target.draw(TitleText, states);
}
