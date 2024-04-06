#include "ResponseInfor.h"
#include"StatusMemory.h"
#include"FanCalculator.h"

#include<algorithm>
#include<iostream>

string response()
{
    Mahjong currPlayTile = memory.getCurrPlayTile();
    if (turn == 0 || turn == 1)return "PASS";
    string responseStr = "";
    int pi = 0;
    int NowShang = Handtiles_ShangTing();
    if (NowShang==-1)return "HU";
    if (memory.getCurrPlayer() == memory.getMyPosistion())
    {
        if (memory.getCurrAction() == DRAW){//���Լ��غϲ������ƣ���δ�����
            //BUGANG Card1��������Card1��
            if (canBuGang()){
                Mahjong tmp = memory.getCurrPlayTile();
                vector<Mahjong> hand = memory.getHandTile(), thand = hand;
                int len = thand.size();
                for (int i = 0; i < len; i++)
                {
                    if (thand[i].getTile() == tmp)
                    {
                        thand.erase(thand.begin()+i);
                        break;
                    }
                }
                int TestShang = Handtiles_ShangTing();
                if (NowShang > TestShang)
                {
                    responseStr = "BUGANG ";
                    memory.playTile(currPlayTile, BUGANG);
                    responseStr += currPlayTile.getTileString();

                    return responseStr;
                }
                else
                {
                    responseStr = "PASS";
                    return responseStr;
                }
            }
            //GANG Card1��������Card1��
            else if (canAnGang()){
                memory.sortHand();
                Mahjong tmp = memory.getCurrPlayTile();
                vector<Mahjong> hand = memory.getHandTile(), thand = hand;
                int len = thand.size();
                for (int i = 0; i < len; i++)
                {
                    if (thand[i].getTile() == tmp)
                    {
                        for(int j=0;j<4;j++)
                        thand.erase(thand.begin()+i);
                        break;
                    }
                }
                int TestShang = Handtiles_ShangTing();
                if (NowShang > TestShang)
                {
                    responseStr = "GANG ";
                    memory.playTile(currPlayTile, ANGANG);
                    responseStr += currPlayTile.getTileString();
                    return responseStr;
                }
                else
                {
                    return "PASS";
                }
            }
            //PLAY Card1��������Card1��
            else {//����
                responseStr = "PLAY ";
                memory.sortHand();
                vector<Mahjong> hand = memory.getHandTile(), thand = hand;
                int perfectlo = 0, maxShang = INT_MAX, frotile = 0, len = thand.size();
                for (int i = 0; i < len; i++)
                {
                    int tmp = thand[i].getTile();
                    if (tmp == frotile)continue;
                    frotile = tmp;
                    thand.erase(thand.begin() + i);
                    int tmpShang = Handtiles_ShangTing();
                    if (tmpShang <= maxShang)
                    {
                        perfectlo = i;
                        maxShang = tmpShang;
                    }
                    thand = hand;
                }
                pi = perfectlo;
                //ѡ��Ҫ�����
            }
        }
        else{//���Լ��غ����Լ�����ƻ������
            return "PASS";
        }
    }
    else
    {

        //GANG
        if (canMinGang()) {
            memory.sortHand();
            vector<Mahjong> hand = memory.getHandTile(), thand = hand;
            int len = thand.size();
            for (int i = 0; i < len; i++)
            {
                if (thand[i].getTile() == currPlayTile)
                {
                    for (int j = 0; j < 3; j++)
                     thand.erase(thand.begin() + i);
                    break;
                }
            }
            int TestShang = Handtiles_ShangTing();
            if (NowShang > TestShang)
            {
                responseStr = "GANG ";
                memory.playTile(currPlayTile, GANG);

                return responseStr;
            }
            else
            {
                return "PASS";
            }
        }
        //PENG Card1����Card1��
        else if (canPeng()) {
            memory.sortHand();
            vector<Mahjong> hand = memory.getHandTile(), thand = hand;
            int len = thand.size();
            for (int i = 0; i < len; i++)
            {
                if (thand[i].getTile() == currPlayTile)
                {
                    for (int j = 0; j < 2; j++)
                        thand.erase(thand.begin() + i);
                    break;
                }
            }
            int TestShang = Handtiles_ShangTing();
            if (NowShang > TestShang)
            {
                responseStr = "PENG ";
                memory.playTile(currPlayTile, PENG);
            }
            else return "PASS";
        }
        //CHI Card1 Card2����Card1��Card2��
        else if (canChi()) {
            memory.sortHand();
            vector<Mahjong> hand = memory.getHandTile(), thand = hand;
            int len = thand.size();
            for (int i = 0; i < len; i++)
            {
                if (thand[i].getTile() == currPlayTile-1&&thand[i+1].getTile()!=thand[i])
                {
                    for (int j = 0; j < 2; j++)
                        thand.erase(thand.begin() + i);
                    break;
                }
            }
            int TestShang = Handtiles_ShangTing();
            if (NowShang > TestShang)
            {
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
        else{
            return "PASS";
        }
    }
    //ѡ������
    responseStr += memory.getHandTile()[pi].getTileString();
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
    vector<pair<Mahjong, int>>MyPeng = memory.getPeng(memory.getMyPosistion());
    for (auto& it : MyPeng) {
        if (it.first == memory.getCurrPlayTile()) {
            return true;
        }
    }
    return false;
}
