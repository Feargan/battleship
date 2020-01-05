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

template<typename T>
void CInterfaceUtils::drawField(sf::RenderTarget & Target, sf::RenderStates States, const CGameResources& Resources, const T& Field, sf::Vector2i Pos, std::optional<sf::Vector2i> SelectedTile)
{
	static_assert(std::is_base_of<CTile, T::ElementType>::value, "field must consist of CTile based elements");
	for (int i = 0; i < Field.getWidth(); i++)
	{
		for (int j = 0; j < Field.getHeight(); j++)
		{
			sf::Sprite Spr;
			switch (Field.at(i, j).getState())
			{
			case CTile::CState::MISS:
				Spr = sf::Sprite(Resources.m_TxtTileMiss);
				break;
			case CTile::CState::DESTROYED:
				Spr = sf::Sprite(Resources.m_TxtTileDestroyed);
				break;
			case CTile::CState::HIT:
				Spr = sf::Sprite(Resources.m_TxtTileHit);
				break;
			case CTile::CState::TAKEN:
				Spr = sf::Sprite(Resources.m_TxtTileTaken);
				break;
			default:
				Spr = sf::Sprite(Resources.m_TxtTileEmpty);
			}
			Spr.setPosition(i*Resources.m_TileSize.x + static_cast<float>(Pos.x), j*Resources.m_TileSize.y + static_cast<float>(Pos.y));
			if (SelectedTile && SelectedTile->x == i && SelectedTile->y == j)
				Spr.setColor(sf::Color(128, 255, 128));
			Target.draw(Spr, States);
		}
	}
}

template void CInterfaceUtils::drawField<CGameBoard::CField>(sf::RenderTarget& Target, sf::RenderStates States, const CGameResources& Resources, const CGameBoard::CField& Field, sf::Vector2i Pos, std::optional<sf::Vector2i> SelectedTile);
template void CInterfaceUtils::drawField<CIntelligentPlayer::CField>(sf::RenderTarget& Target, sf::RenderStates States, const CGameResources& Resources, const CIntelligentPlayer::CField& Field, sf::Vector2i Pos, std::optional<sf::Vector2i> SelectedTile);