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
	case 2: {
		drawTile(sin); break;
	}
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

	int HanaNum = 0;

	for (int i = 0; i < 4; i++) {
		sin >> HanaNum;
		memory.buHua(i, HanaNum);
	}

	while (sin >> tile) {
		memory.drawTile(Mahjong(tile));
	}
	
	for (int i = 0; i < 4; i++) {
		if (i != memory.getMyPosistion()) {
			memory.drawTile(i, 13);
		}
	}
	memory.setCurrPlayer(0);
}

void drawTile(istringstream& sin)
{
	string tile;
	sin >> tile;
	memory.drawTile(Mahjong(tile));
}

void complexInfor(istringstream& sin)
{

    int currPlayer; string currAction = "", card1 = "", card2 = "";
    sin >> currPlayer >> currAction >> card1 >> card2;

	action Action = actionStrToEnum(currAction);

	if (currPlayer == memory.getMyPosistion()) {
		switch (Action)
		{
		case PLAY: {//3 playerID DRAW
			memory.playTile(Mahjong(card1), PLAY);
			break;
		}
		case CHI: {//3 playerID CHI Card1 Card2
			memory.playTile(Mahjong(card1), CHI);
			memory.playTile(Mahjong(card2), PLAY);
			break;
		}
		case PENG: {//3 playerID PENG Card1
			memory.playTile(Mahjong(card1), PENG);
			break;
		}
		case GANG: {
			if (memory.getCurrAction() == DRAW) {
				memory.playTile(Mahjong(card1), ANGANG);
			}
			else {
				memory.playTile(Mahjong(card1), GANG);
			}
			break;
		}
		case BUGANG: {//3 playerID BUGANG Card1
			memory.playTile(Mahjong(card1), BUGANG);
			break;
		}
		case BUHUA: {//3 playerID BUHUA Card1 
			memory.playTile(Mahjong(card1), BUHUA);
			break;
		}
		case HU:
			break;
		default:
			break;
		}

	}
	else {
		switch (Action)
		{
		case DRAW: {//3 playerID DRAW
			memory.drawTile(currPlayer);
			break;
		}
		case PLAY: {//3 playerID DRAW
			memory.playTile(currPlayer, Mahjong(card1), PLAY);
			break;
		}
		case CHI: {//3 playerID CHI Card1 Card2
			memory.playTile(currPlayer, Mahjong(card1), CHI);
			memory.playTile(currPlayer, Mahjong(card2), PLAY);
			break;
		}
		case PENG: {//3 playerID PENG Card1
			memory.playTile(currPlayer, memory.getCurrPlayTile(), PENG);
			memory.playTile(currPlayer, Mahjong(card1), PLAY);
			break;
		}
		case GANG: {
			if (memory.getCurrAction() == DRAW) {
				memory.playTile(currPlayer, Mahjong(card1), ANGANG);
			}
			else {
				memory.playTile(currPlayer, Mahjong(card1), GANG);
			}
			break;
		}
		case BUGANG: {//3 playerID BUGANG Card1
			memory.playTile(currPlayer, Mahjong(card1), BUGANG);
			break;
		}
		case BUHUA: {//3 playerID BUHUA Card1 
			memory.playTile(currPlayer, Mahjong(card1), BUHUA);
			break;
		}
		case HU:
			break;
		default:
			break;
		}

	}

}
