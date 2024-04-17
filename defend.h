#pragma once
#include "StatusMemory.h"
#include "shanten.h"
#include <set>


#include <map>
#include <algorithm>
typedef double tile_risk_table_t[TILE_TABLE_SIZE];

int tile_get_rank(int tile);

void Init_table();

int Special_solve(int bias, int tn, int flag);

void Calculate_poss();

void Calculate_risk();

void Calculate_risk_table(int pack_cnt, int incomplete_cnt, tile_risk_table_t& oper, int player);

int Get_safe_tile(vector<Mahjong>& handTile);

int get_defend_tile_1(vector<Mahjong>& useless);

int get_defend_tile(vector<Mahjong>& handTile);