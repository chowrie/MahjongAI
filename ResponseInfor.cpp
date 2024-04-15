#include "ResponseInfor.h"

#include<algorithm>
#include<iostream>
#include<climits>

string response()
{
    //currPlayTile�����š����š�������
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
    if (isHeJueZhang(currPlayTile,currAction))
    {
        Winflag |= WIN_FLAG_4TH_TILE;
    }

    if (memory.getCurrPlayer() == memory.getMyPosistion())
    {
        if (memory.getCurrAction() == DRAW)
        {
            //���Լ��غϲ������ƣ���δ�����
        //2.���ֻش�
        //3.����
        //4.���Ͽ���
        //���ڸ��ƺ�����ǰһ���޷����ƣ��ʲ���Ҫ�ڲ��ܡ��������жϺ���
            Winflag |= WIN_FLAG_SELF_DRAWN;
            if (isHandSpring())
                Winflag |= WIN_FLAG_WALL_LAST;
            if (isGangShangKaiHua(currAction, memory.getGangFlag()))
                Winflag |= WIN_FLAG_ABOUT_KONG;


            string NowHands = hands.getFormatHandSting();
            int Nowfan = Handtiles_Point(NowHands, Winflag, currPlayTile);
            if (Nowfan >= 8)return "HU";



            //BUGANG Card1��������Card1��
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
            //GANG Card1��������Card1��
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
            //PLAY Card1��������Card1��
             //����
            Mahjong playedtile = Search_playtile(hands, Minshang);
            if (playedtile.getTile()>0)
            {
                responseStr = "PLAY ";
                responseStr += playedtile.getTileString();
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

        if (currAction != GANG) {//���ָ��ƻغ��У����Լ��޷��������ܺͣ���ִ���κβ�����ֱ��PASS

        //������

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

            //PENG Card1����Card1��
            if (canPeng())
            {
                int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
                int tars = memory.getFormatPosition(myP, otherP);

                hands.addPeng(currPlayTile, tars);


                int PengFlag = false;

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

