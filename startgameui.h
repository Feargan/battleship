#pragma once

//#include "entryship.h"
#include "extendedpreset.h"
//#include "radiogroup.h"
#include "slider.h"
#include "text.h"
#include "gameboard.h"
#include "button.h"
#include "screencontext.h"
#include "localplayer.h"
#include "aiplayer.h"
#include "gamecontroller.h"
#include "gameui.h"

#include <vector>
#include <memory>
#include <optional>

class CStartGameUi : public IScreenContext, private IControl::IEventListener // IScreenContext
{
	/*class CShipSlider : private IControl::IEventListener
	{
		CGamePreset* m_Preset;
		CSlider* m_Slider;
		CText* m_Text;
		int m_Size;

		virtual void onEvent(IControl* Control, int EventId)
		{
			if (Control == m_Slider && EventId == CSlider::CEvent::VALUE_CHANGED)
			{
				m_Preset->setShipAmount(m_Size, m_Slider->getValue());
				m_Text->setText(std::to_string(m_Preset->getShipAmount(m_Size)).c_str());
			}
		}
	public:
		//CShipSlider() {}
		CShipSlider(CPanel* Panel, sf::Vector2i Pos, const CSlider::CResources& Resources, const sf::Font& Font, CGamePreset* Preset, int Size) : m_Preset(Preset), m_Size(Size)
		{
			CSlider* Slider = new CSlider(Panel);
			Slider->setResources(Resources);
			Slider->setPosition({ Pos.x, Pos.y, 100, 15 });
			Slider->setMin(0);
			Slider->setMax(4);
			Slider->setDiv(1);
			Slider->setValue(m_Preset->getShipAmount(m_Size));
			Slider->addListener(this);
			m_Slider = Slider;

			CText* Text = new CText(Panel);
			sf::Text Properties(std::to_string(m_Preset->getShipAmount(m_Size)).c_str(), Font, 12);
			Text->setPosition({ Pos.x + 110, Pos.y, 50, 15 });
			Text->setProperties(Properties);
			m_Text = Text;
		}
	};*/
	using CPos = sf::Vector2i;
	CPanel m_Panel;
	CButton::CResources m_ButtonResources;
	CSlider::CResources m_SliderResources;
	CText* m_ChanceText;
	CText* m_ErrorText;
	CButton* m_StartButton;
	CButton* m_AutoButton;
	CButton* m_ClearButton;
	std::unique_ptr<CGameUi> m_GameUi;
	struct CShipSlider
	{
		CSlider* m_Slider;
		CText* m_Amount;
		CText* m_Remaining;
	};
	std::vector<CShipSlider> m_ShipSliders;
	CPos m_BoardPos;
	std::optional<CPos> m_CurrentTile;

	CGameBoard m_Board;
	CLocalPlayer m_Player;
	CAiPlayer m_AiPlayer;
	IGameController m_Controller;
	CExtendedPreset* m_Preset;
public:
	CStartGameUi(CExtendedPreset* Preset);
	~CStartGameUi();

	virtual void handleInput(sf::Event Event) override;
	virtual void run() override;
protected:
	virtual void draw(sf::RenderTarget& Target, sf::RenderStates states) const override;
private:
	virtual void onEvent(IControl* Control, int EventId) override;

	void updateRemaining();
	void updateChance();
};

