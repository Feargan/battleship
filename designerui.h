#pragma once

#include "entryship.h"
#include "extendedpreset.h"
#include "radiogroup.h"
#include "gameboard.h"
#include "dragndropship.h"

#include <vector>
#include <memory>
#include <optional>

class CDesignerUi : public sf::Drawable, private IControl::CEventListener // IScreenContext
{
	using CPos = sf::Vector2i;
	std::vector<std::unique_ptr<CEntryShip>> m_Entries;
	CPanel m_ShipToolbar;
	CRadioButton::CResources m_RadioResources;
	CRadioGroup m_RadioGroup;
	CGameBoard m_Board;
	CPos m_BoardPos;
	std::optional<CPos> m_CurrentTile;
	std::optional<CShipTemplate> m_DraggedTemplate;
	bool m_CanPlace;
	CExtendedPreset* m_Preset;
	CDragNDropShip* m_Drag;
public:
	CDesignerUi();
	~CDesignerUi();

	void preparePreset(CExtendedPreset* Preset);

	void handleInput(sf::Event Event);
	void run();

	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;

	virtual void onEvent(IControl* Control, int EventId) override;
};

