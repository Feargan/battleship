#pragma once

//#include "entryship.h"
#include "extendedpreset.h"
//#include "radiogroup.h"
#include "gameboard.h"
#include "dragndropship.h"
#include "button.h"
#include "screencontext.h"

#include <vector>
#include <memory>
#include <optional>

class CBoardBuilderUi : public IScreenContext, private IControl::IEventListener // IScreenContext
{
	using CPos = sf::Vector2i;
	CPanel m_Panel;
	CButton::CResources m_ButtonResources;
	CGameBoard m_Board;
	CPos m_BoardPos;
	std::optional<CPos> m_CurrentTile;
	std::optional<CShipTemplate> m_DraggedTemplate;
	CDragNDropShip* m_Drag;
	CButton* m_StartButton;
	bool m_CanPlace;
	CExtendedPreset* m_Preset;
	bool m_Lock;
public:
	CBoardBuilderUi(IControl::IEventListener* ReadyObserver = nullptr);
	~CBoardBuilderUi();

	void preparePreset(CExtendedPreset* Preset);

	CGameBoard& getBoard();
	bool isLockedIn() const;

	virtual void handleInput(sf::Event Event) override;
	virtual void run() override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
private:
	virtual void onEvent(IControl* Control, int EventId) override;
};

