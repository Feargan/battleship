#pragma once

#include "gamepreset.h"
#include "sections.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <vector>
#include <cinttypes>
#include <optional>

class CExtendedPreset : public CGamePreset
{
public:
	struct CShipAsset
	{
		sf::Texture m_TxtAlive;
		sf::Texture m_TxtDestroyed;
		sf::Vector2<int> m_Size;
	};
	struct CBasicAssets
	{
		sf::SoundBuffer m_SndHit;
		sf::SoundBuffer m_SndDestroy;
		sf::SoundBuffer m_SndMiss;
		sf::SoundBuffer m_SndVictory;
		sf::Texture m_TxtTileUnrevealed;
		sf::Texture m_TxtTileTaken;
		sf::Texture m_TxtTileMiss;
		sf::Texture m_TxtTileEmpty;
		sf::Texture m_TxtTileHit;
		sf::Vector2<int> m_TileSize;
	};
private:
	CBasicAssets m_Assets;
public:
	CExtendedPreset();
	~CExtendedPreset();

	bool load(const char* Filename);
	bool save(const char* Filename);

	void putTemplate(const CShipLayout & Layout, const std::shared_ptr<CShipAsset>& Asset, int Amount);
	const CShipAsset* getShipAsset(unsigned int TemplateId) const;
	const CBasicAssets& getBasicAssets() const;

	static std::optional<CShipTemplate> readTemplate(CSections& Sections, const char* Name);
	static std::optional<CShipLayout> readLayout(CSections& Sections, const char* Name);
	static std::optional<sf::Texture> readTexture(CSections& Sections, const char* Name);
	static std::optional<sf::Sound> readSound(CSections& Sections, const char* Name);

	static void writeLayout(CSections& Sections, const char* Name, const CShipLayout& Layout);
};

