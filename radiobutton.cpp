#include "radiobutton.h"
#include "sections.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#ifdef _WIN32
const std::string FONT_PATH = "C:\\WINDOWS\\Fonts\\";
#endif

bool CRadioButton::CResources::load(const char* Filename)
{
	CSections Sections;
	Sections.loadFromFile(Filename);
	auto TxtInactive = Sections.get("TXT_NORMAL"); // 
	auto TxtActive = Sections.get("TXT_PRESSED"); //
	auto FontName = Sections.get("FONT_NAME");
	auto FontSize = Sections.getInt<int32_t>("FONT_SIZE");
	if (!TxtInactive || !TxtActive)
		return false;
	m_TxtInactive.loadFromMemory(&(*TxtInactive)[0], TxtInactive->size());
	m_TxtActive.loadFromMemory(&(*TxtActive)[0], TxtActive->size());
	if (FontName && FontSize)
	{
		m_Font.loadFromFile((FONT_PATH + std::string(FontName->begin(), FontName->end())).c_str());
		m_FontSize = *FontSize;
	}
	return true;
}

CRadioButton::CRadioButton(CPanel* Panel) : IControl(Panel), m_Active(false)
{
}


CRadioButton::~CRadioButton()
{
}

void CRadioButton::switchOn()
{
	m_Active = true;
	event(SWITCH_ON);
}

void CRadioButton::switchOff()
{
	m_Active = false;
	event(SWITCH_OFF);
}

bool CRadioButton::isActive() const
{
	return m_Active;
}

void CRadioButton::setResources(const CResources & Resources)
{
	m_Resources = Resources;
}

void CRadioButton::setTitle(const std::string & Text)
{
	m_Title = Text;
}

const std::string & CRadioButton::getTitle() const
{
	return m_Title;
}

void CRadioButton::handleInput(sf::Event Event)
{
	if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left
		&& getPanel()->GetCurrentFocus() == this)
	{
		switchOn();
	}
	/*else if (Event.type == sf::Event::MouseButtonReleased && Event.mouseButton.button == sf::Mouse::Button::Left && m_Held)
	{
		m_Held = false;
		if (getPosition().contains(static_cast<float>(Event.mouseMove.x), static_cast<float>(Event.mouseMove.y)))
			event(Event::PRESSED);
	}*/
}

void CRadioButton::draw(sf::RenderTarget & Target, sf::RenderStates states) const
{
	sf::Text TitleText(m_Title, m_Resources.m_Font, m_Resources.m_FontSize);
	TitleText.setPosition(getPosition().left + getPosition().width / 2 - TitleText.getGlobalBounds().width / 2, getPosition().top);
	sf::Sprite Spr;
	const sf::Texture* Texture;
	Spr.setPosition(getPosition().left, getPosition().top);
	if (m_Active)
		Texture = &m_Resources.m_TxtActive;
	else
		Texture = &m_Resources.m_TxtInactive;
	Spr.setTexture(*Texture);
	//Spr.setScale(getPosition().width / Texture->getSize().x, getPosition().height / Texture->getSize().y);
	Target.draw(Spr, states);
	Target.draw(TitleText, states);
}
