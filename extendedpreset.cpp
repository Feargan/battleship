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

bool CExtendedPreset::load(const char * Filename)
{
	CSections Sections;
	if (Sections.loadFromFile(Filename) != CSections::CState::LOAD_OK)
		return false;
	
	sf::Image Img;
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
	m_Assets.m_TxtTileUnrevealed.loadFromFile("tile_unrevealed.png");
	m_Assets.m_TxtTileMiss.loadFromFile("tile_miss.png");
	m_Assets.m_SndHit.loadFromFile("boom.wav");
	m_Assets.m_SndVictory.loadFromFile("victory.ogg");
	m_Assets.m_TileSize = { 20, 20 };

	
	return true;
}

bool CExtendedPreset::save(const char * Filename)
{
	return false;
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
	LayoutSections.putInt("W", Width);
	LayoutSections.putInt("H", Height);
	for (int i = 0; i < Height; i++)
		for (int j = 0; j < Width; j++)
		{
			LayoutBuffer[i*Width + j] = Layout[{j, i}];
		}
	LayoutSections.put("L", LayoutBuffer);
	std::vector<char> SectionBuffer;
	LayoutSections.saveToBuffer(SectionBuffer);
	Sections.put(Name, SectionBuffer);
}
