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

            //选择要打的牌
        }
    }
    else {

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
    }

    //选择打的牌
    responseStr += memory.getHandTile()[0].getTileString();

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
    vector<Mahjong>MyPeng = memory.getPeng(memory.getMyPosistion());
    for (auto& it : MyPeng) {
        if (it == memory.getCurrPlayTile()) {
            return true;
        }
    }
    return false;
}
