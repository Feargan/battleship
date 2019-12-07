#include "interface.h"
#include "interface_control.h"

CInterface::CInterface()
{
    m_Focus = nullptr;
    m_Released = nullptr;
}

CInterface::~CInterface()
{
	destroy();
}

void CInterface::addControl(ControlKey Key, IControl* Control)
{
    if(std::find(m_Controls.begin(), m_Controls.end(), Control) == m_Controls.end())
        m_Controls.push_back(Control);
}

void CInterface::deleteControl(ControlKey Key, IControl* Control)
{
    auto It = std::find(m_Controls.begin(), m_Controls.end(), Control);
    if(It != m_Controls.end())
        m_Controls.erase(It);
}

void CInterface::destroy()
{
	while (m_Controls.size())
	{
		IControl* Control = m_Controls[0];
		delete Control;
	}
}

void CInterface::handleInput(sf::Event Event)
{
    if(Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left)
    {
        auto It = std::find_if(m_Controls.begin(), m_Controls.end(),
                               [Event](const IControl* Control)-> bool
                               {
                                   return Control->getPosition().contains(Event.mouseButton.x, Event.mouseButton.y);
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

void CInterface::update()
{
	for (IControl* Control : m_Controls)
		Control->update();
}

void CInterface::draw(sf::RenderTarget& Target, sf::RenderStates states) const
{
    for(IControl* Control : m_Controls)
        Target.draw(*Control, states);
}