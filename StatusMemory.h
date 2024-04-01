#pragma once

#include"Mahjong.h"
#include<vector>


const int allTile = 144;
const int initialHand = 13;

//���ϵģ����ϵģ�δ���ֵģ�����

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
    int myPosition;//����
    int lastPosition;//�ϼ�
    int nextPosition;//�¼�
    int currPlayer;
    action currAction;
    Mahjong currPlayTile;//���ڳԡ�����
    int currTurn;
    int Quan;//����

    int Unplayed[70];//��Ӧÿ���ƣ�δ���ֵ��ƣ����Ʋ�����
    int totalTile;//��������Ӧ������δ���ֵ���

    int handNum[4];//���Ե�������
    vector<Mahjong>handTile;//����
    vector<Mahjong>Played;//���д������
    vector<Mahjong>eachPlayed[4];//���Դ�����ƣ����������ܻ�

    vector<Mahjong>Chi[4];
    vector<Mahjong>Peng[4];
    vector<Mahjong>Gang[4];
    vector<Mahjong>anGang[4];
    int Hana[4];
    int currFan[4];//����Ŀǰ���еķ�����˳�����벹����

public:
    Memory();
    void initial(int myPos, int quan);

    void buHua(int num);

    int getQuan();

    int getMyPosistion();
    int getLastPosition();
    int getNextPosition();

    void setCurrPlayer(int idx);
    int getCurrPlayer();

    void setCurrAction(action Action);
    void setCurrAction(string Action);
    string getCurrAction();
    Mahjong getCurrPlayTile();

    int getTargetTileLeft(int tileNum);
    int getTargetTileLeft(const Mahjong& majang);
    int getTargetTileLeft(const Mahjong majang);
    int getTargetTileLeft(string& majang);
    int getTargetTileLeft(string majang);

    //��ȡĿ����Ҵ��/��Ӧ����
    const vector<Mahjong>& getChi(int idx);
    const vector<Mahjong>& getPeng(int idx);
    const vector<Mahjong>& getGang(int idx);
    const vector<Mahjong>& getanGang(int idx);
    const vector<Mahjong>& getEachPlayed(int idx);


    //��¼
    void playTile(Mahjong majang, action Action);//���ҳ��ƣ����������ܻ�,ע������
    void playTile(Mahjong majang);//���ҳ��ƣ�����������

    void playTile(int idx, Mahjong majang, action Action);//���ҳ��ƣ����������ܻ�


    void drawTile(Mahjong majang);//����ץ��
    void drawTile(int idx, int num = 1);//����ץ��
};

extern Memory memory;

extern int turn;