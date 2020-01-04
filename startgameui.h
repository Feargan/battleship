#pragma once

//#include "entryship.h"
#include "extendedpreset.h"
//#include "radiogroup.h"
#include "slider.h"
#include "text.h"
#include "gameboard.h"
#include "button.h"
#include "screencontext.h"
#include "localplayer.h"
#include "aiplayer.h"
#include "gamecontroller.h"
#include "gameui.h"

#include <vector>
#include <memory>
#include <optional>

class CStartGameUi : public IScreenContext, private IControl::IEventListener
{
	using CPos = sf::Vector2i;
	CPanel m_Panel;
	CButton::CResources m_ButtonResources;
	CSlider::CResources m_SliderResources;
	CText* m_ChanceText;
	CText* m_ErrorText;
	CButton* m_StartButton;
	CButton* m_AutoButton;
	CButton* m_ClearButton;
	std::unique_ptr<CGameUi> m_GameUi;
	struct CShipSlider
	{
		CSlider* m_Slider;
		CText* m_Amount;
		CText* m_Remaining;
	};
	std::vector<CShipSlider> m_ShipSliders;
	CPos m_BoardPos;
	std::optional<CPos> m_CurrentTile;

	CGameBoardBuilder m_Board;
	CLocalPlayer m_Player;
	CAiPlayer m_AiPlayer;
	CAiPlayer m_AiPlayer2;//!!
	IGameController m_Controller;
	CExtendedPreset* m_Preset;
public:
	CStartGameUi(CExtendedPreset* Preset);
	~CStartGameUi();

	virtual void handleInput(sf::Event Event) override;
	virtual void run() override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
private:
	virtual void onEvent(IControl* Control, int EventId) override;

	void updateRemaining();
	void updateChance();
};

