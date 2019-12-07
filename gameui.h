#pragma once

#include "gamecontroller.h"
#include "extendedpreset.h"
#include "localplayer.h"
#include "aiplayer.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio/Sound.hpp>

#include <vector>
#include <array>

class CGameUi : public sf::Drawable, public IGameController::IObserver
{
	using CScreenPos = sf::Vector2<int>;
	IGameController* m_Controller;
	CExtendedPreset* m_Preset;
	CLocalPlayer m_Player;
	CAiPlayer m_CPU;
	const CGameBoard* m_PlayerBoard;
	const CGameBoard* m_EnemyBoard;
	CScreenPos m_PlayerBoardPos, m_EnemyBoardPos, m_PlayerHelperPos, m_EnemyHelperPos;

	sf::Text m_VictoryInfo;
	sf::Font m_Font;

	unsigned int m_NextVoice;
	static const unsigned int MaxVoices = 16;
	std::array<sf::Sound, MaxVoices> m_Sounds;
public:
	CGameUi(IGameController* Controller = nullptr, CExtendedPreset* Preset = nullptr);
	~CGameUi();

	void run();
	void handleInput(sf::Event Event);
	
	//void getBoards();

	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
private:
	virtual void onEvent(const CGameEvent& Event) override;

	void drawField(sf::RenderTarget& Target, sf::RenderStates states, const CGameBoard::CField& Field, CScreenPos Pos) const;
	void drawField(sf::RenderTarget& Target, sf::RenderStates States, const CAiPlayer::CField& Field, CScreenPos Pos) const;
	template<typename T> void drawShips(sf::RenderTarget & Target, sf::RenderStates States, const std::vector<T>& Destroyed, CScreenPos Pos) const;
	void playSound(const sf::SoundBuffer& Buffer);
};

