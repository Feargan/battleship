#include "interfaceutils.h"

std::optional<sf::Vector2i> CInterfaceUtils::getTilePos(sf::Vector2i BoardSize, sf::Vector2i TilePxSize, sf::Vector2i BoardPos, sf::Vector2i CursorPos)
{
	if (CursorPos.x > BoardPos.x && CursorPos.y > BoardPos.y &&
		CursorPos.x < BoardPos.x + TilePxSize.x * BoardSize.x && CursorPos.y < BoardPos.y + TilePxSize.y * BoardSize.y)
	{
		CursorPos -= BoardPos;
		CursorPos.x /= TilePxSize.x;
		CursorPos.y /= TilePxSize.y;
		return CursorPos;
	}
	return {};
}

void CInterfaceUtils::drawField(sf::RenderTarget & Target, sf::RenderStates States, const CExtendedPreset & Preset, const CIntelligentPlayer::CField & Field, sf::Vector2i Pos, std::optional<sf::Vector2i> SelectedTile)
{
	const auto& Assets = Preset.getBasicAssets();
	for (int i = 0; i < Field.getWidth(); i++)
	{
		for (int j = 0; j < Field.getHeight(); j++)
		{
			sf::Sprite Spr;
			switch (Field.at(i, j).getState())
			{
			case CTile::CState::MISS:
				Spr = sf::Sprite(Assets.m_TxtTileMiss);
				break;
			case CTile::CState::DESTROYED:
			case CTile::CState::HIT:
				Spr = sf::Sprite(Assets.m_TxtTileHit);
				break;
			case CTile::CState::TAKEN:
				Spr = sf::Sprite(Assets.m_TxtTileTaken);
				break;
			default:
				Spr = sf::Sprite(Assets.m_TxtTileEmpty);
			}
			Spr.setPosition(i*Assets.m_TileSize.x + static_cast<float>(Pos.x), j*Assets.m_TileSize.y + static_cast<float>(Pos.y));
			if (SelectedTile && SelectedTile->x == i && SelectedTile->y == j)
				Spr.setColor(sf::Color(128, 255, 128));
			Target.draw(Spr, States);
		}
	}
}

void CInterfaceUtils::drawField(sf::RenderTarget & Target, sf::RenderStates States, const CExtendedPreset & Preset, const CGameBoard::CField & Field, sf::Vector2i Pos, std::optional<sf::Vector2i> SelectedTile)
{
	const auto& Assets = Preset.getBasicAssets();
	for (int i = 0; i < Field.getWidth(); i++)
	{
		for (int j = 0; j < Field.getHeight(); j++)
		{
			sf::Sprite Spr;
			switch (Field.at(i, j).getState())
			{
			case CTile::CState::MISS:
				Spr = sf::Sprite(Assets.m_TxtTileMiss);
				break;
			case CTile::CState::DESTROYED:
			case CTile::CState::HIT:
				Spr = sf::Sprite(Assets.m_TxtTileHit);
				break;
			case CTile::CState::TAKEN:
				Spr = sf::Sprite(Assets.m_TxtTileTaken);
				break;
			default:
				Spr = sf::Sprite(Assets.m_TxtTileEmpty);
			}
			Spr.setPosition(i*Assets.m_TileSize.x + static_cast<float>(Pos.x), j*Assets.m_TileSize.y + static_cast<float>(Pos.y));
			if (SelectedTile && SelectedTile->x == i && SelectedTile->y == j)
				Spr.setColor(sf::Color(128, 255, 128));
			Target.draw(Spr, States);
		}
	}
}

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
		Spr.setPosition(Spr.getPosition().x - (Spr.getGlobalBounds().left - (Pos.x + Meta.Pos.first*TileSize.x)), Spr.getPosition().y - (Spr.getGlobalBounds().top - (Pos.y + Meta.Pos.second*TileSize.y)));
		Target.draw(Spr, States);
	}
}*/

