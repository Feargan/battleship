#include "dragndropship.h"

CDragNDropShip::CDragNDropShip(CPanel* Panel) : CDragNDrop(Panel), m_Template(nullptr)
{
}


CDragNDropShip::~CDragNDropShip()
{
}


void CDragNDropShip::setTemplate(const CExtendedPreset* Preset, unsigned int Id)
{
	m_Template = Preset->getTemplate(Id);
	if (m_Template)
		setImage(static_cast<CExtendedPreset::CShipAsset*>(m_Template->getExternal().get())->m_TxtAlive);
}

const CShipTemplate * CDragNDropShip::getTemplate() const
{
	return m_Template;
}
