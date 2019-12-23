#include "txtview.h"

#include <SFML/Graphics.hpp>

CTxtView::CTxtView(CPanel* Panel) : IControl(Panel), m_Texture(nullptr)
{
}


CTxtView::~CTxtView()
{
}

void CTxtView::setTexture(sf::Texture * Texture)
{
	m_Texture = Texture;
}

void CTxtView::handleInput(sf::Event)
{
	return;
}

void CTxtView::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	if (!m_Texture)
		return;
	sf::Sprite Spr(*m_Texture);
	Spr.setPosition(static_cast<float>(getPosition().left), static_cast<float>(getPosition().top));
	Target.draw(Spr, States);
}
