#pragma once
#include<string>
#include"tile.h"

using namespace std;

using namespace mahjong;

enum tileType {
    Empty,
    W,//万
    B,//筒
    T,//条
    F,//风
    J,//箭
    H//花
};

class Mahjong {
private:
    int tile;
    string FormatStr;

public:
    Mahjong();
    Mahjong(const string& s);
    Mahjong(const Mahjong& other);
    Mahjong(int tileNum);
    Mahjong(const string s,int idx);

    char getTypeChar();
    string getTypeStr();
    int getTypeInt();
    int getNum();
    string getTileString();
    int getTile() { return tile; }

    bool isHana();//花？
    bool isJian();//箭？
    bool isFeng();//风？
    bool isNum();

    //用于吃碰
    Mahjong getNext();
    Mahjong getLast();

    Mahjong& operator=(const Mahjong& other);
    bool operator==(const Mahjong& other);
    bool operator!=(const Mahjong& other);
    bool operator<(const Mahjong& other)const;
    bool operator>(const Mahjong& other)const;
    operator int()const;
    operator string();

    void setFortamtStr();
    string getFormatStr();

    tile_t MahjongTotile();
};


class cmp {
public:
    bool operator()(const Mahjong& a, const Mahjong& b)const {
        return a < b;
    }
};
