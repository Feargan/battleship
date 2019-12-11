#include "shiptemplate.h"


CShipTemplate::CShipTemplate(unsigned int Id, const CShipLayout& Layout, const std::shared_ptr<void>& External, const CGamePreset* Preset)
	: m_Id(Id), m_Layout(Layout), m_External(External), m_Preset(Preset)
{
}

void CShipTemplate::rotate(CRotation::CDir Dir)
{
	m_Layout.rotate(Dir);
}

unsigned int CShipTemplate::getId() const
{
	return m_Id;
}

const CShipLayout & CShipTemplate::getLayout() const
{
	return m_Layout;
}

const std::shared_ptr<void>& CShipTemplate::getExternal() const
{
	return m_External;
}

const CGamePreset* CShipTemplate::getPreset() const
{
	return m_Preset;
}
