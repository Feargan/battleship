#pragma once

#include "gameui.h"

#include <SFML/Graphics.hpp>

class CProgram
{
    int m_WindowWidth;
    int m_WindowHeight;
    bool m_FullscreenMode;

	CGamePreset m_GamePreset;
	CGameResources m_GameResources;
	CUiResources m_UiResources;
public:
    CProgram(){}
    bool init();
    void run();
};
