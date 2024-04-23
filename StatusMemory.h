#pragma once
#include"Mahjong.h"
#include<vector>
#include<map>


const int allTile = 136;
const int initialHand = 13;

//手上的，场上的，未出现的，面子

enum action {
    PASS,
    DRAW,
    PLAY,
    CHI,
    PENG,
    GANG,
    BUGANG,
    ANGANG,
    BUHUA,
    HU
};

action actionStrToEnum(string Action);

string actionEnumToStr(action Action);

class Memory {
private:
    int myPosition;//本家
    int lastPosition;//上家
    int nextPosition;//下家
    int oppoPosition;//对家

    int currPlayer;
    action currAction;
    action lastAction;//用于杠上开花
    Mahjong currPlayTile;//用于吃
    int currTurn;
    int Quan;//场风

    bool GangFlag;

    int Unplayed[70];//对应每张牌，未出现的牌，手牌不计入
    int totalTile;//不单独对应，所有未出现的牌
    map<Mahjong, int>cnt_hand;//未有初始化为0，手牌中某种牌的数量

    int handNum[4];//各自的手牌数
    vector<Mahjong>handTile;//手牌
    vector<Mahjong>Played;//所有打出的牌
    vector<Mahjong>eachPlayed[4];//各自打出的牌，不含吃碰杠花

    vector<pair<Mahjong,int>>Chi[4];//1=吃第一张
    vector<pair<Mahjong, int>>Peng[4];//2=对家，3=下家
    vector<pair<Mahjong, int>>Gang[4];//0=暗杠，1=上家明杠，5=碰上家后补杠

    int Hana[4];
    int currFan[4];//各自目前已有的番数，顺带计入补花数

    int tileWall[4];

    //暗杠
    int unknownTile;

public:
    Memory();
    void initial(int myPos, int quan);

    void buHua(int idx, int num);

    int getQuan();

    int getMyPosistion();

    int getLastPosition();
    int getNextPosition();
    int getoppoPosition();

    int getFormatPosition(int target);

    int getLastPosition(int idx);
    int getNextPosition(int idx);
    int getoppoPosition(int idx);

    int getFormatPosition(int own, int target);

    vector<Mahjong>& getHandTile();

    void setCurrPlayer(int idx);
    int getCurrPlayer();

    void setCurrAction(action Action);
    void setCurrAction(string Action);

    action getCurrAction();
    action getLastAction();

    Mahjong getCurrPlayTile();

    int getTargetTileLeft(int tileNum);
    int getTargetTileLeft(Mahjong majang);
    int getTargetTileLeft(string& majang);
    int getTargetTileLeft(string majang);

    //获取目标玩家打出/响应的牌
    vector<pair<Mahjong, int>>& getChi(int idx);
    vector<pair<Mahjong, int>>& getPeng(int idx);
    vector<pair<Mahjong, int>>& getGang(int idx);
    vector<Mahjong>& getEachPlayed(int idx);
    int getHana(int idx);

    int* getUnPlayed();
    //获取各玩家手牌信息
    int gethandNum(int player)
    {
        return handNum[player];
    }
    int gettileWall(int player)
    {
        return tileWall[player];
    }
    vector<Mahjong>& getPlayed()
    {
        for (int i = 0; i < 4; i++)
        {
            if (1)
            {
                vector<Mahjong>& oper = eachPlayed[i];
                for (int i = 0; i < oper.size(); i++)
                {
                    Played.push_back(oper[i]);
                }
            }
        }
        return Played;
    };

    //记录
    void playTile(Mahjong majang, action Action);//本家出牌，包括吃碰杠花,注明动作
    void playTile(Mahjong majang);//本家出牌，包括吃碰花

    void playTile(int idx, Mahjong majang, action Action);//他家出牌，包括吃碰杠花


    void drawTile(Mahjong majang);//本家抓牌

    void drawTile(int idx, int num = 1);//他家抓牌

    int getCntHand(Mahjong majang);

    void sortHand();

    string getFormatHandSting();

    int getTileWallNum(int idx);

    int getHandNum(int idx);

    bool getGangFlag();

    int getUnknownTile();
};

extern Memory memory;

extern int turn;


class Hand_Claim {
public:
    int myPosition;
    vector<Mahjong>handTile;//手牌

    vector<pair<Mahjong, int>>Chi[4];//1=吃第一张
    vector<pair<Mahjong, int>>Peng[4];//2=对家，3=下家
    vector<pair<Mahjong, int>>Gang[4];//0=暗杠，1=上家明杠，5=碰上家后补杠
    int Hana;

    Hand_Claim();
    Hand_Claim(Hand_Claim& other);

    //1.摸后 

    void addHand(Mahjong majang);

    void removeHand(Mahjong majang);

    void addChi(Mahjong majang, int target);
    void removeChi(Mahjong majang, int target);

    void addPeng(Mahjong majang, int target);
    int removePeng(Mahjong majang);

    void addAnGang(Mahjong majang);

    void addMinGang(Mahjong majang, int target);

    void addBuGang(Mahjong majang);

    void removeAnGang(Mahjong majang);
    void removeMinGang(Mahjong majang);
    void removeBuGang(Mahjong majang);

    string getFormatHandSting();

};