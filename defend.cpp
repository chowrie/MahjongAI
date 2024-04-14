///****************************************************************************
//�ܲ��ԣ����Ʒ����
// ****************************************************************************/
//#include "StatusMemory.h"
//#include "shanten.h"
//#include <set>
//
//
//#include <map>
//#include <algorithm>
//
//#define INF 0x3ffffff
//typedef double tile_risk_table_t[TILE_TABLE_SIZE];
//
//using namespace mahjong;
//
//vector<pair<tile_t,double>> triplet_table;		//<���ܵ�δ֪������ϣ������>
//vector<pair<tile_t, double>> straight_table;		//<���ܵ�δ֪˳����ϣ������>
//vector<pair<tile_t, double>> pair_table;		//<���ܵ�δ֪������ϣ������>
//vector<pair<pair<tile_t,tile_t>, double>> dazi_table;		//<���ܵ�δ֪������ϣ������>	��¼˳��������Ƽ��������
//tile_risk_table_t risk_table[4];	//ÿһ���ƵĶ��ڶ���i��Σ�նȣ���ʼ����Ϊ0��1-�ϼң�2-�Լң�3-�¼�
//double urgent_table[4];		//�������������õĽ����̶�
//set<int> safe_t_table;	//��ȫ��
//
//double sigma = 0.01;	//�������ޣ��ڱ������н��п���
//vector<vector<double>> WN_poss_table;	//��ʵ���������õ�ĳһ�ֵ����������ʱ�
//int tile_get_rank(int tile)
//{
//	if (11 <= tile && tile <= 39)
//		return tile % 10;
//	return 11;
//}
//
////��ʼ��urgent����WN_poss��
//void Init_table()
//{
//	//risk_table.resize(4);
//	for (int i = 1; i < 4; i++)
//	{
//		int handNum = memory.gethandNum(i);
//		switch (handNum)
//		{
//		case 1:
//			urgent_table[i] = 2;
//			break;
//		case 4:
//			urgent_table[i] = 1.8;
//			break;
//		case 7:
//			urgent_table[i] = 1.2;
//			break;
//		default:
//			urgent_table[i] = 1;
//		}
//	}
//
//	//�ӵ�8�ֿ�ʼ���Ʊ�
//	//�ӵ�14��֮���ǵ�14�����ݣ�
//	freopen("inWN.txt", "r", stdin);
//	for (int i = 0; i < 7; i++)
//	{
//		for (int j = 0; j < 9; j++)
//		{
//			int wn;
//			scanf("%d", &wn);
//			if (wn <= 20) wn = 0;
//			double p = wn / 10000;
//			WN_poss_table[i].push_back(p);
//		}
//	}
//}
////����֪ʶ�뾭�飬�޸�bias��δ���
//int Special_solve(int bias, int tn, int flag)		//0-���� 1-˳�� 2-���� 3-����
//{
//	//�������	�ڸ����Ѿ��г�Ϊ**���ܵ������
//	//����1��������ÿ��һ�����޷���˳����ɿ̸�������
//	//����2����������
//	//����3�����ųɿ̸�������
//	//����4�����Ƴɿ̸�������
//	bias = 0;
//	int* unplayed = memory.getUnPlayed();
//	switch (flag)
//	{
//	case 0:
//	{
//		Mahjong t(tn);
//		if (t.isNum())
//		{
//			int type = t / 10;
//			int ind = tn % 10;
//			if (ind == 1 || ind == 9)
//				bias = 8;
//			else
//			{
//				if (unplayed[ind - 1] == 0)
//					bias += 4;
//				if (unplayed[ind + 1] == 0)
//					bias += 4;
//			}
//		}
//		else if (t.isFeng() || t.isJian())
//		{
//			bias = 8;
//		}
//	
//	}
//	case 1:
//	{
//
//	}
//	case 2:
//	{
//
//	}
//	case 3:
//	{
//
//	}
//	}
//	return bias;
//}
////���÷����
//void Calculate_poss()
//{
//	//�ӵ�11λ��ʼ�洢�� �����齫��ָ���±�
//	int* unplayed = memory.getUnPlayed();
//	for (int in = 0; in < 34; in++)
//	{
//		int i = in + 11;
//		if (unplayed[i] == 0) continue;
//		else if (unplayed[i] == 1)
//		{
//			//˳�ӻ����
//			
//			//˳��
//			if (tile_get_rank(i) < 8 && unplayed[i + 1] && unplayed[i + 2])
//			{
//				int min = std::min(unplayed[i], unplayed[i + 1], unplayed[i + 2]);
//				int bias = Special_solve(bias, i + 1, 1);
//				straight_table.push_back(make_pair(all_tiles[in + 1], min + bias));
//			}
//			//AB���� �������˳����һ������
//			if (tile_get_rank(i) < 8 && unplayed[i + 1])
//			{
//				int min = std::min(unplayed[i], unplayed[i + 1]);
//				int bias = Special_solve(bias, i+1, 3);
//				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 2]), min + bias));
//			}
//			//AC����
//			if (tile_get_rank(i) < 8 && unplayed[i + 2])
//			{
//				int min = std::min(unplayed[i], unplayed[i + 2]);
//				int bias = Special_solve(bias, i + 1, 3);
//				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 1]), min + bias));
//			}
//
//		}
//		else if (unplayed[i] == 2)
//		{
//			//���ӻ�˳�ӻ����
//			//����
//			int bias = Special_solve(bias, i, 2);
//			pair_table.push_back(make_pair(all_tiles[in], 2 + bias));
//
//			//˳��
//			if (tile_get_rank(i) < 8 && unplayed[i + 1] && unplayed[i + 2])
//			{
//				int min = std::min(unplayed[i], unplayed[i + 1], unplayed[i + 2]);
//				int bias = Special_solve(bias, i+1,1);
//				straight_table.push_back(make_pair(all_tiles[in + 1], min + bias));
//			}
//			//AB���� �������˳����һ������
//			if (tile_get_rank(i) < 8 && unplayed[i + 1])
//			{
//				int min = std::min(unplayed[i], unplayed[i + 1]);
//				int bias = Special_solve(bias, i+1, 3);
//				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 2]), min + bias));
//			}
//			//AC����
//			if (tile_get_rank(i) < 8 && unplayed[i + 2])
//			{
//				int min = std::min(unplayed[i], unplayed[i + 2]);
//				int bias = Special_solve(bias, i+1, 3);
//				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 1]), min + bias));
//			}
//
//
//		}
//		else if (unplayed[i] >= 3)
//		{
//			//���ֿ���
//			//����
//			int bias = Special_solve(bias, i, 0);
//			triplet_table.push_back(make_pair(all_tiles[in], 2 + bias));
//			//����
//			int bias = Special_solve(bias,i+1, 2);
//			pair_table.push_back(make_pair(all_tiles[in], 2 + bias));
//
//			//˳��
//			if (tile_get_rank(i) < 8 && unplayed[i + 1] && unplayed[i + 2])
//			{
//				int min = std::min(unplayed[i], unplayed[i + 1], unplayed[i + 2]);
//				int bias = Special_solve(bias, i+1, 1);
//				straight_table.push_back(make_pair(all_tiles[in + 1], min + bias));
//			}
//			//AB���� �������˳����һ������
//			if (tile_get_rank(i) < 8 && unplayed[i + 1])
//			{
//				int min = std::min(unplayed[i], unplayed[i + 1]);
//				int bias = Special_solve(bias,i+1, 3);
//				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 2]), min + bias));
//			}
//			//AC����
//			if (tile_get_rank(i) < 8 && unplayed[i + 2])
//			{
//				int min = std::min(unplayed[i], unplayed[i + 2]);
//				int bias = Special_solve(bias, i+1, 3);
//				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 1]), min + bias));
//			}
//		}
//	}
//}
////���ÿ��ܵ��������������
//void Calculate_risk()
//{
//	Init_table();
//	int* unplayed = memory.getUnPlayed();
//	//���Լ���ǽʣ����ģ��غ���
//	int turn = 21 - memory.getTileWall(0);
//	vector<double> WN_list = WN_poss_table[turn];
//	for (int player = 1; player < 4; player++)
//	{
//		int handnum = memory.gethandNum(player);
//		tile_table_t& oper = risk_table[0];
//		if (handnum == 1)
//		{
//			//����һ�� �ض����Ʋ�������������
//
//		}
//		for (int WN = 0; WN < WN_list.size(); WN++)
//		{
//			double p = WN_list[WN];
//			if (p < sigma) continue;
//
//			//����ĳ��ȷ����������WN
//			for (int pack_cnt = 0; pack_cnt <= 4; pack_cnt++)
//			{
//				if (3 * pack_cnt > handnum) break;
//				for (int has_pair = 0; has_pair < 2; has_pair++)
//				{
//					int incomplete_cnt;
//					if (has_pair == 1)
//					{
//						//��Ԥ������Ϊ n*AAA + B��������ȸͷ
//						if (3 * pack_cnt + has_pair ==  handnum) break;
//						/*��ȸͷ
//						WN = incomplete_cnt + 2*(4 - pack_cnt - incomplete_cnt) - 1
//						WN = -incomplete_cnt + 8 - 2*pack_cnt + 1
//						*/
//						incomplete_cnt = 8 - 2 * pack_cnt - WN + 1;
//						//���ܵı������
//						if (incomplete_cnt < 0) continue;
//						
//					}
//					else
//					{
//						/*��ȸͷ
//						WN = incomplete_cnt + 2*(4 - pack_cnt - incomplete_cnt) 
//						WN = -incomplete_cnt + 8 - 2*pack_cnt + 1
//						*/
//						incomplete_cnt = 8 - 2 * pack_cnt - WN + 1;
//						//���ܵı������
//						if (incomplete_cnt < 0) continue;
//					}
//					//�ѷ����������������(pack_cnt, incomplete_cnt)
//					//�������������
//
//						
//
//				}
//			}
//		}
//	}
//}
////�������Ҿ�����ƣ�����Σ�նȱ�
////���ݷ���������ʼ���Ļ���pack_cnt��incomplete_cntֻ��Ӱ��
//void Calculate_risk_table(int pack_cnt, int incomplete_cnt, tile_table_t& oper, int player)
//{
//	//���ݸ��˳���΢����	����˵�����ݶ��ֵ���������ģ��ֵ��/��¶������ʵֵ�����ý���Ȩ�أ����伴��������Ȩ������
//	
//	double total_tri_p, total_straight_p, total_pair_p, total_dazi_p;
//	if (1)
//	{
//		for (int i = 0; i < triplet_table.size(); i++)
//		{
//			total_tri_p += triplet_table[i].second;
//		}
//		for (int i = 0; i < straight_table.size(); i++)
//		{
//			total_straight_p += straight_table[i].second;
//		}
//		for (int i = 0; i < pair_table.size(); i++)
//		{
//			total_pair_p += pair_table[i].second;
//
//		}
//		for (int i = 0; i < dazi_table.size(); i++)
//		{
//			total_dazi_p += dazi_table[i].second;
//		}
//	}
//	
//	if (total_tri_p > 0)
//	{
//		//���Ӵ��ڸܿ���
//		for (int i = 0; i < triplet_table.size(); i++)
//		{
//			tile_t t = triplet_table[i].first;
//			double p = triplet_table[i].second/total_tri_p;
//			oper[t] += p;
//		}
//	}
//	if (total_straight_p > 0)
//	{
//		//˳�Ӳ�Ӱ��
//		//�������ص�����˳Ҳ����˳�Ӵ��Ӵ���
//	}
//	if (total_pair_p > 0)
//	{
//		//���ӳɿ�
//		for (int i = 0; i < pair_table.size(); i++)
//		{
//			tile_t t = pair_table[i].first;
//			double p = pair_table[i].second / total_pair_p;
//			oper[t] += p;
//		}
//	}
//	if (total_tri_p > 0)
//	{
//		//����
//		for (int i = 0; i < dazi_table.size(); i++)
//		{
//			tile_t t = dazi_table[i].first.second;
//			double p = dazi_table[i].second / total_dazi_p;
//			oper[t] += p;
//		}
//	}
//}
//
//int Get_safe_tile(vector<Mahjong>& handTile)
//{
//	//
//	vector<Mahjong>& played = memory.getPlayed();
//	for (int i = played.size() - 1; i >= 0; i--)
//	{
//		Mahjong rplayed = played[i];
//		safe_t_table.insert(rplayed);
//	}
//	for (int i = 0; i < handTile.size(); i++)
//	{
//		Mahjong tile = handTile[i];
//		auto it = safe_t_table.find(tile);
//		if (it != safe_t_table.end())
//			return tile;
//	}
//	return -1;
//}
////���ýӿ�
//int get_defend_tile(vector<Mahjong>& handTile)
//{
//	//����Ƿ��а�ȫ�ƣ��ް�ȫ�Ʋż���Σ�ն�
//	int safe_t = Get_safe_tile(handTile);
//	if (safe_t > 0) return safe_t;
//	Init_table();
//	Calculate_poss();
//	Calculate_risk();
//	double min_p = INF;
//	int defend_t = -1;
//	for (int i = 0; i < handTile.size(); i++)
//	{
//		int t = handTile[i].getNum() - 11;
//		double p = risk_table[1][t]*urgent_table[i] + risk_table[2][t]*urgent_table[i] + risk_table[3][t]*urgent_table[i];
//		if (p < min_p)
//		{
//			min_p = p;
//			defend_t = t;
//		}
//	}
//
//	return defend_t + 11;
//}