#include "ResponseInfor.h"
#include "defend.h"

#include<algorithm>
#include<iostream>
#include<climits>


string response()
{
    bool noTileFlag = isNoTile();


    //currPlayTile�����š����š�������
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

    //���Լ��Ļغ��а������¶�������Ҫ�ֱ��б𲢽�����Ӧ��
    //1.�Լ����ƣ����ڰ��ܡ����ܡ������Ŀ��ܣ�������������Լ��غϣ����������ƺ�
    //Ӧ�������б��Ƿ����������㷬������ֱ�ӵ����㷬�����㣩���������Ҫ��ֱ�ӷ��غ���
    //�������㣬���ǰ��ܻ򲹸ܺ��Ƿ����������������ڸܺ�ֱ�Ӽ��㣬�������ⶪ�ƣ�
    //�����ܺͲ��ܶ����ܼ������������ٿ��Ǵ��һ������
    //�ڴ������ʱ�����ǶԶ�������������ƽ����������飬���ڷ��ػ����
    //2.�ԼҴ򡢳���
    //ֱ�ӷ���PASS


    //���Ʊ�Ƕ��崦
    //1.�;���
    win_flag_t Winflag = 0;
    if (isHeJueZhang(currPlayTile, currAction))
    {
        Winflag |= WIN_FLAG_4TH_TILE;
    }

    if (memory.getCurrPlayer() == memory.getMyPosistion())
    {
        if (memory.getCurrAction() == DRAW)
        {
            //�Ƿ�����
            bool anGangFlag = false;
            bool buGangFlag = false;

        //���Լ��غϲ������ƣ���δ�����
        //2.���ֻش�
        //3.����
        //4.���Ͽ���
        //���ڸ��ƺ�����ǰһ���޷����ƣ��ʲ���Ҫ�ڲ��ܡ��������жϺ���
            Winflag |= WIN_FLAG_SELF_DRAWN;
            if (noTileFlag)
                Winflag |= WIN_FLAG_WALL_LAST;
            if (isGangShangKaiHua(currAction, memory.getGangFlag()))
                Winflag |= WIN_FLAG_ABOUT_KONG;


            string NowHands = hands.getFormatHandSting();
            int Nowfan = Handtiles_Point(NowHands, Winflag, currPlayTile);
            if (Nowfan >= 8)return "HU";

            //����û�г��ƣ����û����ν����Ч�ƹ�ѡ��

            //BUGANG Card1��������Card1��
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

                    //������ע������
                    responseStr = "BUGANG ";
                    responseStr += currPlayTile.getTileString();

                    //unusedTile.push_back(currPlayTile);
                    //usednum.insert({ currPlayTile, usenums });

                    Minshang = TempShang;
                }

                hands.removeBuGang(currPlayTile);
            }
            //GANG Card1��������Card1��
            else if (canAnGang())
            {

                hands.addAnGang(currPlayTile);
                string h1 = hands.getFormatHandSting();
                int usenums = 0;
                int TempShang = Handtiles_ShangTing_Temp(h1, usenums);
                if (Minshang > TempShang)
                {


                    //��������ע������
                    //����֮������֮�䲻����˴˰��ܵ���
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




            //PLAY Card1��������Card1��
             //����

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

                    //������Ȼ�ӷ������Ǵ��ڵ��ڷ��գ��˴�����
                    buGangFlag = false;
                }
                hands.addHand(tmp);
                sort(hands.handTile.begin(), hands.handTile.end(), cmp());
            }
            string temphands = hands.getFormatHandSting();
            //   int Shang_tocheck = Handtiles_ShangTing_Temp(temphands);
            


            sort(unusedTile.begin(), unusedTile.end(), cmp());


            Mahjong playedTile;

            //����˴���ζ�������ƽ׶�
            if (quitHu(Minshang,unusedTile)) {//����


                //��Shanten��<Shanten�У����������򲻸ܣ�ѡ���У�����responseStr
                //��ֹ��ȫ�Ƽ��ٺͲ��ܵ���

                responseStr = "PLAY ";
                playedTile = get_defend_tile(hands.handTile);
                responseStr += playedTile.getTileString();


            }
            else{//������

                if (!buGangFlag && !anGangFlag) {


                    //�˴������ܷ�����Ƕ��������
                    //����֮����ʱ responseStr = "PLAY ";
                    responseStr = "PLAY ";//���մ���
                    playedTile = get_defend_tile_1(unusedTile);
                    responseStr += playedTile.getTileString();
                }
            }



            flag = true;
        }
    }

    //���ҳ����ܡ����ƣ�Ҳ��ֱ��б�
    // 1.���ҳ��ƣ��������ڳ�������ƣ��������ҳ��ƺ����ǿ����������ܷ����ڳ������������ƣ���������
    // ���ڿ��ܵĳ������ڳ��������ȼ����Ƿ��� ���� ������ܺ���>=8����ֱ�ӷ���
    // �����ܺ������ⶪ�ƽ��������������Ƿ��������������������
    // ���ڿ��ܵĸܣ��ڸܺ�ֱ�Ӽ�������������Ƚϣ������ⶪ��
    // �����ܳ����ܣ�ֱ��PASS
    else
    {
        //2.�ٺ�
        //3.��������
        //4.���ܺ�

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

        //���ƻ����Ǹ��õ�ѡ��
        //�����������������������

        Minshang = INT_MAX;

        //Minshang = Handtiles_ShangTing_Temp(t1, rr);
        int Shang_tocheck = Minshang;

        //1.���ָ��ƻغ��У����Լ��޷��������ܺͣ���ִ���κβ�����ֱ��PASS
        //2.����һ����ǽΪ��ʱ���޷�������
        if (currAction != GANG && !noTileFlag) {

            int GangFlag = false;

            int PengFlag = false;

            int chiTarget = canChi();

            int ChiFlag = false;


            //���Ȼ�ȡ��ǰ����¸�¶

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

            //PENG Card1����Card1��
            if (canPeng())
            {
                int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
                int tars = memory.getFormatPosition(myP, otherP);

                hands.addPeng(currPlayTile, tars);



                sort(hands.handTile.begin(), hands.handTile.end(), cmp());
                //�����ˣ�ѡ��
                //ȱ�ٶ��������Ƶ����
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

            //CHI Card1 Card2����Card1��Card2��
            if (chiTarget) {

                hands.addChi(currPlayTile, chiTarget);

                //������
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


            //�����������ж�
            if (quitHu(Minshang, unusedTile)) {
                return "PASS";
            }
            //������˭�����أ�
            //���Ʋ���ѡ������
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
        //���õ���19�߽������0�����ƣ�Ĭ�ϲ�����
        //��δ�����ж��ֳ��Ƶ�����µĿ���
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