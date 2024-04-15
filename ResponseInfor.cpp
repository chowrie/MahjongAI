#include "ResponseInfor.h"

#include<algorithm>
#include<iostream>
#include<climits>

string response()
{
    //currPlayTile：坎张、边张、单钓将
    Mahjong currPlayTile = memory.getCurrPlayTile();


    int currPlayer = memory.getCurrPlayer();
    bool GangFlag = memory.getGangFlag();
    action currAction = memory.getCurrAction();
    Hand_Claim hands;
    int Minshang = INT_MAX;


    if (turn == 0 || turn == 1)return "PASS";
    string responseStr = "";
    bool flag = false;



    Mahjong card1, card2;

    //在自己的回合中包含以下动作，需要分别判别并进行响应：
    //1.自家摸牌，存在暗杠、补杠、自摸的可能（这个部分是在自己回合，并且摸完牌后）
    //应当首先判别是否自摸并计算番数（可直接调用算番器计算），如果满足要求，直接返回胡牌
    //若不满足，考虑暗杠或补杠后是否会减少上听数（须在杠后直接计算，而不用拟丢牌）
    //若暗杠和补杠都不能减少上听数，再考虑打出一张手牌
    //在打出手牌时，考虑对多个上听数最大的牌建立无用牌组，用于防守或进攻
    //2.自家打、吃碰
    //直接返回PASS


    //胡牌标记定义处
    //1.和绝张
    win_flag_t Winflag = 0;
    if (isHeJueZhang(currPlayTile,currAction))
    {
        Winflag |= WIN_FLAG_4TH_TILE;
    }

    if (memory.getCurrPlayer() == memory.getMyPosistion())
    {
        if (memory.getCurrAction() == DRAW)
        {
            //到自己回合并且摸牌，尚未打出牌
        //2.妙手回春
        //3.自摸
        //4.杠上开花
        //由于杠牌后、摸牌前一定无法胡牌，故不需要在补杠、暗杠里判断胡牌
            Winflag |= WIN_FLAG_SELF_DRAWN;
            if (isHandSpring())
                Winflag |= WIN_FLAG_WALL_LAST;
            if (isGangShangKaiHua(currAction, memory.getGangFlag()))
                Winflag |= WIN_FLAG_ABOUT_KONG;


            string NowHands = hands.getFormatHandSting();
            int Nowfan = Handtiles_Point(NowHands, Winflag, currPlayTile);
            if (Nowfan >= 8)return "HU";



            //BUGANG Card1（摸得是Card1）
            if (canBuGang())
            {
                hands.addBuGang(currPlayTile);
                string h1 = hands.getFormatHandSting();
                int TempShang = Handtiles_ShangTing_Temp(h1);
                if (Minshang >= TempShang)
                {
                    responseStr = "BUGANG ";
                    responseStr += currPlayTile.getTileString();

                    Minshang = TempShang;
                }
                hands.removeBuGang(currPlayTile);
            }
            //GANG Card1（摸得是Card1）
            else if (canAnGang())
            {

                hands.addAnGang(currPlayTile);
                string h1 = hands.getFormatHandSting();
                int TempShang = Handtiles_ShangTing_Temp(h1);
                if (Minshang >= TempShang)
                {
                    responseStr = "GANG ";
                    responseStr += currPlayTile.getTileString();

                    Minshang = TempShang;
                }
                hands.removeAnGang(currPlayTile);
            }

            int tempMinShang = Minshang;
            //PLAY Card1（打手牌Card1）
             //摸切
            Mahjong playedtile = Search_playtile(hands, Minshang);
            if (playedtile.getTile()>0)
            {
                responseStr = "PLAY ";
                responseStr += playedtile.getTileString();
            }
            flag = true;
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
        //2.荣和
        //3.海底捞月
        //4.抢杠和

        Winflag |= WIN_FLAG_DISCARD;
       
        if (isSeaMoon(currPlayer))
            Winflag |= WIN_FLAG_WALL_LAST;
        if (isQiangGangHe(currAction))
            Winflag |= WIN_FLAG_ABOUT_KONG;

        hands.addHand(currPlayTile);
        string t1 = hands.getFormatHandSting();

        int Nowfan = Handtiles_Point(t1, Winflag, currPlayTile);
        if (Nowfan >= 8)return "HU";
        hands.removeHand(currPlayTile);

        Minshang = Handtiles_ShangTing_Temp(t1);

        if (currAction != GANG) {//对手杠牌回合中，若自己无法进行抢杠和，则不执行任何操作，直接PASS

        //单钓将

            int chiTarget = canChi();

            //GANG
            if (canMinGang())
            {

                int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
                int tars = memory.getFormatPosition(myP, otherP);

                hands.addMinGang(currPlayTile, tars);
                string t1 = hands.getFormatHandSting();
                int Ts = Handtiles_ShangTing_Temp(t1);
                if (Minshang >= Ts)
                {
                    responseStr = "GANG";

                    flag = true;

                    Minshang = Ts;
                }
                hands.removeMinGang(currPlayTile);

            }

            //PENG Card1（打Card1）
            if (canPeng())
            {
                int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
                int tars = memory.getFormatPosition(myP, otherP);

                hands.addPeng(currPlayTile, tars);


                int PengFlag = false;

                //胡不了，选牌
                //缺少多张无用牌的情况
                int len = hands.handTile.size();
                int perfectlo = 0;
                for (int i = 0; i < len; i++)
                {
                    Mahjong tmp = hands.handTile[i];
                    if (i > 0 && tmp == hands.handTile[i - 1])
                        continue;

                    hands.removeHand(hands.handTile[i]);

                    string t1 = hands.getFormatHandSting();
                    int Ts = Handtiles_ShangTing_Temp(t1);

                    if (Ts <= Minshang)
                    {
                        responseStr = "PENG ";
                        perfectlo = i;
                        Minshang = Ts;

                        PengFlag = true;
                    }
                    hands.addHand(tmp);
                }
                if (PengFlag) {
                    responseStr += hands.handTile[perfectlo].getTileString();
                    flag = true;
                }
                hands.removePeng(currPlayTile);

            }

            //CHI Card1 Card2（吃Card1打Card2）
            if (chiTarget) {

                hands.addChi(currPlayTile, chiTarget);

                //胡不了
                Mahjong cTarget;

                switch (chiTarget) {
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

                int ChiFlag = false;

                int len = hands.handTile.size();
                int perfectlo = 0;
                for (int i = 0; i < len; i++)
                {
                    Mahjong tmp = hands.handTile[i];
                    if (i > 0 && tmp == hands.handTile[i - 1])
                        continue;
                    hands.removeHand(hands.handTile[i]);
                    string t1 = hands.getFormatHandSting();
                    int Ts = Handtiles_ShangTing_Temp(t1);
                    if (Ts <= Minshang)
                    {
                        responseStr = "CHI ";
                        responseStr += cTarget.getTileString() + " ";
                        perfectlo = i;
                        Minshang = Ts;

                        ChiFlag = true;
                    }
                    hands.addHand(tmp);
                }
                if (ChiFlag)
                {
                    responseStr += hands.handTile[perfectlo].getTileString();
                    flag = true;
                }

                hands.removeChi(cTarget, chiTarget);
            }

        }
    }   

    //

    if (flag)return responseStr;
      return "PASS";
}
int canChi()
{
    if (memory.getCurrAction() == PLAY) {
        //不用担心19边界情况，0处无牌，默认不存在
        //暂未考虑有多种吃牌的情况下的考虑
        if (memory.getCurrPlayer() == memory.getLastPosition() && memory.getCurrPlayTile().isNum()) {
            if (memory.getCntHand(memory.getCurrPlayTile().getNext()) &&
                memory.getCntHand(memory.getCurrPlayTile().getNext().getNext())
                )return 1;
            if (memory.getCntHand(memory.getCurrPlayTile().getLast()) &&
                memory.getCntHand(memory.getCurrPlayTile().getLast().getLast())
                )return 3;
            if (memory.getCntHand(memory.getCurrPlayTile().getLast()) &&
                memory.getCntHand(memory.getCurrPlayTile().getNext())
                )return 2;
        }
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

bool isHeJueZhang(Mahjong& majang, action Action)
{
    if (Action == DRAW) {
        return memory.getUnPlayed()[majang] + memory.getCntHand(majang) == 1;
    }
    else {
        return memory.getUnPlayed()[majang] + memory.getCntHand(majang) == 0;
    }
}

bool isQiangGangHe(action& currAction)
{
    return currAction == GANG;
}

bool isGangShangKaiHua(action& currAction, bool GangFlag)
{
    return currAction == DRAW && GangFlag == true;
}



bool isHandSpring()
{
    return memory.getTileWallNum(memory.getMyPosistion())==0;
}

bool isSeaMoon(int idx)
{
    return memory.getTileWallNum(memory.getMyPosistion()) == 0;
}

