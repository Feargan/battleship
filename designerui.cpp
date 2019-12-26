#include "designerui.h"
#include "interfaceutils.h"
#include "tab.h"
#include "slider.h"

CDesignerUi::CDesignerUi()
{
	if (!m_Preset.load("preset.bsp")) // preset.bsp
		throw std::exception("failed to open template preset");

	m_ButtonResources.load("button.irc");

	CSlider::CResources SliderResources;
	SliderResources.load("slider.irc");
	/*SliderResources.m_TxtBar.loadFromFile("sl_bar.png");
	SliderResources.m_TxtPointer.loadFromFile("sl_pointer.png");
	CSections Sections;
	CExtendedPreset::writeResource(Sections, "TXT_BAR", SliderResources.m_TxtBar.copyToImage(), "png");
	CExtendedPreset::writeResource(Sections, "TXT_POINTER", SliderResources.m_TxtPointer.copyToImage(), "png");
	Sections.saveToFile("slider.irc");*/
	//m_RadioResources.load("button.irc");

	m_FieldPos = { 170, 50 };

	CButton* Button;
	Button = new CButton(&m_FilePanel);
	Button->setResources(m_ButtonResources);
	Button->setPosition({0, 0, 100, 60});
	Button->setTitle("New");
	Button->addListener(this);
	
	m_LoadButton = new CButton(&m_FilePanel);
	m_LoadButton->setResources(m_ButtonResources);
	m_LoadButton->setPosition({ 0, 70, 100, 60 });
	m_LoadButton->setTitle("Load");
	m_LoadButton->addListener(this);

	Button = new CButton(&m_FilePanel);
	Button->setResources(m_ButtonResources);
	Button->setPosition({ 0, 150, 100, 60 });
	Button->setTitle("Save");
	Button->addListener(this);

	Button = new CButton(&m_FilePanel);
	Button->setResources(m_ButtonResources);
	Button->setPosition({ 0, 230, 100, 60 });
	Button->setTitle("Save as");
	Button->addListener(this);

	m_FilePanel.autoSize();

	m_FieldXSlider = new CSlider(&m_TemplatesPanel);
	m_FieldXSlider->setResources(SliderResources);
	m_FieldXSlider->setPosition({ 170, 200, 100, 15 });
	m_FieldXSlider->setMin(1);
	m_FieldXSlider->setMax(5);
	m_FieldXSlider->setDiv(1);
	m_FieldXSlider->setValue(5);
	m_FieldXSlider->addListener(this);

	m_FieldYSlider = new CSlider(&m_TemplatesPanel);
	m_FieldYSlider->setResources(SliderResources);
	m_FieldYSlider->setPosition({ 170, 230, 100, 15 });
	m_FieldYSlider->setMin(1);
	m_FieldYSlider->setMax(5);
	m_FieldYSlider->setDiv(1);
	m_FieldYSlider->setValue(5);
	m_FieldYSlider->addListener(this);

	m_FieldEditor = new CFieldEditor(&m_TemplatesPanel, &m_Preset);
	m_FieldEditor->setPosition({ 170, 50, 0, 0 }); // fixxxxxxxxxx
	m_FieldEditor->resize(5, 5);

	m_TemplatesPanel.autoSize();

	/*Button = new CButton(&m_TemplatesPanel);
	Button->setResources(m_ButtonResources);
	Button->setPosition({ 170, 260, 100, 60 });
	Button->setTitle("dupa");
	Button->addListener(this);*/
	
	/*CRadioButton* Radio;
	Radio = new CRadioButton(&m_BrushPanel);
	Radio->setResources(m_ButtonResources);
	Radio->setPosition({ 0, 0, 100, 30 });
	Radio->setTitle("Draw");
	Radio->addListener(this);
	m_BrushGroup.add(Radio);
	Radio->switchOn();

	Radio = new CRadioButton(&m_BrushPanel);
	Radio->setResources(m_ButtonResources);
	Radio->setPosition({ 0, 40, 100, 30 });
	Radio->setTitle("Erase");
	Radio->addListener(this);
	m_BrushGroup.add(Radio);*/

	m_TxtEmptyEditor = std::unique_ptr<CTxtEditor>(new CTxtEditor(&m_BoardPanel, { 200, 10 }, { 40, 40 }, &m_Preset.getBasicAssets().m_TxtTileEmpty, m_ButtonResources));
	m_TxtTakenEditor = std::unique_ptr<CTxtEditor>(new CTxtEditor(&m_BoardPanel, { 200, 110 }, { 40, 40 }, &m_Preset.getBasicAssets().m_TxtTileTaken, m_ButtonResources));
	m_TxtMissEditor = std::unique_ptr<CTxtEditor>(new CTxtEditor(&m_BoardPanel, { 200, 210 }, { 40, 40 }, &m_Preset.getBasicAssets().m_TxtTileMiss, m_ButtonResources));
	m_TxtHitEditor = std::unique_ptr<CTxtEditor>(new CTxtEditor(&m_BoardPanel, { 200, 310 }, { 40, 40 }, &m_Preset.getBasicAssets().m_TxtTileHit, m_ButtonResources));
	m_BoardPanel.autoSize();

	

	CTab* Tab = new CTab(&m_Editor);
	Tab->setResources(m_ButtonResources);
	Tab->setPosition({ 10, 10, 300, 200 });
	Tab->link("File", &m_FilePanel);
	Tab->link("Board", &m_BoardPanel);
	Tab->link("Templates", &m_TemplatesPanel);
	Tab->link("Sounds", &m_SoundsPanel);

	//m_Editor.autoSize();
	m_Editor.setArea({ 0, 0, 800, 600 });
}


CDesignerUi::~CDesignerUi()
{
}

void CDesignerUi::run()
{
	//m_BrushPanel.update();
	//m_FilePanel.update();
	m_Editor.update();
}

void CDesignerUi::handleInput(sf::Event Event)
{
	//m_FilePanel.handleInput(Event);
	//m_BrushPanel.handleInput(Event);
	m_Editor.handleInput(Event);

}

void CDesignerUi::draw(sf::RenderTarget & Target, sf::RenderStates States) const
{
	//Target.draw(m_BrushPanel, States);
	//Target.draw(m_FilePanel, States);
	Target.draw(m_Editor, States);
}

void CDesignerUi::onEvent(IControl * Control, int Id)
{
	if (Control == m_FieldXSlider && Id == CSlider::CEvent::VALUE_CHANGED)
	{
		m_FieldEditor->resize(static_cast<CSlider*>(m_FieldXSlider)->getValue(), m_FieldEditor->getField().getHeight());
	}
	else if (Control == m_FieldYSlider && Id == CSlider::CEvent::VALUE_CHANGED)
	{
		m_FieldEditor->resize(m_FieldEditor->getField().getWidth(), static_cast<CSlider*>(m_FieldYSlider)->getValue());
	}
	else if (Control == m_LoadButton && Id == CButton::CEvent::PRESSED)
	{
		OPENFILENAME ofn;
		char FileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "Preset files (*.bsp)\0*.bsp";
		ofn.lpstrFile = FileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "";
		ofn.lpstrInitialDir = "";

		if (GetOpenFileName(&ofn))
		{
			m_Preset.load(FileName);
		}
	}
}