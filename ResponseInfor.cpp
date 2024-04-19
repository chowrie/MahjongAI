#include "ResponseInfor.h"
#include "defend.h"

#include<algorithm>
#include<iostream>
#include<climits>


string response()
{
    bool noTileFlag = isNoTile();


    //currPlayTile：坎张、边张、单钓将
    Mahjong currPlayTile = memory.getCurrPlayTile();

    int currPlayer = memory.getCurrPlayer();
    bool GangFlag = memory.getGangFlag();
    action currAction = memory.getCurrAction();
    Hand_Claim hands;
    map<Mahjong, int> usednum;
    int Minshang = INT_MAX;


    if (turn == 0 || turn == 1)return "PASS";
    string responseStr = "";
    bool flag = false;

    vector<Mahjong>unusedTile;


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
    if (isHeJueZhang(currPlayTile, currAction))
    {
        Winflag |= WIN_FLAG_4TH_TILE;
    }

    if (memory.getCurrPlayer() == memory.getMyPosistion())
    {
        if (memory.getCurrAction() == DRAW)
        {
            //是否打算杠
            bool anGangFlag = false;
            bool buGangFlag = false;

        //到自己回合并且摸牌，尚未打出牌
        //2.妙手回春
        //3.自摸
        //4.杠上开花
        //由于杠牌后、摸牌前一定无法胡牌，故不需要在补杠、暗杠里判断胡牌
            Winflag |= WIN_FLAG_SELF_DRAWN;
            if (noTileFlag)
                Winflag |= WIN_FLAG_WALL_LAST;
            if (isGangShangKaiHua(currAction, memory.getGangFlag()))
                Winflag |= WIN_FLAG_ABOUT_KONG;


            string NowHands = hands.getFormatHandSting();
            int Nowfan = Handtiles_Point(NowHands, Winflag, currPlayTile);
            if (Nowfan >= 8)return "HU";

            //杠牌没有出牌，因此没有所谓的无效牌供选择

            //BUGANG Card1（摸得是Card1）
            if (canBuGang())
            {
                hands.addBuGang(currPlayTile);
                string h1 = hands.getFormatHandSting();
                int usenums = 0;
                int TempShang = Handtiles_ShangTing_Temp(h1, usenums);
                if (Minshang > TempShang)
                {
                    responseStr = "BUGANG ";
                    responseStr += currPlayTile.getTileString();

                    //unusedTile.clear();
                    //usednum.clear();

                    //unusedTile.push_back(currPlayTile);
                    //usednum.insert({ currPlayTile, usenums });

                    Minshang = TempShang;

                    buGangFlag = true;
                }
                else if (Minshang == TempShang) {

                    //补杠需注明杠牌
                    responseStr = "BUGANG ";
                    responseStr += currPlayTile.getTileString();

                    //unusedTile.push_back(currPlayTile);
                    //usednum.insert({ currPlayTile, usenums });

                    Minshang = TempShang;
                }

                hands.removeBuGang(currPlayTile);
            }
            //GANG Card1（摸得是Card1）
            else if (canAnGang())
            {

                hands.addAnGang(currPlayTile);
                string h1 = hands.getFormatHandSting();
                int usenums = 0;
                int TempShang = Handtiles_ShangTing_Temp(h1, usenums);
                if (Minshang > TempShang)
                {


                    //暗杠无需注明杠牌
                    //换言之，对手之间不清楚彼此暗杠的牌
                    responseStr = "GANG";
                    //responseStr += currPlayTile.getTileString();

                    //unusedTile.clear();
                    //usednum.clear();

                    //unusedTile.push_back(currPlayTile);
                    //usednum.insert({ currPlayTile, usenums });

                    Minshang = TempShang;

                    buGangFlag = true;
                    anGangFlag = false;
                }
                else if (Minshang == TempShang) {

                    responseStr = "GANG ";
                    //responseStr += currPlayTile.getTileString();

                    //unusedTile.push_back(currPlayTile);
                    //usednum.insert({ currPlayTile, usenums });

                    Minshang = TempShang;

                    buGangFlag = true;
                }
                hands.removeAnGang(currPlayTile);
            }




            //PLAY Card1（打手牌Card1）
             //摸切

            sort(hands.handTile.begin(), hands.handTile.end(), cmp());

            int len = hands.handTile.size();
            int perfectlo = 0;
            bool isChange = false;
            for (int i = 0; i < len; i++)
            {
                Mahjong tmp = hands.handTile[i];
                if (i > 0 && tmp == hands.handTile[i - 1])
                    continue;
                hands.removeHand(hands.handTile[i]);
                string t1 = hands.getFormatHandSting();
                int usenums = 0;
                int Ts = Handtiles_ShangTing_Temp(t1, usenums);
                if (Ts < Minshang)
                {
                    responseStr = "PLAY ";
                    //

                    unusedTile.clear();
                    usednum.clear();

                    unusedTile.push_back(tmp);
                    usednum.insert({ tmp, usenums });

                    Minshang = Ts;


                    buGangFlag = false;
                    anGangFlag = false;
                }
                else if (Ts == Minshang) {


                    unusedTile.push_back(tmp);
                    usednum.insert({ tmp, usenums });

                    responseStr = "PLAY ";

                    //补杠虽然加番，但是存在点炮风险，此处待定
                    buGangFlag = false;
                }
                hands.addHand(tmp);
                sort(hands.handTile.begin(), hands.handTile.end(), cmp());
            }
            string temphands = hands.getFormatHandSting();
            //   int Shang_tocheck = Handtiles_ShangTing_Temp(temphands);
            


            sort(unusedTile.begin(), unusedTile.end(), cmp());


            Mahjong playedTile;

            //到达此处意味着在摸牌阶段
            if (quitHu(Minshang,unusedTile)) {//弃胡


                //若Shanten杠<Shanten切，但弃胡，则不杠，选择切，更改responseStr
                //防止安全牌减少和补杠点炮

                responseStr = "PLAY ";
                playedTile = get_defend_tile(hands.handTile);
                responseStr += playedTile.getTileString();


            }
            else{//不弃胡

                if (!buGangFlag && !anGangFlag) {


                    //此处无论能否杠我们都不打算杠
                    //换言之进入时 responseStr = "PLAY ";
                    responseStr = "PLAY ";//保险处理
                    playedTile = get_defend_tile_1(unusedTile);
                    responseStr += playedTile.getTileString();
                }
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

        if (noTileFlag)
            Winflag |= WIN_FLAG_WALL_LAST;
        if (isQiangGangHe(currAction))
            Winflag |= WIN_FLAG_ABOUT_KONG;

        hands.addHand(currPlayTile);
        string t1 = hands.getFormatHandSting();

        int Nowfan = Handtiles_Point(t1, Winflag, currPlayTile);
        if (Nowfan >= 8)return "HU";
        hands.removeHand(currPlayTile);


        int rr = 0;

        //留牌或许是更好的选择？
        //将计算手牌上听数放在最后

        Minshang = INT_MAX;

        //Minshang = Handtiles_ShangTing_Temp(t1, rr);
        int Shang_tocheck = Minshang;

        //1.对手杠牌回合中，若自己无法进行抢杠和，则不执行任何操作，直接PASS
        //2.任意一家牌墙为空时，无法吃碰杠
        if (currAction != GANG && !noTileFlag) {

            int GangFlag = false;

            int PengFlag = false;

            int chiTarget = canChi();

            int ChiFlag = false;


            //首先获取当前情况下副露

            //GANG
            if (canMinGang())
            {

                int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
                int tars = memory.getFormatPosition(myP, otherP);

                hands.addMinGang(currPlayTile, tars);
                sort(hands.handTile.begin(), hands.handTile.end(), cmp());
                string t1 = hands.getFormatHandSting();
                int usenums = 0;
                int Ts = Handtiles_ShangTing_Temp(t1, usenums);
                if (Minshang >= Ts)
                {
                    responseStr = "GANG";

                    GangFlag = true;

                    Minshang = Ts;
                }
                hands.removeMinGang(currPlayTile);
                sort(hands.handTile.begin(), hands.handTile.end(), cmp());

            }

            //PENG Card1（打Card1）
            if (canPeng())
            {
                int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
                int tars = memory.getFormatPosition(myP, otherP);

                hands.addPeng(currPlayTile, tars);



                sort(hands.handTile.begin(), hands.handTile.end(), cmp());
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
                    int usenums = 0;
                    int Ts = Handtiles_ShangTing_Temp(t1, usenums);

                    if (Ts < Minshang)
                    {
                        responseStr = "PENG ";

                        unusedTile.clear();
                        usednum.clear();

                        unusedTile.push_back(tmp);
                        usednum.insert({ tmp, usenums });

                        Minshang = Ts;

                        PengFlag = true;
                    }
                    else if (Ts == Minshang) {

                        responseStr = "PENG ";

                        if (!PengFlag) {
                            unusedTile.clear();
                            usednum.clear();
                        }

                        unusedTile.push_back(tmp);
                        usednum.insert({ tmp, usenums });
                        PengFlag = true;

                    }
                    hands.addHand(tmp);
                    sort(hands.handTile.begin(), hands.handTile.end(), cmp());
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


                sort(hands.handTile.begin(), hands.handTile.end(), cmp());
                int len = hands.handTile.size();
                int perfectlo = 0;
                for (int i = 0; i < len; i++)
                {
                    Mahjong tmp = hands.handTile[i];
                    if (i > 0 && tmp == hands.handTile[i - 1])
                        continue;
                    hands.removeHand(hands.handTile[i]);
                    string t1 = hands.getFormatHandSting();
                    int usenums = 0;
                    int Ts = Handtiles_ShangTing_Temp(t1, usenums);
                    if (Ts < Minshang)
                    {
                        responseStr = "CHI ";
                        responseStr += cTarget.getTileString() + " ";

                        unusedTile.clear();
                        usednum.clear();

                        unusedTile.push_back(tmp);
                        usednum.insert({ tmp, usenums });

                        ChiFlag = true;
                    }
                    else if (Ts == Minshang) {
                        responseStr = "CHI ";
                        responseStr += cTarget.getTileString() + " ";

                        if (!ChiFlag) {
                            unusedTile.clear();
                            usednum.clear();
                        }

                        unusedTile.push_back(tmp);
                        usednum.insert({ tmp, usenums });

                        ChiFlag = true;
                    }
                    hands.addHand(tmp);
                }
                hands.removeChi(cTarget, chiTarget);
                sort(hands.handTile.begin(), hands.handTile.end(), cmp());
            }


            //弃胡，无需判断
            if (quitHu(Minshang, unusedTile)) {
                return "PASS";
            }
            //吃碰杠谁更好呢？
            //杠牌不用选无用牌
            else if (GangFlag) {

            }
            else if (ChiFlag || PengFlag) {
                sort(unusedTile.begin(), unusedTile.end(), cmp());
                Mahjong playedTile;

                playedTile = get_defend_tile(unusedTile);
                responseStr += playedTile.getTileString();
                flag = true;
            }

        }
    }


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
    return currAction == BUGANG;
}

bool isGangShangKaiHua(action& currAction, bool GangFlag)
{
    return currAction == DRAW && GangFlag == true;
}

bool isNoTile() {
    int currPlayer = memory.getCurrPlayer();
    int nextPlayer = memory.getNextPosition(currPlayer);

    return memory.getTileWallNum(nextPlayer) == 0;
}