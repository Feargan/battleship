#pragma once

#include "matrix.h"

#include <memory>

using CShipLayout = CMatrix<bool>;

class CGamePreset;

class CShipTemplate
{
private:
	unsigned int m_Id;
	CShipLayout m_Layout;
	std::shared_ptr<void> m_External;
	const CGamePreset* m_Preset;
public:
	//CShipTemplate(){} //!
	CShipTemplate(unsigned int Id, const CShipLayout& Layout, const std::shared_ptr<void>& External, const CGamePreset* Preset = nullptr);

	unsigned int getId() const;
	const CShipLayout& getLayout() const;
	const std::shared_ptr<void>& getExternal() const;
	const CGamePreset* getPreset() const;
};
