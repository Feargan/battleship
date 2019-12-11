#pragma once
#include "dragndrop.h"
#include "extendedpreset.h"

class CDragNDropShip : public CDragNDrop
{
	const CShipTemplate* m_Template;
	sf::Vector2i m_TileSize;
public:
	CDragNDropShip(CPanel* Panel);

	void setTemplate(const CExtendedPreset* Preset, unsigned int Id);
	const CShipTemplate* getTemplate() const;

	virtual ~CDragNDropShip();

	//virtual void draw(sf::RenderTarget & Target, sf::RenderStates states) const override;
};

