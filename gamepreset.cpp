#include "gamepreset.h"
#include "gameboard.h"


CGamePreset::CGamePreset()
{
}


CGamePreset::~CGamePreset()
{
}

void CGamePreset::setShipAmount(unsigned int TemplateId, int Amount)
{
	if(TemplateId < numTemplates())
		m_Templates[TemplateId].second = Amount;
}

int CGamePreset::getShipAmount(unsigned int TemplateId) const
{
	if (TemplateId >= numTemplates())
		return 0;
	return m_Templates[TemplateId].second;
}

void CGamePreset::setBoardSize(CSize Size)
{
	m_BoardSize = Size;
}

CGamePreset::CSize CGamePreset::getBoardSize() const
{
	return m_BoardSize;
}

void CGamePreset::putTemplate(const CShipLayout & Layout, const std::shared_ptr<void>& External, int Amount)
{
	m_Templates.emplace_back(CShipTemplate(m_Templates.size(), Layout, External, this), Amount);
}

const CShipTemplate* CGamePreset::getTemplate(unsigned int TemplateId) const
{
	if(TemplateId >= numTemplates())
		return nullptr;
	return &m_Templates[TemplateId].first;
}

unsigned int CGamePreset::numTemplates() const
{
	return m_Templates.size();
}
