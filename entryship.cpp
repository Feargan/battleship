#include "entryship.h"
#include "shiptemplate.h"

#include <SFML/Graphics/Sprite.hpp>

CEntryShip::CEntryShip(CPanel * Panel) : CRadioButton(Panel)
{
	m_Template = nullptr;
}

CEntryShip::~CEntryShip()
{
}

void CEntryShip::setTemplate(const CShipTemplate * Template) //! Preset, id
{
	m_Template = Template;
	update();
}

const CShipTemplate * CEntryShip::getTemplate() const
{
	return m_Template;
}

void CEntryShip::update()
{
	/*if (!m_Template)
		return;
	auto Rect = getPosition();
	auto Size = static_cast<const CExtendedPreset::CShipAsset*>(m_Template->getExternal().get())->m_TxtAlive.getSize(); // ...
	Rect.width = static_cast<float>(Size.x);
	Rect.height = static_cast<float>(Size.y);
	setPosition(Rect);*/
}

void CEntryShip::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	auto Pos = getPosition();
	CRadioButton::draw(Target, States);
	sf::Sprite Spr(static_cast<const CExtendedPreset::CShipAsset*>(m_Template->getExternal().get())->m_TxtAlive); //!
	Spr.setPosition(Pos.left, Pos.top);
	auto SprPos = Spr.getLocalBounds();
	if (SprPos.width > Pos.width || SprPos.height > Pos.height)
	{
		auto MaxRes = std::max(SprPos.width, SprPos.height);
		sf::Vector2f Scale(getPosition().width / MaxRes, getPosition().height / MaxRes);
		Spr.setScale(Scale);
	}
	Target.draw(Spr);
}
