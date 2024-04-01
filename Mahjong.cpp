#include "Mahjong.h"

Mahjong::Mahjong():tile(0)
{
}

Mahjong::Mahjong(const string& s)
{
	int num = s[1] - '0';
	tile=10;
	switch (s[0])
	{
	case 'W': {
		tile *= W; break;
	}
	case 'B': {
		tile *= B; break;
	}
	case 'T': {
		tile *= T; break;
	}
	case 'F': {
		tile *= F; break;
	}
	case 'J': {
		tile *= J; break;
	}
	case 'H': {
		tile *= H; break;
	}
	default:
		break;
	}

	tile += num;
}

Mahjong::Mahjong(const Mahjong& other):tile(other.tile){}

Mahjong::Mahjong(int tileNum):tile(tileNum){}

char Mahjong::getTypeChar()
{
	int type = tile / 10;

	switch (type)
	{
	case W: return 'W';
	case B: return 'B';
	case T: return 'T';
	case F: return 'F';
	case J: return 'J';
	case H: return 'H';
	default:return 0;
	}

}

int Mahjong::getTypeInt()
{
	return tile / 10;
}

int Mahjong::getNum()
{
	return tile%10;
}

string Mahjong::getTileString()
{
	string tileStr = "";
	tileStr += getTypeChar();
	tileStr += '0' + getNum();

	return tileStr;
}

bool Mahjong::isHana()
{
	int type = getTypeInt();

	if (type == H)return true;
	return false;
}

bool Mahjong::isJian()
{
	int type = getTypeInt();

	if (type == J)return true;
	return false;
}

Mahjong Mahjong::getNext()
{
	return Mahjong(tile + 1);
}

Mahjong Mahjong::getLast()
{
	return Mahjong(tile - 1);
}

Mahjong& Mahjong::operator=(const Mahjong& other)
{
	if (this != &other) {
		this->tile = other.tile;
	}

	return *this;
}

bool Mahjong::operator==(const Mahjong& other)
{
	return this->tile == other.tile;
}

bool Mahjong::operator!=(const Mahjong& other)
{
	return this->tile != other.tile;
}

bool Mahjong::operator<(const Mahjong& other)const
{
	return this->tile < other.tile;
}

bool Mahjong::operator>(const Mahjong& other)const
{
	return this->tile > other.tile;
}

Mahjong::operator int()const
{
	return tile;
}

Mahjong::operator string ()
{
	return getTileString();
}

