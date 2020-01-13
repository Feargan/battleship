#pragma once

#include "gamecontroller.h"
#include "gameresources.h"
#include "localplayer.h"
#include "aiplayer.h"
#include "screencontext.h"
#include "button.h"
#include "uiresources.h"
#include "text.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <array>
#include <optional>

class CGameUi : public IScreenContext, private CGameController::IObserver, private IControl::IEventListener
{
	using CPos = sf::Vector2i;
	static const CPos PLAYER_BOARD_POS;
	static const CPos HELPER_BOARD_POS;

	CGameResources m_GameResources;

	CPanel m_Panel;
	CButton* m_ExitButton;
	CText* m_VictoryText;

	CGameController* m_Controller;
	CIntelligentPlayer* m_Player;
	const CGameBoard* m_PlayerBoard;
	CPos m_PlayerBoardPos;
	CPos m_PlayerHelperPos;
	std::optional<sf::Vector2i> m_CurrentTile;
	const IPlayer* m_CurrentEnemy;
	std::vector<const IPlayer*> m_Enemies;

	unsigned int m_NextVoice;
	static const unsigned int MaxVoices = 16;
	std::array<sf::Sound, MaxVoices> m_Sounds;

	bool m_Completed;
public:
	CGameUi(CGameController* Controller, CIntelligentPlayer* Player, const CGameBoard* PlayerBoard, const CGameResources& GameResources, const CUiResources& UiResources);
	~CGameUi();

	bool isCompleted() const;

	virtual void run() override;
	virtual void handleInput(sf::Event Event) override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
private:
	virtual void onEvent(const CGameEvent& Event) override;
	virtual void onEvent(IControl* Control, int Id) override;

	void playSound(const sf::SoundBuffer& Buffer);
};