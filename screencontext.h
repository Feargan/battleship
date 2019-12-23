#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class IScreenContext : public sf::Drawable
{
public:
	IScreenContext();
	virtual ~IScreenContext();

	virtual void handleInput(sf::Event Event) = 0;
	virtual void run() = 0;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override = 0;
};

