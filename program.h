#ifndef PROGRAM_H
#define PROGRAM_H

#include "interface.h"
#include "controls/button.h"
#include "controls/textbox.h"
#include "gameui.h"

#include <SFML/Graphics.hpp>

#include <iostream>

class CProgram
{
    int m_WindowWidth;
    int m_WindowHeight;
    bool m_FullscreenMode;

    CInterface m_Interface;
    CButtonResources m_ButtonResources;
    CTextboxResources m_TextboxResources;
	CTextbox* m_Textbox_Message;

	CGameUi m_GameUi;
	CExtendedPreset m_GamePreset;
public:
    CProgram(){}
    void loadConfig(const char* File);
    bool init();
    void run();
};

#endif // PROGRAM_H
