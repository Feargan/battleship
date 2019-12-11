#include "panel.h"
#include "interface_control.h"

#include <SFML/Graphics/RenderTexture.hpp>

CPanel::CPanel() : m_Focus(nullptr), m_Released(nullptr)
{
	setArea({ 0, 0, 0, 0 });
}

CPanel::~CPanel()
{
	destroy();
}

void CPanel::addControl(ControlKey Key, IControl* Control)
{
    if(std::find(m_Controls.begin(), m_Controls.end(), Control) == m_Controls.end())
        m_Controls.push_back(Control);
}

void CPanel::deleteControl(ControlKey Key, IControl* Control)
{
    auto It = std::find(m_Controls.begin(), m_Controls.end(), Control);
    if(It != m_Controls.end())
        m_Controls.erase(It);
}

void CPanel::destroy()
{
	while (m_Controls.size())
	{
		IControl* Control = m_Controls[0];
		delete Control;
	}
}

void CPanel::setArea(const sf::Rect<int>& Area)
{
	m_Area = Area;

}

void CPanel::handleInput(sf::Event Event)
{
    if(Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left)
    {
        auto It = std::find_if(m_Controls.begin(), m_Controls.end(),
                               [Event](const IControl* Control)-> bool
                               {
                                   return Control->getPosition().contains(static_cast<float>(Event.mouseButton.x), static_cast<float>(Event.mouseButton.y));
                               });
        if(It != m_Controls.end())
        {
            m_Focus = *It;
            m_Released = nullptr;
        }
    }
    else if(Event.type == sf::Event::MouseButtonReleased && Event.mouseButton.button == sf::Mouse::Button::Left)
    {
        m_Released = m_Focus;
        m_Focus = nullptr;
    }
    for(IControl* Control : m_Controls)
        Control->handleInput(Event);
}

void CPanel::update()
{
	for (IControl* Control : m_Controls)
		Control->update();
}

void CPanel::draw(sf::RenderTarget& Target, sf::RenderStates states) const
{
	//	sf::RenderTexture Subtarget;
	// Subtarget.create(m_Area.width, m_Area.height);
    for(IControl* Control : m_Controls)
        Target.draw(*Control, states);
	// sf::Sprite Spr(Subtarget)
	// Spr.setPosition(m_Area.left, m_Area.top)
	// Target.draw(Spr);
}