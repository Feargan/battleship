#pragma once
#include "interface_control.h"

#include <SFML/Graphics/Texture.hpp>

class CDragNDrop : public IControl
{
public:
	enum CEvent
	{
		PICK_UP,
		MOVE,
		RELEASE,
	};
private:
	sf::Texture m_Image;
	sf::Vector2i m_CursorPos;
	bool m_Held;
public:
	CDragNDrop(CPanel* Panel);
	virtual ~CDragNDrop();

	void autoSize();
	void setImage(sf::Texture Image);

	bool isHeld() const;
	sf::Vector2i getCursorPos() const;
	const sf::Texture& getImage() const;
	
	virtual void handleInput(sf::Event Event) override;

	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
};

