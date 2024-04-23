/****************************************************************************
总策略：(一)非弃胡状态选择	(二)弃胡状态防守
	(一)
	(1)前期:1.按花箭风打出所有的字牌  2.搜索安全牌  3.任意打出
	(2)后期:1.打出所有的绝张字牌  2.搜索安全牌	3.对手建模计算危险度,打出危险度最小的牌

	(二)
	1.打出所有的绝张或成刻字牌	2.搜索安全牌	3.对手建模计算危险度,打出危险度最小的牌

对手建模:
	1.依据场上未出现的牌, 计算其成刻/顺/搭/对 的分配度
	2.依据知识与经验修正分配度bias值
	3.依据对手副露数修正urgent值
	4.模拟:
		1)按照数据表为对手分配该轮所有较大可能的上听数与概率(如 {(0,0.3), (1, 0.5), (2, 0.4)}
		2)对于每一个分配的上听数, 分配其可能的刻子数与未完成搭子数的所有情况
		3)对于每一个分配的情况, 加概率权计算修正牌危险度

分配度计算:
	P(AAA) = Na >= 3 ? 2 + bias : 0
	P(ABC) = min{Na, Nb, Nc} + bias
	P(AA) = Na >= 2 ? 2 + bias : 0
	P(AB/AC) = min{N1,N2} + bias

安全牌:
	1.刚打出来的牌
	2.绝张的字牌(除十三幺之外绝对安全)

 ****************************************************************************/

/******************************************************************
优化事宜: 
1. turn的界定	√
2. 任意出牌时从边张开始出	√

4.19
1. 对安全牌也按照字牌->边张->里张的顺序出
说明:由于弃胡逻辑上应该是在后期调用, 故即便前期调用不会立即打出字牌
2. 依据实际robot对战经验, 对手听字牌情况稀少, 故不额外考虑弃胡阶段对手单听字牌情况: 只要字牌在手牌中数量>=3可视为安全牌

*******************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include "defend.h"

#define INF 0x3ffffff
#define limit_turn 3	// reset(turn) <= limit_turn 相较于8轮的轮次
#define max_recent 8	// 实际是读取所有打出的牌


//座次信息	1-上家，2-对家，3-下家
int mypos0;
int lastpos1;
int oppopos2;
int nextpos3;
int positions[3] = { lastpos1, oppopos2, nextpos3 };

using namespace mahjong;

vector<pair<tile_t,double>> triplet_table;		//<可能的未知刻子组合，分配度>
vector<pair<tile_t, double>> straight_table;		//<可能的未知顺子组合，分配度>
vector<pair<tile_t, double>> pair_table;		//<可能的未知对子组合，分配度>
vector<pair<pair<tile_t,tile_t>, double>> dazi_table;		//<可能的未知搭子组合，分配度>	记录顺子最左边牌及所差的牌

tile_risk_table_t risk_table[4];	//每一张牌的对于对手i的危险度，初始化都为0
double urgent_table[4];		//依据对手立牌数设置的紧急程度
set<int> safe_t_table;	//安全牌

double sigma = 0.01;	//概率下限，在表输入中进行控制
vector<vector<double>> WN_poss_table;	//由实验数据所得的某一轮的上听数概率表
//得到数牌值,非数牌返回11


int tile_get_rank(int tile)
{
	if (11 <= tile && tile <= 39)
		return tile % 10;
	return 11;
}

//初始化urgent表与WN_poss表,座次
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

	//**合并文件后无法读取txt文件信息, 只能提前打表进去
	//从第8轮开始复制表
	//从第14轮之后都是第14轮数据？
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
	//从第8轮开始的打表
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

	//更新座次信息
	mypos0 = memory.getMyPosistion();
	lastpos1 = memory.getLastPosition();
	oppopos2 = memory.getoppoPosition();
	nextpos3 = memory.getNextPosition();

	
	positions[0] = lastpos1;
	positions[1] = oppopos2;
	positions[2] = nextpos3;
}
//依据知识与经验，修改bias,可扩展
int Special_solve(int bias, int tn, int flag)		//0-刻子 1-顺子 2-对子 3-搭子
{
	//经验策略	在该牌已经有成为**可能的情况下
	//策略1：若该牌每有一单边无法成顺，则成刻概率增大
	//策略2：
	//策略3：边张成刻概率增大
	//策略4：字牌成刻概率增大
	bias = 0;
	int* unplayed = memory.getUnPlayed();
	switch (flag)
	{
		//刻子
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
	//顺子
	case 1:
	{

	}
	//对子
	case 2:
	{

	}
	//搭子
	case 3:
	{

	}
	}
	return bias;
}
//设置分配度
void Calculate_poss()
{
	//从第11位开始存储， 可用麻将类指定下标
	int* unplayed = memory.getUnPlayed();
	for (int in = 0; in < 34; in++)
	{
		int i = in + 11;
		if (unplayed[i] == 0) continue;
		else if (unplayed[i] == 1)
		{
			//顺子或搭子
			int bias = 0;
			//顺子
			if (tile_get_rank(i) < 8 && unplayed[i + 1] && unplayed[i + 2])
			{
				int min = std::min(std::min(unplayed[i], unplayed[i + 1]), unplayed[i + 2]);
				bias = Special_solve(bias, i + 1, 1);
				straight_table.push_back(make_pair(all_tiles[in + 1], min + bias));
			}
			//AB搭子 或包含在顺子中一起讨论
			if (tile_get_rank(i) < 8 && unplayed[i + 1])
			{
				int min = std::min(unplayed[i], unplayed[i + 1]);
				bias = Special_solve(bias, i+1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 2]), min + bias));
			}
			//AC搭子
			if (tile_get_rank(i) < 8 && unplayed[i + 2])
			{
				int min = std::min(unplayed[i], unplayed[i + 2]);
				bias = Special_solve(bias, i + 1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 1]), min + bias));
			}

		}
		else if (unplayed[i] == 2)
		{
			//对子或顺子或搭子
			int bias = 0;
			//对子
			bias = Special_solve(bias, i, 2);
			pair_table.push_back(make_pair(all_tiles[in], 2 + bias));

			//顺子
			if (tile_get_rank(i) < 8 && unplayed[i + 1] && unplayed[i + 2])
			{
				int min = std::min(std::min(unplayed[i], unplayed[i + 1]), unplayed[i + 2]);
				bias = Special_solve(bias, i+1,1);
				straight_table.push_back(make_pair(all_tiles[in + 1], min + bias));
			}
			//AB搭子 或包含在顺子中一起讨论
			if (tile_get_rank(i) < 8 && unplayed[i + 1])
			{
				int min = std::min(unplayed[i], unplayed[i + 1]);
				bias = Special_solve(bias, i+1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 2]), min + bias));
			}
			//AC搭子
			if (tile_get_rank(i) < 8 && unplayed[i + 2])
			{
				int min = std::min(unplayed[i], unplayed[i + 2]);
				bias = Special_solve(bias, i+1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 1]), min + bias));
			}


		}
		else if (unplayed[i] >= 3)
		{
			//四种可能
			int bias = 0;
			//刻子
			bias = Special_solve(bias, i, 0);
			triplet_table.push_back(make_pair(all_tiles[in], 2 + bias));
			//对子
			bias = Special_solve(bias,i+1, 2);
			pair_table.push_back(make_pair(all_tiles[in], 2 + bias));

			//顺子
			if (tile_get_rank(i) < 8 && unplayed[i + 1] && unplayed[i + 2])
			{
				int min = std::min(std::min(unplayed[i], unplayed[i + 1]), unplayed[i + 2]);
				bias = Special_solve(bias, i+1, 1);
				straight_table.push_back(make_pair(all_tiles[in + 1], min + bias));
			}
			//AB搭子 或包含在顺子中一起讨论
			if (tile_get_rank(i) < 8 && unplayed[i + 1])
			{
				int min = std::min(unplayed[i], unplayed[i + 1]);
				bias = Special_solve(bias,i+1, 3);
				dazi_table.push_back(make_pair(make_pair(all_tiles[in + 1], all_tiles[in + 2]), min + bias));
			}
			//AC搭子
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
	//按照情况设置为0-6的档
	int delta = turn - 60; 
	if (delta < 0) return 0;
	if (delta >= 70) return 6;
	int layer = delta / 10;
	return layer;
}
//设置可能的面子数与搭子数
void Calculate_risk()
{
	//Init_table();
	int* unplayed = memory.getUnPlayed();
	//turn 为全局变量
	int turn_ = reset(turn);
	vector<double> WN_list = WN_poss_table[turn_];
	for (int i = 0; i < 3; i ++)
	{
		int player = positions[i];
		int handnum = memory.gethandNum(player);
		tile_risk_table_t& oper = risk_table[player];
		if (handnum == 1)
		{
			//单吊一张 必定听牌不作上听数讨论
			Calculate_risk_table(0, 0, oper, player);
			continue;
		}
		for (int WN = 0; WN < WN_list.size(); WN++)
		{
			double p = WN_list[WN];
			if (p < sigma) continue;

			//对于某个确定的上听数WN
			for (int pack_cnt = 0; pack_cnt <= 4; pack_cnt++)
			{
				if (3 * pack_cnt > handnum) break;
				for (int has_pair = 0; has_pair < 2; has_pair++)
				{
					int incomplete_cnt;
					if (has_pair == 1)
					{
						//若预估牌型为 n*AAA + B，不会有雀头
						if (3 * pack_cnt + has_pair ==  handnum) break;
						/*有雀头
						WN = incomplete_cnt + 2*(4 - pack_cnt - incomplete_cnt) - 1
						WN = -incomplete_cnt + 8 - 2*pack_cnt + 1
						*/
						incomplete_cnt = 8 - 2 * pack_cnt - WN + 1;
						//可能的报错出口
						if (incomplete_cnt < 0) continue;
						
					}
					else
					{
						/*无雀头
						WN = incomplete_cnt + 2*(4 - pack_cnt - incomplete_cnt) 
						WN = -incomplete_cnt + 8 - 2*pack_cnt + 1
						*/
						incomplete_cnt = 8 - 2 * pack_cnt - WN + 1;
						//可能的报错出口
						if (incomplete_cnt < 0) continue;
					}
					//已分配面子数与搭子数(pack_cnt, incomplete_cnt)
					//打算分配具体的牌
					Calculate_risk_table(pack_cnt, incomplete_cnt, oper, player);					

				}
			}
		}
	}
}
//分配给玩家具体的牌，计算危险度表
//依据分配度作概率计算的话，pack_cnt与incomplete_cnt只会影响
void Calculate_risk_table(int pack_cnt, int incomplete_cnt, tile_risk_table_t& oper, int player)
{
	//依据个人出牌微调？	或者说，根据对手的上听数（模拟值）/副露数（真实值）设置紧急权重，若其即将听牌则权重提升

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
		//刻子存在杠可能
		for (int i = 0; i < triplet_table.size(); i++)
		{
			tile_t t = triplet_table[i].first;
			double p = triplet_table[i].second / total_tri_p;
			oper[t] += p;
		}
	}
	if (total_straight_p > 0)
	{
		//顺子不影响
		//即便是重叠的连顺也是作顺子搭子处理
	}
	if (total_pair_p > 0)
	{
		//对子成刻
		for (int i = 0; i < pair_table.size(); i++)
		{
			tile_t t = pair_table[i].first;
			double p = pair_table[i].second / total_pair_p;
			oper[t] += p;
		}
	}
	if (total_tri_p > 0)
	{
		//搭子
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
				//数牌排最优
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
//非弃胡出牌策略
int get_defend_tile_1(vector<Mahjong>& useless)
{
	if (useless.size() == 1) return useless[0].getTile();
	//清洗无用牌
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
	//策略:
	//牌局前期: 依照花牌 - 箭牌 - 风牌的次序打出
	//牌局后期: 不考虑十三幺这类极低概率事件(若要考虑则额外增加读牌河,判剩余幺九牌数量),当字牌仅剩手上这一张时打出
	//找到手上存在的字牌与数牌
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
	
	//判断牌局时期情况
	int turn_ = reset(turn);
	//前期:
	if (turn_ <= limit_turn)
	{
		//有字牌, 打字牌
		int tile = -1;
		if (!Feng.empty())	tile = *Feng.begin();
		if (!Jian.empty())	tile = *Jian.begin();
		if (!Hana.empty())	tile = *Hana.begin();
		if (tile != -1) return tile;

		//无字牌
		//1.找安全牌
		int safe_t = Get_safe_tile(useless);
		if (safe_t > 0) return safe_t;

		//2.计算危险度(前期信息不足,危险度拟合差)

		//3.从数牌边张开始向内打
		int max = -1;	//距离里张5的距离
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
	//后期:
	else
	{
		int* unplayed = memory.getUnPlayed();
		//map<Mahjong, int> cnt_hand = memory.getCntHand();
		
		//1.检查手牌中的字牌是否是绝张
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

		//2.找安全牌
		
		int safe_t = Get_safe_tile(useless);
		if (safe_t > 0) return safe_t;


		//3.计算危险度
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
//调用接口
int get_defend_tile(vector<Mahjong>& handTile)
{
 //传入两种参数:	1.非弃胡状态下, 传入无用牌(保证不存在手牌全是无用牌的情况)	2. 弃胡状态下, 传入手牌

	//1.非弃胡状态
	if (handTile.size() != memory.getHandTile().size())
	{
		if (handTile.size() == 1) return handTile[0].getTile();
		return get_defend_tile_1(handTile);
	}

	//2.弃胡状态
	 
	//1)检查是否有安全牌
	//优先安全牌判别:	检查手牌中>= 3的字牌和 余牌都在手牌中的字牌(分别包含于 >= 3和对手出过的情况)
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

	//2)计算危险度
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