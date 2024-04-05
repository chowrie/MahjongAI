#include "ResponseInfor.h"
#include"StatusMemory.h"
#include<algorithm>
#include<iostream>
string response()
{
    Mahjong currPlayTile = memory.getCurrPlayTile();

    if (turn == 0 || turn == 1)return "PASS";

    string responseStr = "";

    bool flag = false;

    if (memory.getCurrPlayer() == memory.getMyPosistion()) {

        //BUGANG Card1��������Card1��
        if (canBuGang()) {
            responseStr = "BUGANG ";
            memory.playTile(currPlayTile, BUGANG);

            responseStr += currPlayTile.getTileString();

            return responseStr;
        }
        //GANG Card1��������Card1��
        else if (canAnGang()) {
            responseStr = "GANG ";
            memory.playTile(currPlayTile, ANGANG);
            
            responseStr += currPlayTile.getTileString();

            return responseStr;
        }
        //PLAY Card1��������Card1��
        else {//����
            responseStr = "PLAY ";

            //ѡ��Ҫ�����
        }
    }
    else {

        //GANG
        if (canMinGang()) {
            responseStr = "GANG ";
            memory.playTile(currPlayTile, GANG);

            return responseStr;
        }
        //PENG Card1����Card1��
        else if (canPeng()) {
            responseStr = "PENG ";
            memory.playTile(currPlayTile, PENG);
        }
        //CHI Card1 Card2����Card1��Card2��
        else if (canChi()) {
            responseStr = "CHI ";
            Mahjong chiTarget;
            switch (canChi()) {
            case 1: {
                chiTarget = currPlayTile.getNext();
                break;
            }
            case 2: {
                chiTarget = currPlayTile;
                break;
            }
            case 3: {
                chiTarget = currPlayTile.getLast();
                break;
            }
            }
            responseStr += chiTarget.getTileString() + " ";
            memory.playTile(chiTarget, CHI);
        }
    }

    //ѡ������
    responseStr += memory.getHandTile()[0].getTileString();

    return responseStr;
}

int canChi()
{
    //���õ���19�߽������0�����ƣ�Ĭ�ϲ�����
    if (memory.getCurrPlayTile().isNum() ) {
        if (memory.getCntHand(memory.getCurrPlayTile().getNext()) &&
            memory.getCntHand(memory.getCurrPlayTile().getNext().getNext())
            )return 1;
        if (memory.getCntHand(memory.getCurrPlayTile().getLast()) &&
            memory.getCntHand(memory.getCurrPlayTile().getNext())
            )return 2;
        if (memory.getCntHand(memory.getCurrPlayTile().getLast()) &&
            memory.getCntHand(memory.getCurrPlayTile().getLast().getLast())
            )return 3;
    }

    return 0;
}

bool canPeng()
{
    //��Ϊ�Լң�������
    if (memory.getCurrPlayer() != memory.getMyPosistion() && memory.getCntHand(memory.getCurrPlayTile())==2) {
        return true;
    }
    return false;
}

bool canMinGang()
{
    if (memory.getCurrPlayer() != memory.getMyPosistion() && memory.getCntHand(memory.getCurrPlayTile()) == 3) {
        return true;
    }
    return false;
}

bool canAnGang()
{
    if (memory.getCurrPlayer() == memory.getMyPosistion() && memory.getCntHand(memory.getCurrPlayTile()) == 4) {
        return true;
    }
    return false;
}

bool canBuGang()
{
    vector<Mahjong>MyPeng = memory.getPeng(memory.getMyPosistion());
    for (auto& it : MyPeng) {
        if (it == memory.getCurrPlayTile()) {
            return true;
        }
    }
    return false;
}
