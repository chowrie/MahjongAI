#include "ResponseInfor.h"
#include"StatusMemory.h"
#include<algorithm>

string response()
{
    if (turn == 0 || turn == 1)return "PASS";

    string responseStr = "";

    bool flag = false;

    if (memory.getCurrPlayer() == memory.getMyPosistion()) {
        Mahjong currDrawTile = memory.getCurrDrawTile();

        if (canBuGang()) {
            responseStr = "BUGANG";
            memory.playTile(currDrawTile, BUGANG);

            responseStr += currDrawTile.getTileString();

            return responseStr;
        }
        else if (canAnGang()) {
            responseStr = "GANG";
            memory.playTile(currDrawTile, ANGANG);
            
            responseStr += currDrawTile.getTileString();

            return responseStr;
        }
        else {//摸切
            responseStr = "PLAY";
        }
        flag = true;
    }
    else {
        Mahjong currPlayTile = memory.getCurrDrawTile();
        if (canMinGang()) {
            responseStr = "GANG";
            memory.playTile(currPlayTile, GANG);

            flag = true;
        }
        else if (canPeng()) {
            responseStr = "PENG";
            memory.playTile(currPlayTile, PENG);

            flag = true;
        }
        else if (canChi()) {
            responseStr = "CHI";
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
            responseStr += chiTarget.getTileString();
            memory.playTile(chiTarget, CHI);

            flag = true;
        }
    }
    if(flag){
        responseStr += memory.getHandTile()[0].getTileString();

        return responseStr;
    }
    else {
        return "PASS";
    }
}

int canChi()
{
    //不用担心19边界情况，0处无牌，默认不存在
    if (memory.getCurrPlayer() == memory.getLastPosition()&&memory.getCurrPlayTile().isNum() ) {
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

    return false;
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
    if (memory.getCurrPlayer() == memory.getMyPosistion() && memory.getCntHand(memory.getCurrDrawTile()) == 4) {
        return true;
    }
    return false;
}

bool canBuGang()
{
    if (memory.getCurrPlayer() == memory.getMyPosistion()) {
        vector<Mahjong>MyPeng = memory.getPeng(memory.getMyPosistion());
        for (auto& it : MyPeng) {
            if (it == memory.getCurrDrawTile()) {
                return true;
            }
        }
    }
    return false;
}
