#include "panel.h"
#include "interface_control.h"


CPanel::CPanel(const sf::Rect<int>& Area) : m_Focus(nullptr), m_Released(nullptr)
{
	setArea(Area);
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

	if (m_Area.width > 0 && m_Area.height > 0)
		m_RenderTexture.create(m_Area.width, m_Area.height);
}

const sf::Rect<int>& CPanel::getArea() const
{
	return m_Area;
}

void CPanel::setPosition(sf::Vector2i Pos)
{
	m_Area.left = Pos.x;
	m_Area.top = Pos.y;
}

sf::Vector2i CPanel::getPosition() const
{
	return { m_Area.left, m_Area.top };
}

void CPanel::autoSize()
{
	if (!m_Controls.size())
		return;
	sf::Vector2i Max = { (*m_Controls.begin())->getPosition().width, (*m_Controls.begin())->getPosition().height };
	for (auto c : m_Controls)
	{
		const auto& Pos = c->getPosition();
		if (Pos.left+Pos.width > Max.x)
			Max.x = Pos.left + Pos.width;
		if (Pos.top+Pos.height > Max.y)
			Max.y = Pos.top + Pos.height;
	}
	setArea({ m_Area.left, m_Area.top, Max.x, Max.y });
}

/*void CPanel::setVScrollbar(bool Enable)
{
	m_VScrollbar = Enable;
}

void CPanel::setHScrollbar(bool Enable)
{
	m_HScrollbar = Enable;
}*/

void CPanel::handleInput(sf::Event Event)
{
	normalize(Event);
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

void CPanel::update()
{
	m_RenderTexture.clear(sf::Color(255, 255, 255, 0));
	for (IControl* Control : m_Controls)
	{
		Control->update();
		m_RenderTexture.draw(*Control);
	}
	m_RenderTexture.display();
}

void CPanel::draw(sf::RenderTarget& Target, sf::RenderStates States) const
{
    /*for(IControl* Control : m_Controls)
        Target.draw(*Control, states);*/
	sf::Sprite Spr(m_RenderTexture.getTexture());
	Spr.setPosition(static_cast<float>(m_Area.left), static_cast<float>(m_Area.top));
	Target.draw(Spr, States);
}

void CPanel::normalize(sf::Event & Event) const
{
	int xdif = m_Area.left;
	int ydif = m_Area.top;
	switch (Event.type)
	{
	case sf::Event::MouseButtonPressed:
	case sf::Event::MouseButtonReleased:
		Event.mouseButton.x -= xdif;
		Event.mouseButton.y -= ydif;
		break;
	case sf::Event::MouseMoved:
		Event.mouseMove.x -= xdif;
		Event.mouseMove.y -= ydif;
		break;
	case sf::Event::MouseWheelScrolled:
		Event.mouseWheelScroll.x -= xdif;
		Event.mouseWheelScroll.y -= ydif;
		break;
	}
}
