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

    bool flag = false;
    int pi = 0;
    int NowShang = Handtiles_ShangTing();
    if (!NowShang)return "HU";
    if (memory.getCurrPlayer() == memory.getMyPosistion()) {

        if (memory.getCurrAction() == DRAW) {//到自己回合并且摸牌，尚未打出牌
            //BUGANG Card1（摸得是Card1）
            if (canBuGang()) {
                responseStr = "BUGANG ";
                memory.playTile(currPlayTile, BUGANG);

                responseStr += currPlayTile.getTileString();

                return responseStr;
            }
            //GANG Card1（摸得是Card1）
            else if (canAnGang()) {
                responseStr = "GANG ";
                memory.playTile(currPlayTile, ANGANG);

                responseStr += currPlayTile.getTileString();

                return responseStr;
            }
            //PLAY Card1（打手牌Card1）
            else {//摸切
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
                //选择要打的牌
            }
        }
        else{//到自己回合已自己打出牌或吃碰杠
            return "PASS";
        }

    }
    else
    {

        //GANG
        if (canMinGang()) {
            responseStr = "GANG ";
            memory.playTile(currPlayTile, GANG);

            return responseStr;
        }
        //PENG Card1（打Card1）
        else if (canPeng()) {
            responseStr = "PENG ";
            memory.playTile(currPlayTile, PENG);
        }
        //CHI Card1 Card2（吃Card1打Card2）
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
        else{
            return "PASS";
        }
    }
    //选择打的牌
    responseStr += memory.getHandTile()[pi].getTileString();
    return responseStr;
}

int canChi()
{
    //不用担心19边界情况，0处无牌，默认不存在
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
    //不为自家，手上牌
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
