/****************************************************************************
�ܲ��ԣ�(һ)������״̬ѡ��	(��)����״̬����
	(һ)
	(1)ǰ��:1.�������������е�����  2.������ȫ��  3.������
	(2)����:1.������еľ�������  2.������ȫ��	3.���ֽ�ģ����Σ�ն�,���Σ�ն���С����

	(��)
	1.������еľ��Ż�ɿ�����	2.������ȫ��	3.���ֽ�ģ����Σ�ն�,���Σ�ն���С����

���ֽ�ģ:
	1.���ݳ���δ���ֵ���, ������ɿ�/˳/��/�� �ķ����
	2.����֪ʶ�뾭�����������biasֵ
	3.���ݶ��ָ�¶������urgentֵ
	4.ģ��:
		1)�������ݱ�Ϊ���ַ���������нϴ���ܵ������������(�� {(0,0.3), (1, 0.5), (2, 0.4)}
		2)����ÿһ�������������, ��������ܵĿ�������δ��ɴ��������������
		3)����ÿһ����������, �Ӹ���Ȩ����������Σ�ն�

����ȼ���:
	P(AAA) = Na >= 3 ? 2 + bias : 0
	P(ABC) = min{Na, Nb, Nc} + bias
	P(AA) = Na >= 2 ? 2 + bias : 0
	P(AB/AC) = min{N1,N2} + bias

��ȫ��:
	1.�մ��������
	2.���ŵ�����(��ʮ����֮����԰�ȫ)

 ****************************************************************************/

/******************************************************************
�Ż�����: 
1. turn�Ľ綨	��
2. �������ʱ�ӱ��ſ�ʼ��	��

4.19
1. �԰�ȫ��Ҳ��������->����->���ŵ�˳���
˵��:���������߼���Ӧ�����ں��ڵ���, �ʼ���ǰ�ڵ��ò��������������
2. ����ʵ��robot��ս����, �������������ϡ��, �ʲ����⿼�������׶ζ��ֵ����������: ֻҪ����������������>=3����Ϊ��ȫ��

*******************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include "defend.h"

#define INF 0x3ffffff
#define limit_turn 3	// reset(turn) <= limit_turn �����8�ֵ��ִ�
#define max_recent 8	// ʵ���Ƕ�ȡ���д������


//������Ϣ	1-�ϼң�2-�Լң�3-�¼�
int mypos0;
int lastpos1;
int oppopos2;
int nextpos3;
int positions[3] = { lastpos1, oppopos2, nextpos3 };

using namespace mahjong;

vector<pair<tile_t,double>> triplet_table;		//<���ܵ�δ֪������ϣ������>
vector<pair<tile_t, double>> straight_table;		//<���ܵ�δ֪˳����ϣ������>
vector<pair<tile_t, double>> pair_table;		//<���ܵ�δ֪������ϣ������>
vector<pair<pair<tile_t,tile_t>, double>> dazi_table;		//<���ܵ�δ֪������ϣ������>	��¼˳��������Ƽ��������

tile_risk_table_t risk_table[4];	//ÿһ���ƵĶ��ڶ���i��Σ�նȣ���ʼ����Ϊ0
double urgent_table[4];		//���ݶ������������õĽ����̶�
set<int> safe_t_table;	//��ȫ��

double sigma = 0.01;	//�������ޣ��ڱ������н��п���
vector<vector<double>> WN_poss_table;	//��ʵ���������õ�ĳһ�ֵ����������ʱ�
//�õ�����ֵ,�����Ʒ���11


int tile_get_rank(int tile)
{
	if (11 <= tile && tile <= 39)
		return tile % 10;
	return 11;
}

//��ʼ��urgent����WN_poss��,����
void Init_table()
{
	//risk_table.resize(4);
	WN_poss_table.resize(7);
	for (int i = 1; i < 4; i++)
	{
		int handNum = memory.gethandNum(i);
		switch (handNum)
		{
		case 1:
			urgent_table[i] = 2;
			break;
		case 4:
			urgent_table[i] = 1.8;
			break;
		case 7:
			urgent_table[i] = 1.2;
			break;
		default:
			urgent_table[i] = 1;
		}
	}

	//**�ϲ��ļ����޷���ȡtxt�ļ���Ϣ, ֻ����ǰ����ȥ
	//�ӵ�8�ֿ�ʼ���Ʊ�
	//�ӵ�14��֮���ǵ�14�����ݣ�
	//freopen("inWN.txt", "r", stdin);
	/*for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int wn;
			scanf("%d", &wn);
			if (wn <= 20) wn = 0;
			double p = wn / 10000;
			WN_poss_table[i].push_back(p);
		}
	}*/

	int inWN[] = {
2906, 4626, 2100, 319, 27, 7, 12, 2, 1,
3709, 4546, 1536, 178, 13, 3, 10, 4, 1,
4526, 4269, 1094, 93, 3, 4, 8, 2, 1,
5315, 3893, 729, 50, 3, 3, 6, 1, 1,
5965, 3518, 480, 27, 2, 2, 2, 3, 1,
6519, 3121, 342, 12, 0, 1, 3, 3, 1,
6991, 2787, 209, 9, 0, 2, 0, 2, 0
	};
	//�ӵ�8�ֿ�ʼ�Ĵ��
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int index = i * 9 + j;
			int wn = inWN[index];		
			if (wn <= 20) wn = 0;
			double p = (double)wn / 10000;
			WN_poss_table[i].push_back(p);
		}
	}

	//����������Ϣ
	mypos0 = memory.getMyPosistion();
	lastpos1 = memory.getLastPosition();
	oppopos2 = memory.getoppoPosition();
	nextpos3 = memory.getNextPosition();

	
	positions[0] = lastpos1;
	positions[1] = oppopos2;
	positions[2] = nextpos3;
}
//����֪ʶ�뾭�飬�޸�bias,����չ
int Special_solve(int bias, int tn, int flag)		//0-���� 1-˳�� 2-���� 3-����
{
	//�������	�ڸ����Ѿ��г�Ϊ**���ܵ������
	//����1��������ÿ��һ�����޷���˳����ɿ̸�������
	//����2��
	//����3�����ųɿ̸�������
	//����4�����Ƴɿ̸�������
	bias = 0;
	int* unplayed = memory.getUnPlayed();
	switch (flag)
	{
		//����
	case 0:
	{
		Mahjong t(tn);
		if (t.isNum())
		{
			int type = t / 10;
			int ind = tn % 10;
			if (ind == 1 || ind == 9)
				bias = 8;
			else
			{
				if (unplayed[ind - 1] == 0)
					bias += 4;
				if (unplayed[ind + 1] == 0)
					bias += 4;
			}
		}
		else if (t.isFeng() || t.isJian())
		{
			bias = 8;
		}
	
	}
	//˳��
	case 1:
	{

	}
	//����
	case 2:
	{

	}
	//����
	case 3:
	{

	}
	}
	return bias;
}
//���÷����
void Calculate_poss()
{
	//�ӵ�11λ��ʼ�洢�� �����齫��ָ���±�
	int* unplayed = memory.getUnPlayed();
	for (int in = 0; in < 34; in++)
	{
		int i = in + 11;
		if (unplayed[i] == 0) continue;
		else if (unplayed[i] == 1)
		{
			//˳�ӻ����
			int bias = 0;
			//˳��
			if (tile_get_rank(i) < 8 && unplayed[i + 1] && unplayed[i + 2])
			{
				int min = std::min(std::min(unplayed[i], unplayed[i + 1]), unplayed[i + 2]);
				bias = Special_solve(bias, i + 1, 1);
				straight_table.push_back(make_pair(all_tiles[in + 1], min + bias));
			}
			//AB���� �������˳����һ������
			if (tile_get_rank(i) < 8 && unplayed[i + 1])
			{
				int min = std::min(unplayed[i], unplayed[i + 1]);
				bias = Special_solve(bias, i+1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 2]), min + bias));
			}
			//AC����
			if (tile_get_rank(i) < 8 && unplayed[i + 2])
			{
				int min = std::min(unplayed[i], unplayed[i + 2]);
				bias = Special_solve(bias, i + 1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 1]), min + bias));
			}

		}
		else if (unplayed[i] == 2)
		{
			//���ӻ�˳�ӻ����
			int bias = 0;
			//����
			bias = Special_solve(bias, i, 2);
			pair_table.push_back(make_pair(all_tiles[in], 2 + bias));

			//˳��
			if (tile_get_rank(i) < 8 && unplayed[i + 1] && unplayed[i + 2])
			{
				int min = std::min(std::min(unplayed[i], unplayed[i + 1]), unplayed[i + 2]);
				bias = Special_solve(bias, i+1,1);
				straight_table.push_back(make_pair(all_tiles[in + 1], min + bias));
			}
			//AB���� �������˳����һ������
			if (tile_get_rank(i) < 8 && unplayed[i + 1])
			{
				int min = std::min(unplayed[i], unplayed[i + 1]);
				bias = Special_solve(bias, i+1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 2]), min + bias));
			}
			//AC����
			if (tile_get_rank(i) < 8 && unplayed[i + 2])
			{
				int min = std::min(unplayed[i], unplayed[i + 2]);
				bias = Special_solve(bias, i+1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 1]), min + bias));
			}


		}
		else if (unplayed[i] >= 3)
		{
			//���ֿ���
			int bias = 0;
			//����
			bias = Special_solve(bias, i, 0);
			triplet_table.push_back(make_pair(all_tiles[in], 2 + bias));
			//����
			bias = Special_solve(bias,i+1, 2);
			pair_table.push_back(make_pair(all_tiles[in], 2 + bias));

			//˳��
			if (tile_get_rank(i) < 8 && unplayed[i + 1] && unplayed[i + 2])
			{
				int min = std::min(std::min(unplayed[i], unplayed[i + 1]), unplayed[i + 2]);
				bias = Special_solve(bias, i+1, 1);
				straight_table.push_back(make_pair(all_tiles[in + 1], min + bias));
			}
			//AB���� �������˳����һ������
			if (tile_get_rank(i) < 8 && unplayed[i + 1])
			{
				int min = std::min(unplayed[i], unplayed[i + 1]);
				bias = Special_solve(bias,i+1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 2]), min + bias));
			}
			//AC����
			if (tile_get_rank(i) < 8 && unplayed[i + 2])
			{
				int min = std::min(unplayed[i], unplayed[i + 2]);
				bias = Special_solve(bias, i+1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 1]), min + bias));
			}
		}
	}
}

int reset(int turn)
{
	//�����������Ϊ0-6�ĵ�
	int delta = turn - 60; 
	if (delta < 0) return 0;
	if (delta >= 70) return 6;
	int layer = delta / 10;
	return layer;
}
//���ÿ��ܵ��������������
void Calculate_risk()
{
	//Init_table();
	int* unplayed = memory.getUnPlayed();
	//turn Ϊȫ�ֱ���
	int turn_ = reset(turn);
	vector<double> WN_list = WN_poss_table[turn_];
	for (int i = 0; i < 3; i ++)
	{
		int player = positions[i];
		int handnum = memory.gethandNum(player);
		tile_risk_table_t& oper = risk_table[player];
		if (handnum == 1)
		{
			//����һ�� �ض����Ʋ�������������
			Calculate_risk_table(0, 0, oper, player);
			continue;
		}
		for (int WN = 0; WN < WN_list.size(); WN++)
		{
			double p = WN_list[WN];
			if (p < sigma) continue;

			//����ĳ��ȷ����������WN
			for (int pack_cnt = 0; pack_cnt <= 4; pack_cnt++)
			{
				if (3 * pack_cnt > handnum) break;
				for (int has_pair = 0; has_pair < 2; has_pair++)
				{
					int incomplete_cnt;
					if (has_pair == 1)
					{
						//��Ԥ������Ϊ n*AAA + B��������ȸͷ
						if (3 * pack_cnt + has_pair ==  handnum) break;
						/*��ȸͷ
						WN = incomplete_cnt + 2*(4 - pack_cnt - incomplete_cnt) - 1
						WN = -incomplete_cnt + 8 - 2*pack_cnt + 1
						*/
						incomplete_cnt = 8 - 2 * pack_cnt - WN + 1;
						//���ܵı������
						if (incomplete_cnt < 0) continue;
						
					}
					else
					{
						/*��ȸͷ
						WN = incomplete_cnt + 2*(4 - pack_cnt - incomplete_cnt) 
						WN = -incomplete_cnt + 8 - 2*pack_cnt + 1
						*/
						incomplete_cnt = 8 - 2 * pack_cnt - WN + 1;
						//���ܵı������
						if (incomplete_cnt < 0) continue;
					}
					//�ѷ����������������(pack_cnt, incomplete_cnt)
					//�������������
					Calculate_risk_table(pack_cnt, incomplete_cnt, oper, player);					

				}
			}
		}
	}
}
//�������Ҿ�����ƣ�����Σ�նȱ�
//���ݷ���������ʼ���Ļ���pack_cnt��incomplete_cntֻ��Ӱ��
void Calculate_risk_table(int pack_cnt, int incomplete_cnt, tile_risk_table_t& oper, int player)
{
	//���ݸ��˳���΢����	����˵�����ݶ��ֵ���������ģ��ֵ��/��¶������ʵֵ�����ý���Ȩ�أ����伴��������Ȩ������

	double total_tri_p = 0, total_straight_p = 0, total_pair_p = 0, total_dazi_p = 0;

	if (1)
	{
		for (int i = 0; i < triplet_table.size(); i++)
		{
			total_tri_p += triplet_table[i].second;
		}
		for (int i = 0; i < straight_table.size(); i++)
		{
			total_straight_p += straight_table[i].second;
		}
		for (int i = 0; i < pair_table.size(); i++)
		{
			total_pair_p += pair_table[i].second;

		}
		for (int i = 0; i < dazi_table.size(); i++)
		{
			total_dazi_p += dazi_table[i].second;
		}
	}

	if (total_tri_p > 0)
	{
		//���Ӵ��ڸܿ���
		for (int i = 0; i < triplet_table.size(); i++)
		{
			tile_t t = triplet_table[i].first;
			double p = triplet_table[i].second / total_tri_p;
			oper[t] += p;
		}
	}
	if (total_straight_p > 0)
	{
		//˳�Ӳ�Ӱ��
		//�������ص�����˳Ҳ����˳�Ӵ��Ӵ���
	}
	if (total_pair_p > 0)
	{
		//���ӳɿ�
		for (int i = 0; i < pair_table.size(); i++)
		{
			tile_t t = pair_table[i].first;
			double p = pair_table[i].second / total_pair_p;
			oper[t] += p;
		}
	}
	if (total_tri_p > 0)
	{
		//����
		for (int i = 0; i < dazi_table.size(); i++)
		{
			tile_t t = dazi_table[i].first.second;
			double p = dazi_table[i].second / total_dazi_p;
			oper[t] += p;
		}
	}
}


int Get_safe_tile(vector<Mahjong>& handTile)
{
	vector<Mahjong>& played = memory.getPlayed();
	for (int i = played.size() - 1; i >= 0; i--)
	{
		Mahjong rplayed = played[i];
		safe_t_table.insert(rplayed);
	}
	//	11-19
	//	21-29
	//	31-39
	int safe_t = -1;
	int isZi = 0;
	int max_to_5 = -1;
	for (int i = 0; i < handTile.size(); i++)
	{
		Mahjong tile = handTile[i];
		auto it = safe_t_table.find(tile);
		if (it != safe_t_table.end())
		{
			if (tile.isFeng() || tile.isHana() || tile.isJian())
			{
				isZi = 1;
				if (tile.getTile() > safe_t)
					safe_t = tile;
			}
			else if (!isZi)
			{
				//����������
				int num = tile.getNum();
				int delta = num >= 5 ? num - 5 : 5 - num;
				if (delta > max_to_5)
				{
					max_to_5 = delta;
					safe_t = tile;
				}
			}
		}

	}
	
	return safe_t;
}
//���������Ʋ���
int get_defend_tile_1(vector<Mahjong>& useless)
{
	if (useless.size() == 1) return useless[0].getTile();
	//��ϴ������
	vector<Mahjong> tmp = useless;
	for (int i = 0; i < tmp.size(); i++)
	{
		Mahjong tile = tmp[i];
		if (!tile.isNum() && memory.getCntHand(tile) >= 2)
		{
			tmp.erase(tmp.begin() + i);
			i--;
		}
	}
	if (tmp.size() != 0)	useless = tmp;
	//����:
	//�ƾ�ǰ��: ���ջ��� - ���� - ���ƵĴ�����
	//�ƾֺ���: ������ʮ�������༫�͸����¼�(��Ҫ������������Ӷ��ƺ�,��ʣ���۾�������),�����ƽ�ʣ������һ��ʱ���
	//�ҵ����ϴ��ڵ�����������
	set<int> Hana;
	set<int> Jian;
	set<int> Feng;
	set<int> Num;
	if (1)
	{
		int tile = -1;
		for (int i = 0; i < useless.size(); i++)
		{
			Mahjong t = useless[i];
			if (t.isHana())
				Hana.insert(t.getTile());
			else if (t.isJian())
				Jian.insert(t.getTile());
			else if (t.isFeng())
				Feng.insert(t.getTile());
			else
				Num.insert(t.getTile());
		}
		
	}
	
	//�ж��ƾ�ʱ�����
	int turn_ = reset(turn);
	//ǰ��:
	if (turn_ <= limit_turn)
	{
		//������, ������
		int tile = -1;
		if (!Feng.empty())	tile = *Feng.begin();
		if (!Jian.empty())	tile = *Jian.begin();
		if (!Hana.empty())	tile = *Hana.begin();
		if (tile != -1) return tile;

		//������
		//1.�Ұ�ȫ��
		int safe_t = Get_safe_tile(useless);
		if (safe_t > 0) return safe_t;

		//2.����Σ�ն�(ǰ����Ϣ����,Σ�ն���ϲ�)

		//3.�����Ʊ��ſ�ʼ���ڴ�
		int max = -1;	//��������5�ľ���
		int index = -1;
		for (int i = 0; i < useless.size(); i++)
		{
			Mahjong t = useless[i];
			int num = t.getNum();
			int delta = num >= 5 ? num - 5 : 5 - num;
			if (delta > max)
			{
				max = delta;
				index = i;
			}
		}
		if (index == -1) index = 0;
		return useless[index];

	}
	//����:
	else
	{
		int* unplayed = memory.getUnPlayed();
		//map<Mahjong, int> cnt_hand = memory.getCntHand();
		
		//1.��������е������Ƿ��Ǿ���
		if (!Feng.empty() || !Jian.empty() || !Hana.empty())
		{
			for (const auto& it : Feng)
			{
				Mahjong tile = Mahjong(it);
				if (unplayed[tile] == 1)
					return tile;
			}
			for (const auto& it : Jian)
			{
				Mahjong tile = Mahjong(it);
				if (unplayed[tile] == 1)
					return tile;
			}
			for (const auto& it : Hana)
			{
				Mahjong tile = Mahjong(it);
				if (unplayed[tile] == 1)
					return tile;
			}
			
		}

		//2.�Ұ�ȫ��
		
		int safe_t = Get_safe_tile(useless);
		if (safe_t > 0) return safe_t;


		//3.����Σ�ն�
		Init_table();
		Calculate_poss();
		Calculate_risk();
		double min_p = INF;
		int defend_t = -1;
		for (int i = 0; i < useless.size(); i++)
		{
			int t = useless[i].getTile() - 11;
			double p = risk_table[1][t] * urgent_table[i] + risk_table[2][t] * urgent_table[i] + risk_table[3][t] * urgent_table[i];
			if (p < min_p)
			{
				min_p = p;
				defend_t = t;
			}
		}
		return defend_t + 11;
	}
}
//���ýӿ�
int get_defend_tile(vector<Mahjong>& handTile)
{
 //�������ֲ���:	1.������״̬��, ����������(��֤����������ȫ�������Ƶ����)	2. ����״̬��, ��������

	//1.������״̬
	if (handTile.size() != memory.getHandTile().size())
	{
		if (handTile.size() == 1) return handTile[0].getTile();
		return get_defend_tile_1(handTile);
	}

	//2.����״̬
	 
	//1)����Ƿ��а�ȫ��
	//���Ȱ�ȫ���б�:	���������>= 3�����ƺ� ���ƶ��������е�����(�ֱ������ >= 3�Ͷ��ֳ��������)
	for (int i = 0; i < handTile.size(); i++)
	{
		Mahjong tile = handTile[i];
		if (!tile.isNum())
		{
			int cnt = memory.getCntHand(tile);
			if (cnt >= 3)
				return tile;
		}
	}


	int safe_t = Get_safe_tile(handTile);
	if (safe_t > 0) return safe_t;

	//2)����Σ�ն�
	Init_table();
	Calculate_poss();
	Calculate_risk();
	double min_p = INF;
	int defend_t = -1;
	for (int i = 0; i < handTile.size(); i++)
	{
		int t = handTile[i].getTile() - 11;
		double p = risk_table[lastpos1][t]*urgent_table[lastpos1] + risk_table[oppopos2][t]*urgent_table[oppopos2] + risk_table[nextpos3][t]*urgent_table[nextpos3];
		if (p < min_p)
		{
			min_p = p;
			defend_t = t;
		}
	}

	return defend_t + 11;
}