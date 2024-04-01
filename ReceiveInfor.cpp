#include "ReceiveInfor.h"
#include"StatusMemory.h"

void receive(istringstream& sin)
{
    int inforType;
    sin >> inforType;

	switch (inforType)
	{
	case 0:memoryInitial(sin); break;
	case 1:handInitial(sin); break;
	case 2:drawTile(sin); break;
	case 3:complexInfor(sin); break;
	default:
		break;
	}
}

void memoryInitial(istringstream& sin)
{
	int plaerID, quan;

	sin >> plaerID >> quan;

	memory.initial(plaerID, quan);
}

void handInitial(istringstream& sin)
{
	string tile;

	//test
	while (sin >> tile) {
		memory.drawTile(Mahjong(tile));
	}

	///
}

void drawTile(istringstream& sin)
{
}

void complexInfor(istringstream& sin)
{
}
