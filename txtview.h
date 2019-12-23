#pragma once
#include "interface_control.h"
class CTxtView : public IControl
{
	sf::Texture* m_Texture;
public:
	CTxtView(CPanel* Panel);
	virtual ~CTxtView();

	void setTexture(sf::Texture* Texture);

	virtual void handleInput(sf::Event) override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
};

