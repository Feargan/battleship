#pragma once

#include "shiptemplate.h"

#include <vector>

class CGamePreset
{
public:
	using CSize = std::pair<int, int>;
private:
	std::vector<std::pair<CShipTemplate, int>> m_Templates;
	CSize m_BoardSize;
public:
	CGamePreset();
	~CGamePreset();

	void setShipAmount(unsigned int TemplateId, int Amount);
	int getShipAmount(unsigned int TemplateId) const;
	
	void setBoardSize(CSize Size);
	CSize getBoardSize() const;

	void putTemplate(const CShipLayout& Layout, const std::shared_ptr<void>& External, int Amount);
	const CShipTemplate* getTemplate(unsigned int TemplateId) const;
	unsigned int numTemplates() const;
};

