#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct CGameResources
{
	sf::SoundBuffer m_SndHit;
	sf::SoundBuffer m_SndDestroy;
	sf::SoundBuffer m_SndMiss;
	sf::SoundBuffer m_SndVictory;
	sf::Texture m_TxtTileTaken;
	sf::Texture m_TxtTileMiss;
	sf::Texture m_TxtTileEmpty;
	sf::Texture m_TxtTileHit;
	sf::Texture m_TxtTileDestroyed;
	sf::Vector2<int> m_TileSize;
};