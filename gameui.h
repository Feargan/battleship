#pragma once

#include "gamecontroller.h"
#include "extendedpreset.h"
#include "localplayer.h"
#include "aiplayer.h"
#include "boardbuilderui.h"
#include "screencontext.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <array>
#include <optional>

class CGameUi : public IScreenContext, private IGameController::IObserver, private IControl::IEventListener
{
	IGameController* m_Controller;
	CExtendedPreset* m_Preset;

	CLocalPlayer* m_Player;
	const CGameBoard* m_PlayerBoard;
	sf::Vector2i m_PlayerBoardPos, m_PlayerHelperPos;
	std::optional<sf::Vector2i> m_CurrentTile;
	const IPlayer* m_CurrentEnemy;
	std::vector<const IPlayer*> m_Enemies;

	sf::Text m_VictoryInfo;
	sf::Font m_Font;

	unsigned int m_NextVoice;
	static const unsigned int MaxVoices = 16;
	std::array<sf::Sound, MaxVoices> m_Sounds;
public:
	CGameUi(IGameController* Controller, CExtendedPreset* Preset, CLocalPlayer* Player, const CGameBoard* PlayerBoard);
	~CGameUi();

	virtual void run() override;
	virtual void handleInput(sf::Event Event) override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
private:
	virtual void onEvent(const CGameEvent& Event) override;
	virtual void onEvent(IControl* Control, int Id) override;

	//void drawField(sf::RenderTarget& Target, sf::RenderStates states, const CGameBoard::CField& Field, sf::Vector2i Pos) const;
	//void drawField(sf::RenderTarget& Target, sf::RenderStates States, const CAiPlayer::CField& Field, sf::Vector2i Pos) const;
	//template<typename T> void drawShips(sf::RenderTarget & Target, sf::RenderStates States, const std::vector<T>& Destroyed, sf::Vector2i Pos) const;
	void playSound(const sf::SoundBuffer& Buffer);
};

