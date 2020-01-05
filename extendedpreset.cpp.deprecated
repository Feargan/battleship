#include "extendedpreset.h"
#include "sections.h"

CExtendedPreset::CExtendedPreset()
	: CGamePreset()
{
}


CExtendedPreset::~CExtendedPreset()
{
}

#include <iostream>
#include <filesystem>

bool CExtendedPreset::load(const char * Filename)
{
	CSections Sections;
	if (Sections.loadFromFile(Filename) != CSections::CState::LOAD_OK)
		return false;
	auto BoardWidth = Sections.getInt<int32_t>("BW");
	auto BoardHeight = Sections.getInt<int32_t>("BH");
	auto TileWidth = Sections.getInt<int32_t>("TW");
	auto TileHeight = Sections.getInt<int32_t>("TH");
	auto TxtTileEmpty = Sections.get("TXT_TILE_EMPTY");
	auto TxtTileTaken = Sections.get("TXT_TILE_TAKEN");
	auto TxtTileMiss = Sections.get("TXT_TILE_MISS");
	auto TxtTileHit = Sections.get("TXT_TILE_HIT");
	auto SndHit = Sections.get("SND_HIT");
	auto SndMiss = Sections.get("SND_MISS");
	auto SndDestroy = Sections.get("SND_DESTROY");
	auto SndVictory = Sections.get("SND_VICTORY");

	if (!BoardWidth || !BoardHeight || !TileWidth || !TileHeight || !TxtTileEmpty || !TxtTileTaken || !TxtTileMiss || !TxtTileHit) // wrap in throw
		return false;
	setBoardSize({ *BoardWidth, *BoardHeight });
	m_Assets.m_TileSize = { *TileWidth, *TileHeight };
	m_Assets.m_TxtTileEmpty.loadFromMemory(&(*TxtTileEmpty)[0], TxtTileEmpty->size());
	m_Assets.m_TxtTileTaken.loadFromMemory(&(*TxtTileTaken)[0], TxtTileTaken->size());
	m_Assets.m_TxtTileHit.loadFromMemory(&(*TxtTileHit)[0], TxtTileHit->size());
	m_Assets.m_TxtTileMiss.loadFromMemory(&(*TxtTileMiss)[0], TxtTileMiss->size());
	//optional
	if(SndHit)
		m_Assets.m_SndHit.loadFromMemory(&(*SndHit)[0], SndHit->size());
	if (SndMiss)
		m_Assets.m_SndMiss.loadFromMemory(&(*SndMiss)[0], SndMiss->size());
	if (SndDestroy)
		m_Assets.m_SndDestroy.loadFromMemory(&(*SndDestroy)[0], SndDestroy->size());
	if (SndVictory)
		m_Assets.m_SndVictory.loadFromMemory(&(*SndVictory)[0], SndVictory->size());

	m_Assets.m_SndHit.loadFromMemory(&(*SndHit)[0], SndHit->size());
	m_Assets.m_SndVictory.loadFromMemory(&(*SndVictory)[0], SndVictory->size());
	return true;
}

bool CExtendedPreset::save(const char * Filename)
{
	CSections Sections;
	Sections.putInt("BW", getBoardSize().first);
	Sections.putInt("BH", getBoardSize().second);
	Sections.putInt("TW", m_Assets.m_TileSize.x);
	Sections.putInt("TH", m_Assets.m_TileSize.y);
	try
	{
		writeResource(Sections, "TXT_TILE_EMPTY", m_Assets.m_TxtTileEmpty.copyToImage(), "png");
		writeResource(Sections, "TXT_TILE_TAKEN", m_Assets.m_TxtTileTaken.copyToImage(), "png");
		writeResource(Sections, "TXT_TILE_MISS", m_Assets.m_TxtTileMiss.copyToImage(), "png");
		writeResource(Sections, "TXT_TILE_HIT", m_Assets.m_TxtTileHit.copyToImage(), "png");
		writeResource(Sections, "SND_HIT", m_Assets.m_SndHit, "ogg");
		writeResource(Sections, "SND_MISS", m_Assets.m_SndMiss, "ogg");
		writeResource(Sections, "SND_DESTROY", m_Assets.m_SndDestroy, "ogg");
		writeResource(Sections, "SND_VICTORY", m_Assets.m_SndVictory, "ogg");
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	if (Sections.saveToFile(Filename) != CSections::CState::SAVE_OK)
		return false;
	return true;
}

const CExtendedPreset::CBasicAssets & CExtendedPreset::getBasicAssets() const
{
	return m_Assets;
}

CExtendedPreset::CBasicAssets & CExtendedPreset::getBasicAssets()
{
	return m_Assets;
}

void CExtendedPreset::writeSubsections(CSections & MainSections, const char * Name, const CSections & Subsections)
{
	std::vector<char> SectionBuffer;
	Subsections.saveToBuffer(SectionBuffer);
	MainSections.put(Name, SectionBuffer);
}

std::optional<CShipLayout> CExtendedPreset::readLayout(CSections& Sections, const char* Name)
{
	auto Vec = Sections.get(Name);
	if (!Vec)
		return {};
	CSections LayoutSections;
	LayoutSections.loadFromBuffer(*Vec);
	auto Width = LayoutSections.getInt<int32_t>("W");
	auto Height = LayoutSections.getInt<int32_t>("H");
	auto LayoutBuffer = LayoutSections.get("L");
	if (!Width || !Height || !LayoutBuffer || LayoutBuffer->size() < static_cast<unsigned int>(*Height * (*Width)))
		return {};
	CShipLayout Layout(*Width, *Height);
	for (int i = 0; i < *Height; i++)
		for (int j = 0; j < *Width; j++)
			Layout[{j, i}] = (*LayoutBuffer)[i*(*Width) + j];
	return Layout;
}

void CExtendedPreset::writeLayout(CSections& Sections, const char * Name, const CShipLayout & Layout)
{
	CSections LayoutSections;
	int Width = Layout.getWidth(), Height = Layout.getHeight();
	std::vector<char> LayoutBuffer(Width*Height);
	LayoutSections.putInt<int32_t>("W", Width);
	LayoutSections.putInt<int32_t>("H", Height);
	for (int i = 0; i < Height; i++)
		for (int j = 0; j < Width; j++)
			LayoutBuffer[i*Width + j] = Layout[{j, i}];
	LayoutSections.put("L", LayoutBuffer);
	writeSubsections(Sections, Name, LayoutSections);
}

template<typename T> static std::optional<T> CExtendedPreset::loadResource(CSections& Sections, const char* Name)
{
	auto Vec = Sections.get(Name);
	if (!Vec)
		return {};
	T Resource;
	if (!Resource.loadFromMemory(*Vec, Vec->size()))
		return {};
	return Resource;
}

template<typename T> static void CExtendedPreset::writeResource(CSections& Sections, const char* Name, const T& Resource, const char* Format)
{
	std::string Filename("rc.tmp.");
	Filename += Format;
	if (std::filesystem::exists(Filename))
		throw std::exception("writeResource(): temporary file exists");
	if (!Resource.saveToFile(Filename.c_str()))
		return;
	std::ifstream File(Filename.c_str(), std::ios_base::binary);
	if (!File)
		throw std::exception("writeResource(): temporary file inaccessible");
	unsigned int FileSize = static_cast<unsigned int>(std::filesystem::file_size(Filename));
	std::vector<char> Buffer(FileSize);
	if (!File.read(&Buffer[0], FileSize))
	{
		File.close();
		std::filesystem::remove(Filename.c_str());
		throw std::exception("writeResource(): failed to read from the temporary");
	}
	File.close();
	std::filesystem::remove(Filename.c_str());
	Sections.put(Name, Buffer);
}