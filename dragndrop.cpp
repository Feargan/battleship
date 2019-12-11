#include "dragndrop.h"

#include <SFML/Graphics/Sprite.hpp>

CDragNDrop::CDragNDrop(CPanel * Panel) : IControl(Panel)
{

}

CDragNDrop::~CDragNDrop()
{
}

void CDragNDrop::handleInput(sf::Event Event)
{
	if (Event.type == sf::Event::MouseMoved && m_Held)
	{
		m_CursorPos = { Event.mouseMove.x, Event.mouseMove.y };
		event(CEvent::MOVE);
	}
	else if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Button::Left
		&& getPanel()->GetCurrentFocus() == this)
	{
		m_Held = true;
		m_CursorPos = { Event.mouseButton.x, Event.mouseButton.y };
		event(CEvent::PICK_UP);
	}
	else if (Event.type == sf::Event::MouseButtonReleased && Event.mouseButton.button == sf::Mouse::Button::Left && m_Held)
	{
		m_Held = false;
		event(CEvent::RELEASE);
	}
}

void CDragNDrop::autoSize()
{
	setPosition({ getPosition().left, getPosition().top, static_cast<float>(m_Image.getSize().x), static_cast<float>(m_Image.getSize().y) });
}

void CDragNDrop::setImage(sf::Texture Image)
{
	m_Image = Image;
	autoSize();
}

bool CDragNDrop::isHeld() const
{
	return m_Held;
}

void CDragNDrop::rotate(CRotation::CDir Dir) // ...
{
	m_Rotation += Dir;
}

void CDragNDrop::resetRotation()
{
	m_Rotation = CRotation::CValue::NONE;
}

sf::Vector2i CDragNDrop::getCursorPos() const
{
	return m_CursorPos;
}

const sf::Texture & CDragNDrop::getImage() const
{
	return m_Image;
}

void CDragNDrop::draw(sf::RenderTarget & Target, sf::RenderStates states) const
{
	const auto& Pos = getPosition();
	sf::Sprite Spr(m_Image);
	Spr.setPosition(Pos.left, Pos.top);
	Spr.setScale({ Pos.width / Spr.getLocalBounds().width, Pos.height / Spr.getLocalBounds().height });
	Target.draw(Spr);
	if (m_Held)
	{
		Spr.setColor(sf::Color(255, 255, 255, 128));
		Spr.setPosition(static_cast<float>(m_CursorPos.x), static_cast<float>(m_CursorPos.y));
		Spr.rotate(m_Rotation.degrees());
		Spr.setPosition(Spr.getPosition().x - (Spr.getGlobalBounds().left - getCursorPos().x), Spr.getPosition().y - (Spr.getGlobalBounds().top - getCursorPos().y));
		Target.draw(Spr);
	}
}
