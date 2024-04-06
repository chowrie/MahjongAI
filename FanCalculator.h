#pragma once

//#include "tile.h"
#include "shanten.h"
//#include "stringify.h"
#include "fan_calculator.h"
//#include "statusMemory.h"

#include <stdio.h>
#include <iostream>
#include <limits>
#include <assert.h>
#include <time.h>
//test
using namespace mahjong;
using namespace std;
typedef uint8_t Win_flag_t;

#define DISCARD       0   /// 点和
#define SELFDRAWN     1   /// 自摸
#define FOURLAST      2   /// 绝张
#define FUGANG          4   /// 杠
#define WALLLAST      8   /// 牌墙最后一张
#define WALLFIRST     16  /// 起手

int Hpoint(const char* str, Win_flag_t win_flag, wind_t prevalent_wind, wind_t seat_wind);
int Handtiles_ShangTing();//计算上听数
int Handtiles_Point(Win_flag_t win_flag);
void Unplayed_totiletable(tile_table_t& target);
wind_t intowind(int a);
int Count_usefultile(const tile_table_t& unplayed_table, const useful_table_t& useful_table);
int Handtiles_ShangTing_Temp(string &a);

