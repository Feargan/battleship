#pragma once
#include "interface_control.h"
#include "gameboard.h"
#include "extendedpreset.h"

#include <optional>

class CFieldEditor : public IControl
{
	CGameBoard::CField m_Field;
	std::optional<sf::Vector2i> m_CurrentTile;
	CExtendedPreset* m_Preset;
public:
	CFieldEditor(CPanel* Panel, CExtendedPreset* Preset);
	virtual ~CFieldEditor();

	const CGameBoard::CField& getField();
	void resize(int x, int y);

	virtual void handleInput(sf::Event) override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates States) const override;
};

