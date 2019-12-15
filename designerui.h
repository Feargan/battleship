#pragma once
#include "screencontext.h"
#include "extendedpreset.h"
#include "panel.h"
#include "interface_control.h"
#include "button.h"
#include "radiobutton.h"
#include "radiogroup.h"
#include "gameboard.h"

class CDesignerUi : public IScreenContext, private IControl::IEventListener
{
	CExtendedPreset m_Preset;
	CButton::CResources m_ButtonResources;
	//CRadioButton::CResources m_RadioResources;
	CPanel m_Buttons;
	CPanel m_BrushPanel;
	CPanel m_Editor;
	CRadioGroup m_BrushGroup;
	CGameBoard::CField m_EditedField;
	sf::Vector2i m_FieldPos;
	std::optional<sf::Vector2i> m_CurrentTile;
public:
	CDesignerUi();
	virtual ~CDesignerUi();

	virtual void run() override;
	virtual void handleInput(sf::Event Event) override;

	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
private:
	virtual void onEvent(IControl* Control, int Id) override;

	void makeTxtEditPanel(CPanel& Panel, sf::Texture* EditedTxt);
	void makeSndEditPanel(CPanel& Panel, sf::SoundBuffer* EditedSnd);
	/*
	template textures edition:
	in the same tab with field
	change, rotate, slidebars?: posx, posy, sizex, sizey, 
	checkbox: show on the field;
	grouped radiobutton?: alive, destroyed
	*/
};

