#include "button.h"
#include "sections.h"

#include <vector>
#include <fstream>

#ifdef _WIN32
const std::string FONT_PATH = "C:\\WINDOWS\\Fonts\\";
#elif __linux__
const std::string FONT_PATH = "//usr/share/fonts/";
#endif

bool CButton::CResources::load(const char* Filename)
{
	CSections Sections;
	Sections.loadFromFile(Filename);
	auto TxtNormal = Sections.get("TXT_NORMAL");
	auto TxtPressed = Sections.get("TXT_PRESSED");
	auto TxtHover = Sections.get("TXT_HOVER");
	auto FontName = Sections.get("FONT_NAME");
	auto FontSize = Sections.getInt<int32_t>("FONT_SIZE");
	if (!TxtNormal || !TxtPressed | !TxtHover)
		return false;
	m_TxtNormal.loadFromMemory(&(*TxtNormal)[0], TxtNormal->size());
	m_TxtPressed.loadFromMemory(&(*TxtPressed)[0], TxtPressed->size());
	m_TxtHover.loadFromMemory(&(*TxtHover)[0], TxtHover->size());
	if (FontName && FontSize)
	{
		m_Font.loadFromFile( (FONT_PATH + std::string(FontName->begin(), FontName->end()) ).c_str());
		m_FontSize = *FontSize;
	}
    return true;
}

CButton::CButton(CPanel* Panel)
	: IControl(Panel), m_Held(false), m_Hover(false)
{
}

void CButton::setTitle(const std::string& Title)
{
	m_Title = Title;
}

void CButton::setResources(const CResources & Resources)
{
	m_Resources = Resources;
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
       && getPanel()->GetCurrentFocus() == this)
    {
        m_Held = true;
    }
    else if(Event.type == sf::Event::MouseButtonReleased && Event.mouseButton.button == sf::Mouse::Button::Left && m_Held)
    {
        m_Held = false;
		if (getPosition().contains(Event.mouseButton.x, Event.mouseButton.y))
			event(CEvent::PRESSED);
    }
}

void CButton::draw(sf::RenderTarget& Target, sf::RenderStates states) const
{
	const auto& Pos = getPosition();
	sf::Text TitleText(m_Title, m_Resources.m_Font, m_Resources.m_FontSize);
	TitleText.setPosition(Pos.left + Pos.width / 2.f - TitleText.getGlobalBounds().width / 2.f, static_cast<float>(Pos.top));
	sf::Sprite Spr;
	const sf::Texture* Texture;
	Spr.setPosition(static_cast<float>(Pos.left), static_cast<float>(Pos.top));
	if (m_Held && m_Hover)
		Texture = &m_Resources.m_TxtPressed;
	else if (!m_Held && m_Hover)
		Texture = &m_Resources.m_TxtHover;
	else
		Texture = &m_Resources.m_TxtNormal;
	Spr.setTexture(*Texture);
	Spr.setScale(static_cast<float>(Pos.width) / Texture->getSize().x, static_cast<float>(Pos.height) / Texture->getSize().y);
	Target.draw(Spr, states);
	Target.draw(TitleText, states);
}
