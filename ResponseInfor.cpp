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

    int NowFan = Handtiles_Point(SELFDRAWN);
    if (NowFan != -3)return "HU";

    Hand_Claim hands;
    int NowShang = Handtiles_ShangTing();

    bool flag = false;

    Mahjong card1, card2;

    if (memory.getCurrPlayer() == memory.getMyPosistion())
    {
        if (memory.getCurrAction() == DRAW) 
        {//到自己回合并且摸牌，尚未打出牌
            //BUGANG Card1（摸得是Card1）
            if (canBuGang()) 
            {
                hands.addBuGang(currPlayTile);
                string h1 = hands.getFormatHandSting();
                int TempShang = Handtiles_ShangTing_Temp(h1);
                if (NowShang > TempShang)
                {
                    responseStr = "BUGANG ";
                    responseStr += currPlayTile.getTileString();
                }
                hands.removeBuGang(currPlayTile);
                return responseStr;
            }
            //GANG Card1（摸得是Card1）
            else if (canAnGang()) 
            {
                hands.addAnGang(currPlayTile);
                string h1 = hands.getFormatHandSting();
                int TempShang = Handtiles_ShangTing_Temp(h1);
                if (NowShang > TempShang)
                {
                    responseStr = "GANG ";
                    responseStr += currPlayTile.getTileString();
                }
                hands.removeAnGang(currPlayTile);
                return responseStr;
            }

            //PLAY Card1（打手牌Card1）
            // 
            // //摸切
            responseStr = "PLAY ";
            int len = hands.handTile.size();
            int perfectlo = 0, MinShang = INT_MAX, fro = 0;
            for (int i = 0; i < len; i++)
            {
                Mahjong tmp = hands.handTile[i];
                if (tmp == hands.handTile[i - 1])
                    continue;
                hands.removeHand(hands.handTile[i]);
                string t1 = hands.getFormatHandSting();
                int Ts = Handtiles_ShangTing_Temp(t1);
                if (Ts < MinShang)
                {
                    perfectlo = i;
                    MinShang = Ts;
                }
                hands.addHand(tmp);
            }
            if (MinShang != std::numeric_limits<int>::max() && MinShang <= NowShang)
                responseStr += hands.handTile[perfectlo].getTileString();
            return responseStr;
        }
    }
    else
    {
        int chiTarget = canChi();
        //GANG
        if (canMinGang())
        {
            int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
            int tars = memory.getFormatPosition(myP, otherP);
            hands.addMinGang(currPlayTile, tars);
            string t1 = hands.getFormatHandSting();
            int Ts = Handtiles_ShangTing_Temp(t1);
            if (NowShang > Ts)
            {
                responseStr = "GANG ";
                memory.playTile(currPlayTile, GANG);
                //     hands.
                flag = true;
            }
            hands.removeMinGang(currPlayTile);
        }
        //PENG Card1（打Card1）
        if (canPeng()) {
            int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
            int tars = memory.getFormatPosition(myP, otherP);
            hands.addPeng(currPlayTile, tars);
            string t1 = hands.getFormatHandSting();
            int Ts = Handtiles_ShangTing_Temp(t1);

            if (Ts == -1)return"HU";

            Hand_Claim thands;
            int len = thands.handTile.size();
            int perfectlo = 0, MinShang = INT_MAX, fro = 0;
            for (int i = 0; i < len; i++)
            {
                Mahjong tmp = thands.handTile[i];
                if (tmp == thands.handTile[i - 1])
                    continue;

                thands.removeHand(thands.handTile[i]);
                string t1 = thands.getFormatHandSting();
                int Ts = Handtiles_ShangTing_Temp(t1);
                if (Ts < MinShang)
                {
                    responseStr = "PENG ";
                    perfectlo = i;
                    MinShang = Ts;
                }
                thands.addHand(tmp);
            }
            if (MinShang != std::numeric_limits<int>::max() && MinShang <= NowShang) {
                responseStr += thands.handTile[perfectlo].getTileString();
                flag = true;
            }

            hands.removePeng(currPlayTile);
        }
        //CHI Card1 Card2（吃Card1打Card2）
        if (chiTarget != -1) {
            hands.addChi(currPlayTile, chiTarget);
            string t1 = hands.getFormatHandSting();
            int Ts = Handtiles_ShangTing_Temp(t1);

            if (Ts == -1)return"HU";

            Mahjong cTarget;
            switch (canChi()) {
            case 0: {
                cTarget = currPlayTile.getNext();
                break;
            }
            case 1: {
                cTarget = currPlayTile;
                break;
            }
            case 2: {
                cTarget = currPlayTile.getLast();
                break;
            }
            }

            Hand_Claim thands;
            int len = thands.handTile.size();
            int perfectlo = 0, MinShang = INT_MAX, fro = 0;
            for (int i = 0; i < len; i++)
            {
                Mahjong tmp = thands.handTile[i];
                if (tmp == thands.handTile[i - 1])
                    continue;
                thands.removeHand(thands.handTile[i]);
                string t1 = thands.getFormatHandSting();
                int Ts = Handtiles_ShangTing_Temp(t1);
                if (Ts < MinShang)
                {
                    responseStr = "CHI ";
                    responseStr += cTarget.getTileString() + " ";
                    perfectlo = i;
                    MinShang = Ts;
                }
                thands.addHand(tmp);
            }
            if (MinShang != std::numeric_limits<int>::max() && MinShang <= NowShang)
            {
                responseStr += thands.handTile[perfectlo].getTileString();
                flag = true;
            }

            hands.removeChi(currPlayTile, chiTarget);
        }
    }

    if (flag)return responseStr;
    return "PASS";
}

int canChi()
{
    //不用担心19边界情况，0处无牌，默认不存在
    if (memory.getCurrPlayTile().isNum() ) {
        if (memory.getCntHand(memory.getCurrPlayTile().getNext()) &&
            memory.getCntHand(memory.getCurrPlayTile().getNext().getNext())
            )return 0;
        if (memory.getCntHand(memory.getCurrPlayTile().getLast()) &&
            memory.getCntHand(memory.getCurrPlayTile().getNext())
            )return 1;
        if (memory.getCntHand(memory.getCurrPlayTile().getLast()) &&
            memory.getCntHand(memory.getCurrPlayTile().getLast().getLast())
            )return 2;
    }

    return -1;
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
