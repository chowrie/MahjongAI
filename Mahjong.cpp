#pragma once
#include <cctype>
#include "Mahjong.h"
Mahjong::Mahjong():tile(0)
{
	setFortamtStr();
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
	setFortamtStr();
}

Mahjong::Mahjong(const Mahjong& other):tile(other.tile){

	setFortamtStr();
}

Mahjong::Mahjong(int tileNum):tile(tileNum){

	setFortamtStr();
}

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

string Mahjong::getTypeStr()
{
	int type = tile / 10;

	switch (type)
	{
	case W: return "m";
	case B: return "p";
	case T: return "s";
	default:return "";
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
	return getTypeInt() == H;
}

bool Mahjong::isJian()
{
	return getTypeInt() == J;
}

bool Mahjong::isFeng()
{
	return getTypeInt() == F;
}

bool Mahjong::isNum()
{
	return tile>=11&&tile<=39;
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
		this->FormatStr = other.FormatStr;
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

Mahjong::operator string()
{
	return getTileString();
}

void Mahjong::setFortamtStr()
{
	if (isNum()) {
		FormatStr = to_string(getNum()) + getTypeStr();
	}
	else {
		switch (tile) {
		case 41: {
			FormatStr = "E";
			break;
		}
		case 42: {
			FormatStr = "S";
			break;
		}
		case 43: {
			FormatStr = "W";
			break;
		}
		case 44: {
			FormatStr = "N";
			break;
		}
		case 51: {
			FormatStr = "C";
			break;
		}
		case 52: {
			FormatStr = "F";
			break;
		}
		case 53: {
			FormatStr = "P";
			break;
		}
		}
	}
}

string Mahjong::getFormatStr()
{
	return FormatStr;
}

tile_t Mahjong::MahjongTotile()
{
	int h = getTypeInt(), d = getNum();
	tile_t su = (uint8_t)h, ra = (uint8_t)(d);
	return make_tile(su, ra);
}


