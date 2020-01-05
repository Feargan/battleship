#pragma once
#include "gameboard.h"
#include "gameresources.h"
#include "gameboard.h"
#include "intelligentplayer.h"

#include <optional>
#include <memory>
#include <type_traits>

#include <SFML/Graphics.hpp>

class CInterfaceUtils
{
public:
	static std::optional<sf::Vector2i> getTilePos(sf::Vector2i BoardSize, sf::Vector2i TilePxSize, sf::Vector2i BoardPos, sf::Vector2i CursorPos);
	//static void drawField(sf::RenderTarget & Target, sf::RenderStates States, const CGameResources& Resources, const CIntelligentPlayer::CField & Field, sf::Vector2i Pos, std::optional<sf::Vector2i> SelectedTile);
	//static void drawField(sf::RenderTarget& Target, sf::RenderStates States, const CGameResources& Resources, const CGameBoard::CField& Field, sf::Vector2i Pos, std::optional<sf::Vector2i> SelectedTile);
	template<typename T> static void drawField(sf::RenderTarget& Target, sf::RenderStates States, const CGameResources& Resources, const T& Field, sf::Vector2i Pos, std::optional<sf::Vector2i> SelectedTile);
};

/*template <> void CInterfaceUtils::drawShips<class std::shared_ptr<class CShip>>(sf::RenderTarget & Target, sf::RenderStates States, const CExtendedPreset & Preset, const std::vector<std::shared_ptr<CShip>>& Ships, sf::Vector2i Pos);
template <> void CInterfaceUtils::drawShips<const CShip*>(sf::RenderTarget & Target, sf::RenderStates States, const CExtendedPreset & Preset, const std::vector<const CShip*>& Ships, sf::Vector2i Pos);
template <> void CInterfaceUtils::drawShips<CShip*>(sf::RenderTarget & Target, sf::RenderStates States, const CExtendedPreset & Preset, const std::vector<CShip*>& Ships, sf::Vector2i Pos);*/