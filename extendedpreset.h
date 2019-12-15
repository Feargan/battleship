#pragma once

#include "gamepreset.h"
#include "sections.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <cinttypes>
#include <optional>
#include <fstream>

class CExtendedPreset : public CGamePreset
{
public:
	enum class CState
	{
		OK,
		INCOMPLETE,
		ERROR,
	};
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
		//sf::Texture m_TxtTileUnrevealed;
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

	CState load(const char* Filename);
	bool save(const char* Filename);

	void putTemplate(const CShipLayout & Layout, const std::shared_ptr<CShipAsset>& Asset, int Amount);
	const CShipAsset* getShipAsset(unsigned int TemplateId) const;
	const CBasicAssets& getBasicAssets() const;

	static void writeSubsections(CSections& MainSections, const char* Name, const CSections& Subsections);
	static std::optional<CShipLayout> readLayout(CSections& Sections, const char* Name);
	static void writeLayout(CSections& Sections, const char* Name, const CShipLayout& Layout);
	template<typename T> static std::optional<T> loadResource(CSections& Sections, const char* Name);
	template<typename T> static void writeResource(CSections& Sections, const char* Name, const T& Resource, const char* Format);

	//static void drawField(sf::RenderTarget& Target, const CExtendedPreset& Preset, const CGameBoard::CField& Field, sf::Vector2i Pos, std::optional<sf::Vector2i> TilePos);
	//template<typename T> static void getAndSetInt(CSections& Sections, const char* Name, T& Integer);
	// template<typename T> static void getAndSetResource(CSections& Sections, const char* Name, T& Resource);
	/*static std::optional<sf::Image> readImage(CSections& Sections, const char* Name);
	static void writeImage(CSections& Sections, const char* Name, const sf::Image& Image);
	static std::optional<sf::SoundBuffer> readSound(CSections& Sections, const char* Name);
	static void writeSound(CSections& Sections, const char* Name, const sf::SoundBuffer Sound);*/
/*private:
	std::optional<CShipTemplate> readNextTemplate(CSections & Sections);*/
};