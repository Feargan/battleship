#pragma once
#include "screencontext.h"
#include "extendedpreset.h"
#include "panel.h"
#include "interface_control.h"
#include "button.h"
#include "radiobutton.h"
#include "radiogroup.h"
#include "slider.h"
#include "txtview.h"
#include "fieldeditor.h"
#include "gameboard.h"

#include <windows.h>

class CDesignerUi : public IScreenContext, private IControl::IEventListener
{
	class CTxtEditor : private IControl::IEventListener
	{
		sf::Texture* m_Texture;
		CButton* m_ChangeButton;
	public:
		CTxtEditor(CPanel* Panel, sf::Vector2i Pos, sf::Vector2i TxtMaxRes, sf::Texture* Texture, const CButton::CResources& ButtonResources) : m_Texture(Texture)
		{
			m_ChangeButton = new CButton(Panel);
			m_ChangeButton->setResources(ButtonResources);
			m_ChangeButton->setPosition({ Pos.x, Pos.y+TxtMaxRes.y, 100, 30 }); //!
			m_ChangeButton->setTitle("Change");
			m_ChangeButton->addListener(this);

			CTxtView* View = new CTxtView(Panel);
			View->setTexture(Texture);
			View->setPosition({ Pos.x, Pos.y, 50, 50 }); //!!
		}

		virtual void onEvent(IControl* Control, int EventId) override
		{
			OPENFILENAME ofn;
			char fileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "Image files (*.png, *.jpeg, *.jpg, *.bmp, *.gif)\0*.png\0*.jpeg\0*.jpg\0*.bmp\0*.gif";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = "";
			ofn.lpstrInitialDir = "";

			std::string fileNameStr;
			if (GetOpenFileName(&ofn))
				fileNameStr = fileName;
			if (!m_Texture->loadFromFile(fileName))
				MessageBox(NULL, "", "", 0);
		}
	};
	
	std::unique_ptr<CTxtEditor> m_TxtEmptyEditor, m_TxtTakenEditor, m_TxtMissEditor, m_TxtHitEditor;

	class CSndEditor
	{
		sf::SoundBuffer* m_Sound;
		CButton* m_ChangeButton;
	public:
		
	};

	CExtendedPreset m_Preset;
	CButton::CResources m_ButtonResources;
	//CRadioButton::CResources m_RadioResources;
	CPanel m_FilePanel;
	CPanel m_BoardPanel;
	CPanel m_TemplatesPanel;
	CPanel m_SoundsPanel;
	CPanel m_Editor;
	CButton* m_LoadButton;
	CFieldEditor* m_FieldEditor;
	CRadioGroup m_BrushGroup;
	sf::Vector2i m_FieldPos;

	CSlider* m_FieldXSlider;
	CSlider* m_FieldYSlider;
public:
	CDesignerUi();
	virtual ~CDesignerUi();

	virtual void run() override;
	virtual void handleInput(sf::Event Event) override;

protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
private:
	virtual void onEvent(IControl* Control, int Id) override;
	/*
	template textures edition:
	in the same tab with field
	change, rotate, slidebars?: posx, posy, sizex, sizey, 
	checkbox: show on the field;
	grouped radiobutton?: alive, destroyed
	*/
};

