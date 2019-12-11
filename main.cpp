#include "program.h"

#include <iostream>

#include "gameboard.h"
#include "localplayer.h"
#include "gamecontroller.h"
#include "sections.h"

void printField(const CGameBoard::CField& Field)
{
	for (int i = 0; i < Field.getHeight(); i++)
	{
		for (int j = 0; j < Field.getWidth(); j++)
		{
			int State = Field.at(j, i).getState();
			switch (State)
			{
			case CTile::CState::TAKEN:
				std::cout << "X ";
				break;
			case CTile::CState::HIT:
				std::cout << "* ";
				break;
			case CTile::CState::DESTROYED:
				std::cout << "o ";
				break;
			case CTile::CState::MISS:
				std::cout << ". ";
				break;
			default:
				std::cout << "/ ";
				break;
			}
		}
		std::cout << std::endl;
	}
}

void printField(const CShipLayout& Field)
{
	for (int i = 0; i < Field.getHeight(); i++)
	{
		for (int j = 0; j < Field.getWidth(); j++)
		{
			int State = Field.at(j, i);
			switch (State)
			{
			case 1:
				std::cout << "X ";
				break;
			default:
				std::cout << "O ";
				break;
			}
		}
		std::cout << std::endl;
	}
}

int main()
{
	/*std::ios_base::sync_with_stdio(false);
	sf::Texture a1, a2, a3;
	const char FontName[] = "Arial.ttf";
	a1.loadFromFile("normal.png");
	a2.loadFromFile("hover.png");
	a3.loadFromFile("pressed.png");
	CSections s;
	s.put("FONT_NAME", std::vector<char>(FontName, FontName + sizeof(FontName) - 1));
	CExtendedPreset::writeResource(s, "TXT_NORMAL", a1.copyToImage(), "png");
	CExtendedPreset::writeResource(s, "TXT_HOVER", a2.copyToImage(), "png");
	CExtendedPreset::writeResource(s, "TXT_PRESSED", a3.copyToImage(), "png");
	s.putInt<int32_t>("FONT_SIZE", 12);
	s.saveToFile("button.irc");*/
    CProgram Program;
    if(!Program.init())
    {
       std::cout << "startup failed, exitting" << std::endl;
       return -1;
    }
    Program.run();

    return 0;
}
