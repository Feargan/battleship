#pragma once
#include "gameresources.h"
#include "uiresources.h"
#include "slider.h"
#include "text.h"
#include "gameboard.h"
#include "button.h"
#include "checkbox.h"
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
	static const CPos BOARD_POS;
	static const CPos SLIDER_POS;

	CGameResources m_GameResources;
	CUiResources m_UiResources;

	CPanel m_Panel;
	CText* m_ChanceText;
	CText* m_ErrorText;
	CButton* m_StartButton;
	CButton* m_AutoButton;
	CButton* m_ClearButton;
	CCheckBox* m_SurroundCheck;
	CCheckBox* m_BotCheck;
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
	std::unique_ptr<CIntelligentPlayer> m_Player;
	CAiPlayer m_AiPlayer;
	CGameController m_Controller;
	CGamePreset m_Preset;
public:
	CStartGameUi(const CGameResources& GameResources, const CUiResources& UiResources);
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

