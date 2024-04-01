#include "RequestInfor.h"
#include"StatusMemory.h"

void request(istringstream& sin)
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

	sin >> tile;

	if (tile.size() == 1) {
		memory.buHua(stoi(tile));
	}
	else {
		memory.drawTile(Mahjong(tile));
	}

	while (sin >> tile) {
		memory.drawTile(Mahjong(tile));
	}
	
	for (int i = 0; i < 4; i++) {
		if (i != memory.getMyPosistion()) {
			memory.drawTile(i, 13);
		}
	}
}

void drawTile(istringstream& sin)
{
	string tile;
	sin >> tile;
	memory.drawTile(Mahjong(tile));
}

void complexInfor(istringstream& sin)
{
	int currPlayer; string currAction, currTile = "";
	sin >> currPlayer >> currAction >> currTile;

	memory.setCurrAction(PASS);
	if (currAction == "DRAW") {
		memory.drawTile(currPlayer);
	}
	else {
		Mahjong lastTile(memory.getCurrPlayTile());
		action Action = actionStrToEnum(currAction);
		memory.playTile(currPlayer, lastTile, Action);
		memory.playTile(currPlayer, Mahjong(currTile), PLAY);
	}
}
