#pragma once
#include "gameboard.h"
#include "extendedpreset.h"
#include "gameboard.h"

#include <optional>
#include <memory>
#include <type_traits>

#include <SFML/Graphics.hpp>

class CInterfaceUtils
{
public:
	static std::optional<sf::Vector2i> getTilePos(sf::Vector2i BoardSize, sf::Vector2i TilePxSize, sf::Vector2i BoardPos, sf::Vector2i CursorPos);
	static void drawField(sf::RenderTarget& Target, sf::RenderStates States, const CExtendedPreset& Preset, const CGameBoard::CField& Field, sf::Vector2i Pos, std::optional<sf::Vector2i> SelectedTile);
	//template<typename T> static void drawShips(sf::RenderTarget& Target, sf::RenderStates States, const CExtendedPreset& Preset, const std::vector<T>& Ships, sf::Vector2i FieldPos); // shared_ptr...
};

/*template<typename T>
void CInterfaceUtils::drawShips(sf::RenderTarget & Target, sf::RenderStates States, const CExtendedPreset & Preset, const std::vector<T>& Ships, sf::Vector2i Pos)
{
	//static_assert(std::is_same<typename std::remove_const<T>::type, CShip>::value, "T is not CShip");
	auto TileSize = Preset.getBasicAssets().m_TileSize;
	for (const auto p : Ships)
	{
		const CShip& s = *p;
		const auto& Meta = s.getMeta();
		if (!Preset.getShipAsset(Meta.m_TemplateId))
			continue;
		sf::Sprite Spr(s.isDestroyed() ? Preset.getShipAsset(Meta.m_TemplateId)->m_TxtDestroyed : Preset.getShipAsset(Meta.m_TemplateId)->m_TxtAlive);
		Spr.rotate(static_cast<float>(Meta.m_Rotation.degrees()));
		Spr.setPosition(Spr.getPosition().x - (Spr.getGlobalBounds().left - (Pos.x + Meta.m_Pos.first*TileSize.x)), Spr.getPosition().y - (Spr.getGlobalBounds().top - (Pos.y + Meta.m_Pos.second*TileSize.y)));
		Target.draw(Spr, States);
	}
}*/

/*template <> void CInterfaceUtils::drawShips<class std::shared_ptr<class CShip>>(sf::RenderTarget & Target, sf::RenderStates States, const CExtendedPreset & Preset, const std::vector<std::shared_ptr<CShip>>& Ships, sf::Vector2i Pos);
template <> void CInterfaceUtils::drawShips<const CShip*>(sf::RenderTarget & Target, sf::RenderStates States, const CExtendedPreset & Preset, const std::vector<const CShip*>& Ships, sf::Vector2i Pos);
template <> void CInterfaceUtils::drawShips<CShip*>(sf::RenderTarget & Target, sf::RenderStates States, const CExtendedPreset & Preset, const std::vector<CShip*>& Ships, sf::Vector2i Pos);*/