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
    string NowHands = memory.getFormatHandSting();
    Hand_Claim hands;


    if (turn == 0 || turn == 1)return "PASS";
    string responseStr = "";
    bool flag = false;


    //{������Ҫɾ���Ĳ��֣�����Ӧ�����б�Ŀǰ�غϵ�״̬�پ����Ƿ�����������Լ�����

   

    //int NowFan = Handtiles_Point(NowHands,SELFDRAWN);
    //if (NowFan != -3&&NowFan>=8)return "HU";

    
    //int NowShang = Handtiles_ShangTing();

    


    //}

    Mahjong card1, card2;

    //���Լ��Ļغ��а������¶�������Ҫ�ֱ��б𲢽�����Ӧ��
    //1.�Լ����ƣ����ڰ��ܡ����ܡ������Ŀ��ܣ�������������Լ��غϣ����������ƺ�
    //Ӧ�������б��Ƿ����������㷬������ֱ�ӵ����㷬�����㣩���������Ҫ��ֱ�ӷ��غ���
    //�������㣬���ǰ��ܻ򲹸ܺ��Ƿ����������������ڸܺ�ֱ�Ӽ��㣬�������ⶪ�ƣ�
    //�����ܺͲ��ܶ����ܼ������������ٿ��Ǵ��һ������
    //�ڴ������ʱ�����ǶԶ�������������ƽ����������飬���ڷ��ػ����
    //2.�ԼҴ򡢳���
    //ֱ�ӷ���PASS
    if (memory.getCurrPlayer() == memory.getMyPosistion())
    {
        if (memory.getCurrAction() == DRAW) 
        {//���Լ��غϲ������ƣ���δ�����

            int NowFan = Handtiles_Point(NowHands,SELFDRAWN);//����
            if (NowFan != -3&&NowFan>=8)return "HU";

            int MinShang = INT_MAX;

            //BUGANG Card1��������Card1��
            if (canBuGang()) 
            {

                hands.addBuGang(currPlayTile);
                string h1 = hands.getFormatHandSting();
                int TempShang = Handtiles_ShangTing_Temp(h1);
                if (MinShang > TempShang)
                {
                    responseStr = "BUGANG ";
                    responseStr += currPlayTile.getTileString();

                    MinShang = TempShang;
                }
                hands.removeBuGang(currPlayTile);
            }
            //GANG Card1��������Card1��
            else if (canAnGang()) 
            {
                hands.addAnGang(currPlayTile);
                string h1 = hands.getFormatHandSting();
                int TempShang = Handtiles_ShangTing_Temp(h1);
                if (MinShang > TempShang)
                {
                    responseStr = "GANG ";
                    responseStr += currPlayTile.getTileString();

                    MinShang = TempShang;
                }
                hands.removeAnGang(currPlayTile);
            }

            int tempMinShang = MinShang;

            //PLAY Card1��������Card1��
            // //����
            int len = hands.handTile.size();
            int initShang = Handtiles_ShangTing();
            int perfectlo = 0;
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
                    responseStr = "PLAY ";
                    perfectlo = i;
                    MinShang = Ts;
                    
                }
                hands.addHand(tmp);
            }
            if (MinShang != std::numeric_limits<int>::max() && MinShang < tempMinShang)
            {
                responseStr += hands.handTile[perfectlo].getTileString();
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
        int initShang = Handtiles_ShangTing();

        int chiTarget = canChi();
        //GANG
        if (canMinGang())
        {
            int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
            int tars = memory.getFormatPosition(myP, otherP);

            hands.addMinGang(currPlayTile, tars);
            string t1 = hands.getFormatHandSting();
            int Ts = Handtiles_ShangTing_Temp(t1);
            if (initShang > Ts)
            {
                responseStr = "GANG";

                flag = true;

                initShang = Ts;
            }
            hands.removeMinGang(currPlayTile);
            
        }

        //PENG Card1����Card1��
        if (canPeng()) 
        {
            int myP = memory.getMyPosistion(), otherP = memory.getCurrPlayer();
            int tars = memory.getFormatPosition(myP, otherP);

            hands.addPeng(currPlayTile, tars);

            int NowFan = Handtiles_Point(NowHands, DISCARD);//���ƣ�ȱ�ж����� �������Ȱ���ͨ

            //��Կ�����󷬼�ȥ

            if (NowFan != -3 && NowFan >= 8)return "HU";//����8�����

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

                if (Ts < initShang)
                {
                    responseStr = "PENG ";
                    perfectlo = i;
                    initShang = Ts;

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

            string t1 = hands.getFormatHandSting();

            int NowFan = Handtiles_Point(NowHands, DISCARD);//���ƣ�ȱ�ж����� �������Ȱ���ͨ
            if (NowFan != -3 && NowFan >= 8)return "HU";//����8�����

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
                if (Ts < initShang)
                {
                    responseStr = "CHI ";
                    responseStr += cTarget.getTileString() + " ";
                    perfectlo = i;
                    initShang = Ts;

                    ChiFlag = true;
                }
                hands.addHand(tmp);
            }
            if (ChiFlag)
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
    //���õ���19�߽������0�����ƣ�Ĭ�ϲ�����
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
