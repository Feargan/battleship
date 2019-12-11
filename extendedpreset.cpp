#include "extendedpreset.h"
#include "sections.h"

#include <SFML/Graphics.hpp>

CExtendedPreset::CExtendedPreset()
	: CGamePreset()
{
}


CExtendedPreset::~CExtendedPreset()
{
}

#include <iostream>
#include <filesystem>

CExtendedPreset::CState CExtendedPreset::load(const char * Filename)
{
	CState PresetState = CState::OK;
	CSections Sections;
	if (Sections.loadFromFile(Filename) != CSections::CState::LOAD_OK)
		return CState::ERROR;
	// setIfPresent()
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
		return CState::ERROR;
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
	const std::vector<char>* Buffer;
	unsigned int i = 0;
	while ( (Buffer = Sections.get(std::string("TMPL_SHIP_" + std::to_string(i)).c_str())) )
	{
		CSections TemplateSections;
		TemplateSections.loadFromBuffer(*Buffer);
		auto Layout = readLayout(TemplateSections, "L");
		auto ShipWidth = TemplateSections.getInt<int32_t>("W");
		auto ShipHeight = TemplateSections.getInt<int32_t>("H");
		auto ShipAmount = TemplateSections.getInt<int32_t>("N");
		auto ShipAlive = TemplateSections.get("TXT_SHIP_ALIVE");
		auto ShipDestroyed = TemplateSections.get("TXT_SHIP_DESTROYED");
		if (!Layout || !ShipAmount)
			break;
		sf::Texture TxtShipAlive, TxtShipDestroyed;
		if (ShipAlive)
			TxtShipAlive.loadFromMemory(&(*ShipAlive)[0], ShipAlive->size());
		if(ShipDestroyed)
			TxtShipDestroyed.loadFromMemory(&(*ShipDestroyed)[0], ShipDestroyed->size());
		CGamePreset::putTemplate(*Layout, std::shared_ptr<CShipAsset>(new CShipAsset{ TxtShipAlive, TxtShipDestroyed }), *ShipAmount);
		i++;
	}
	/*sf::Image Img;
	sf::Texture ShipDestroyedTxt, ShipAliveTxt;

	setBoardSize({ 10, 10 });

	Img.loadFromFile("ship0_destroyed.png");
	Img.createMaskFromColor(sf::Color::White);
	ShipDestroyedTxt.loadFromImage(Img);
	Img.loadFromFile("ship0_alive.png");
	Img.createMaskFromColor(sf::Color::White);
	ShipAliveTxt.loadFromImage(Img);
	CShipLayout Layout = CShipLayout(3, 1);
	Layout.fill(1);
	CGamePreset::putTemplate(Layout, std::shared_ptr<CShipAsset>(new CShipAsset{ ShipAliveTxt, ShipDestroyedTxt }), 2);

	Img.loadFromFile("ship1_destroyed.png");
	Img.createMaskFromColor(sf::Color::White);
	ShipDestroyedTxt.loadFromImage(Img);
	Img.loadFromFile("ship1_alive.png");
	Img.createMaskFromColor(sf::Color::White);
	ShipAliveTxt.loadFromImage(Img);
	Layout = CShipLayout(2, 2);
	Layout.fill(1);
	CGamePreset::putTemplate(Layout, std::shared_ptr<CShipAsset>(new CShipAsset{ ShipAliveTxt, ShipDestroyedTxt }), 1);

	Img.loadFromFile("ship2_destroyed.png");
	Img.createMaskFromColor(sf::Color::White);
	ShipDestroyedTxt.loadFromImage(Img);
	Img.loadFromFile("ship2_alive.png");
	Img.createMaskFromColor(sf::Color::White);
	ShipAliveTxt.loadFromImage(Img);
	Layout = CShipLayout(5, 1);
	Layout.fill(1);
	CGamePreset::putTemplate(Layout, std::shared_ptr<CShipAsset>(new CShipAsset{ ShipAliveTxt, ShipDestroyedTxt }), 1);

	Layout.rotate(CRotationDir::CLOCKWISE);
	CGamePreset::putTemplate(Layout, std::shared_ptr<CShipAsset>(new CShipAsset{ ShipAliveTxt, ShipDestroyedTxt }), 1);

	m_Assets.m_TxtTileEmpty.loadFromFile("tile_empty.png");
	m_Assets.m_TxtTileTaken.loadFromFile("tile_taken.png");
	m_Assets.m_TxtTileHit.loadFromFile("tile_hit.png");
	m_Assets.m_TxtTileMiss.loadFromFile("tile_miss.png");
	m_Assets.m_SndMiss.loadFromFile("miss.ogg");
	m_Assets.m_SndHit.loadFromFile("boom.wav");
	m_Assets.m_SndVictory.loadFromFile("victory.ogg");
	m_Assets.m_TileSize = { 20, 20 };*/

	
	return PresetState;
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
		for (unsigned int i = 0; i < numTemplates(); i++)
		{
			CSections TemplateSections;
			const CShipTemplate* Template = getTemplate(i);
			TemplateSections.putInt<int32_t>("N", getShipAmount(i));
			writeLayout(TemplateSections, "L", Template->getLayout());
			const CShipAsset* Asset = getShipAsset(i);
			TemplateSections.putInt<int32_t>("W", Asset->m_Size.x);
			TemplateSections.putInt<int32_t>("H", Asset->m_Size.y);
			writeResource(TemplateSections, "TXT_SHIP_ALIVE", Asset->m_TxtAlive.copyToImage(), "png");
			writeResource(TemplateSections, "TXT_SHIP_DESTROYED", Asset->m_TxtDestroyed.copyToImage(), "png");
			writeSubsections(Sections, std::string("TMPL_SHIP_" + std::to_string(i)).c_str(), TemplateSections);
		}
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

void CExtendedPreset::putTemplate(const CShipLayout& Layout, const std::shared_ptr<CShipAsset>& Asset, int Amount)
{
	CGamePreset::putTemplate(Layout, std::static_pointer_cast<void>(Asset), Amount);
}

const CExtendedPreset::CShipAsset* CExtendedPreset::getShipAsset(unsigned int TemplateId) const
{
	if (TemplateId >= numTemplates())
		return nullptr;
	return static_cast<CShipAsset*>(getTemplate(TemplateId)->getExternal().get());
}

const CExtendedPreset::CBasicAssets & CExtendedPreset::getBasicAssets() const
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

std::optional<sf::Vector2i> CExtendedPreset::getTilePos(sf::Vector2i BoardSize, sf::Vector2i TilePxSize, sf::Vector2i BoardPos, sf::Vector2i CursorPos)
{
	if (CursorPos.x > BoardPos.x && CursorPos.y > BoardPos.y &
		CursorPos.x < BoardPos.x + TilePxSize.x * BoardSize.x && CursorPos.y < BoardPos.y + TilePxSize.y * BoardSize.y)
	{
		CursorPos -= BoardPos;
		CursorPos.x /= TilePxSize.x;
		CursorPos.y /= TilePxSize.y;
		return CursorPos;
	}
	return {};
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

/*std::optional<sf::Image> CExtendedPreset::readImage(CSections & Sections, const char * Name)
{
	auto Vec = Sections.get(Name);
	if (!Vec)
		return {};
	CSections ImageSections;
	ImageSections.loadFromBuffer(*Vec);
	auto Width = ImageSections.getInt<int32_t>("W");
	auto Height = ImageSections.getInt<int32_t>("H");
	auto LayoutBuffer = ImageSections.get("P");
	if (!Width || !Height || !LayoutBuffer || LayoutBuffer->size() < static_cast<unsigned int>(*Height * (*Width)))
		return {};
	sf::Image Image;
	Image.create(*Width, *Height, reinterpret_cast<const sf::Uint8*>(&(*LayoutBuffer)[0]));
	return Image;
}

void CExtendedPreset::writeImage(CSections & Sections, const char * Name, const sf::Image & Image)
{
	CSections ImageSections;
	int Width = Image.getSize().x, Height = Image.getSize().y;
	ImageSections.putInt<int32_t>("W", Width);
	ImageSections.putInt<int32_t>("H", Height);
	const char* PixelsBuffer = reinterpret_cast<const char*>(Image.getPixelsPtr());
	std::vector<char> Pixels(PixelsBuffer, PixelsBuffer + Width*Height);
	ImageSections.put("P", Pixels);
	writeSubsections(Sections, Name, ImageSections);
}

std::optional<sf::SoundBuffer> CExtendedPreset::readSound(CSections & Sections, const char * Name)
{
	auto Vec = Sections.get(Name);
	if (!Vec)
		return {};
	CSections SoundSections;
	SoundSections.loadFromBuffer(*Vec);
	auto NumSamples = SoundSections.getInt<sf::Uint64>("N");
	auto SamplesBuffer = SoundSections.get("S");
	if (!NumSamples || *NumSamples > 0xffffffff || !SamplesBuffer || SamplesBuffer->size() < *NumSamples*2)
		return {};
	return Image;
}

void CExtendedPreset::writeSound(CSections & Sections, const char * Name, const sf::SoundBuffer Sound)
{
	CSections SoundSections;
	sf::Uint64 NumSamples = Sound.getSampleCount();
	SoundSections.putInt("N", NumSamples);
	if (NumSamples > 0xffffffff)
		throw std::out_of_range("writeSound(): too many samples");
	std::vector<char> Samples(NumSamples * 2);
	const sf::Int16* SamplesBuffer = Sound.getSamples();
	for (sf::Uint64 i = 0; i < NumSamples; i++)
	{
		Samples[2 * i] = static_cast<char>(static_cast<sf::Int16>(SamplesBuffer[i] & 0xff));
		Samples[2 * i + 1] = static_cast<char>(static_cast<sf::Int16>((SamplesBuffer[i] >> 7) & 0xff));
	}
	SoundSections.put("S", Samples);
	writeSubsections(Sections, Name, SoundSections);
}*/
