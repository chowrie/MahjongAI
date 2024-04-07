#include "ResponseInfor.h"
#include"StatusMemory.h"
#include"FanCalculator.h"

#include<algorithm>
#include<iostream>
#include<climits>

string response()
{
    Mahjong currPlayTile = memory.getCurrPlayTile();
    int currPlayer = memory.getCurrPlayer();
    action currAction = memory.getCurrAction();

    if (turn == 0 || turn == 1)return "PASS";

    string responseStr = "";

    //{可能需要删除的部分，我们应该先判别目前回合的状态再决定是否计算上听数以及番数

    string NowHands = memory.getFormatHandSting();

    int NowFan = Handtiles_Point(NowHands,SELFDRAWN);
    if (NowFan != -3&&NowFan>=8)return "HU";

    Hand_Claim hands;
    int NowShang = Handtiles_ShangTing();

    bool flag = false;


    //}

    Mahjong card1, card2;

    //在自己的回合中包含以下动作，需要分别判别并进行响应：
    //1.自家摸牌，存在暗杠、补杠、自摸的可能（这个部分是在自己回合，并且摸完牌后）
    //应当首先判别是否自摸并计算番数（可直接调用算番器计算），如果满足要求，直接返回胡牌
    //若不满足，考虑暗杠或补杠后是否会减少上听数（须在杠后直接计算，而不用拟丢牌）
    //若暗杠和补杠都不能减少上听数，再考虑打出一张手牌
    //在打出手牌时，考虑对多个上听数最大的牌建立无用牌组，用于防守或进攻
    //2.自家打、吃碰
    //直接返回PASS
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

            responseStr = "PLAY ";
            //PLAY Card1（打手牌Card1）
            // 
            // //摸切
            int len = hands.handTile.size();
            int perfectlo = 0, MinShang = INT_MAX;
            for (int i = 0; i < len; i++)
            {
                Mahjong tmp = hands.handTile[i];
                if (i > 0 && tmp == hands.handTile[i - 1])
                    continue;
                hands.removeHand(hands.handTile[i]);
                string t1 = hands.getFormatHandSting();
                int Ts = Handtiles_ShangTing_Temp(t1);
                if (Ts < MinShang)
                {
                    perfectlo = i;
                    MinShang = Ts;
                    flag = true;
                }
                hands.addHand(tmp);
            }
            if (MinShang != std::numeric_limits<int>::max() && MinShang <= NowShang)
                responseStr += hands.handTile[perfectlo].getTileString();
            return responseStr;
        }
    }
    
    //它家吃碰杠、出牌，也需分别判别
    // 1.它家出牌（可能是在吃碰后出牌），在它家出牌后，我们考虑这张牌能否用于吃碰杠甚至胡牌，方法如下
    // 对于可能的吃碰，在吃碰后，首先计算是否能 胡牌 ，如果能胡且>=8番，直接返回
    // 若不能胡，再拟丢牌进行上听数考虑是否吃碰，并建立无用牌组
    // 对于可能的杠，在杠后直接计算计算上听数比较，不用拟丢牌
    // 若不能吃碰杠，直接PASS
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

            int len = hands.handTile.size();
            int perfectlo = 0, MinShang = INT_MAX;
            for (int i = 0; i < len; i++)
            {
                Mahjong tmp = hands.handTile[i];
                if (i > 0 && tmp == hands.handTile[i - 1])
                    continue;

                hands.removeHand(hands.handTile[i]);
                string t1 = hands.getFormatHandSting();
                int Ts = Handtiles_ShangTing_Temp(t1);
                if (Ts < MinShang)
                {
                    responseStr = "PENG ";
                    perfectlo = i;
                    MinShang = Ts;
                }
                hands.addHand(tmp);
            }
            if (MinShang != std::numeric_limits<int>::max() && MinShang <= NowShang) {
                responseStr += hands.handTile[perfectlo].getTileString();
                flag = true;
            }

            hands.removePeng(currPlayTile);
        }

        //CHI Card1 Card2（吃Card1打Card2）
        if (chiTarget) {
            hands.addChi(currPlayTile, chiTarget);
            string t1 = hands.getFormatHandSting();
            int Ts = Handtiles_ShangTing_Temp(t1);

            if (Ts == -1)return"HU";

            Mahjong cTarget;
            switch (canChi()) {
            case 1: {
                cTarget = currPlayTile.getNext();
                break;
            }
            case 2: {
                cTarget = currPlayTile;
                break;
            }
            case 3: {
                cTarget = currPlayTile.getLast();
                break;
            }
            }

            int len = hands.handTile.size();
            int perfectlo = 0, MinShang = INT_MAX;
            for (int i = 0; i < len; i++)
            {
                Mahjong tmp = hands.handTile[i];
                if (i > 0 && tmp == hands.handTile[i - 1])
                    continue;
                hands.removeHand(hands.handTile[i]);
                string t1 = hands.getFormatHandSting();
                int Ts = Handtiles_ShangTing_Temp(t1);
                if (Ts < MinShang)
                {
                    responseStr = "CHI ";
                    responseStr += cTarget.getTileString() + " ";
                    perfectlo = i;
                    MinShang = Ts;
                }
                hands.addHand(tmp);
            }
            if (MinShang != std::numeric_limits<int>::max() && MinShang <= NowShang)
            {
                responseStr += hands.handTile[perfectlo].getTileString();
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
    if (memory.getCurrPlayer()==memory.getLastPosition() && memory.getCurrPlayTile().isNum()) {
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
