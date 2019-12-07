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
	std::ios_base::sync_with_stdio(false);
	/*CGameBoard b = CGameBoard();
	CGameBoard a;
	a = b;
    CProgram Program;
    if(!Program.init())
    {
       std::cout << "startup failed, exitting" << std::endl;
       return -1;
    }
    Program.run();*/
	CSections Sections;
	CShipLayout Layout(3, 6);
	Layout.fill(0);
	Layout[{0, 0}] = 1; Layout[{1, 1}] = 1; Layout[{2, 5}] = 1; Layout[{2, 2}] = 1;
	CExtendedPreset::writeLayout(Sections, "L_1", Layout);
	Sections.putInt("X", -4);
	Sections.saveToFile("preset.bsp");

	CSections Load;
	Load.loadFromFile("preset.bsp");
	auto Layout2 = CExtendedPreset::readLayout(Load, "L_1");
	if (!Layout2)
	{
		std::cout << "error" << std::endl;
		return 0;
	}
	printField(*Layout2);
	std::cout << *Sections.getInt<int>("X");

    return 0;
}
