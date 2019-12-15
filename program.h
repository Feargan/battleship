#pragma once
#include "panel.h"
#include "button.h"
#include "textbox.h"
#include "gameui.h"

#include <SFML/Graphics.hpp>

#include <iostream>

class CProgram
{
    int m_WindowWidth;
    int m_WindowHeight;
    bool m_FullscreenMode;

    CPanel m_Panel;
    CTextboxResources m_TextboxResources;
	CTextbox* m_Textbox_Message;
	CExtendedPreset m_GamePreset;
public:
    CProgram(){}
    void loadConfig(const char* File);
    bool init();
    void run();
};
