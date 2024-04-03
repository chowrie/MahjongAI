#pragma once

#include<string>

using namespace std;


enum tileType {
    Empty,
    W,//��
    B,//Ͳ
    T,//��
    F,//��
    J,//��
    H//��
};

class Mahjong {
private:
    int tile;

public:
    Mahjong();
    Mahjong(const string& s);
    Mahjong(const Mahjong& other);
    Mahjong(int tileNum);

    char getTypeChar();
    int getTypeInt();
    int getNum();
    string getTileString();

    bool isHana();//����
    bool isJian();//����
    bool isFeng();//�磿
    bool isNum();

    //���ڳ���
    Mahjong getNext();
    Mahjong getLast();

    Mahjong& operator=(const Mahjong& other);
    bool operator==(const Mahjong& other);
    bool operator!=(const Mahjong& other);
    bool operator<(const Mahjong& other)const;
    bool operator>(const Mahjong& other)const;
    operator int()const;
    operator string();
};


class cmp {
public:
    bool operator()(const Mahjong& a, const Mahjong& b)const {
        return a < b;
    }
};