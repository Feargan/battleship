#include "dragndropship.h"

#include <SFML/Graphics/Sprite.hpp>

CDragNDropShip::CDragNDropShip(CPanel* Panel) : CDragNDrop(Panel), m_Template(nullptr), m_Rotation(CRotation::CValue::NONE)
{
}


CDragNDropShip::~CDragNDropShip()
{
}

/*void CDragNDropShip::draw(sf::RenderTarget & Target, sf::RenderStates states) const
{

}*/

void CDragNDropShip::setTemplate(const CExtendedPreset* Preset, unsigned int Id)
{
	m_Template = Preset->getTemplate(Id);
	if (m_Template)
	{
		setImage(static_cast<CExtendedPreset::CShipAsset*>(m_Template->getExternal().get())->m_TxtAlive);
		m_TileSize = Preset->getBasicAssets().m_TileSize;
	}
}

const CShipTemplate * CDragNDropShip::getTemplate() const
{
	return m_Template;
}

void CDragNDropShip::rotate(CRotation::CDir Dir) // ...
{
	m_Rotation += Dir;
}

void CDragNDropShip::resetRotation()
{
	m_Rotation = CRotation::CValue::NONE;
}

void CDragNDropShip::draw(sf::RenderTarget & Target, sf::RenderStates states) const
{
	const auto& Pos = getPosition();
	sf::Sprite Spr(getImage());
	Spr.setPosition(Pos.left, Pos.top);
	Spr.setScale({ Pos.width / Spr.getLocalBounds().width, Pos.height / Spr.getLocalBounds().height });
	Target.draw(Spr);
	if (isHeld())
	{
		Spr.setColor(sf::Color(255, 255, 255, 128));
		Spr.setPosition(static_cast<float>(getCursorPos().x), static_cast<float>(getCursorPos().y));
		Spr.setOrigin(m_TileSize.x / 2.f, m_TileSize.y / 2.f);
		Spr.rotate(m_Rotation.degrees());
		if (m_Rotation.get() == CRotation::CValue::HALF)
			Spr.setPosition(Spr.getPosition().x + Spr.getLocalBounds().width - m_TileSize.x, Spr.getPosition().y);
		if (m_Rotation.get() == CRotation::CValue::THREE_QUARTER)
			Spr.setPosition(Spr.getPosition().x, Spr.getPosition().y + Spr.getLocalBounds().width - m_TileSize.x);


		Target.draw(Spr);
	}
}
