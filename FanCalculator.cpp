#include "tile.h"
#include "shanten.h"
#include "stringify.h"
#include "fan_calculator.h"
#include "statusMemory.h"

#include <stdio.h>
#include <iostream>
#include <limits>
#include <assert.h>
#include <time.h>
#include "FanCalculator.h"
//test
using namespace mahjong;
using namespace std;


#define DISCARD       0   /// 点和
#define SELFDRAWN     1   /// 自摸
#define FOURLAST      2   /// 绝张
#define FUGANG          4   /// 杠
#define WALLLAST      8   /// 牌墙最后一张
#define WALLFIRST     16  /// 起手

int Hpoint(const char* str, Win_flag_t win_flag, wind_t prevalent_wind, wind_t seat_wind)
{
    calculate_param_t can;
    long sign = string_to_tiles(str, &can.hand_tiles, &can.win_tile);
    if (sign != PARSE_NO_ERROR) {
        printf("error at line %d error = %ld\n", __LINE__, sign);
        return -1;
    }
    fan_table_t fan_b;
    can.flower_count = 0;
    can.win_flag = win_flag;
    can.prevalent_wind = prevalent_wind;
    can.seat_wind = seat_wind;
    int points = calculate_fan(&can, &fan_b);
    return points;
}
int count_useful_tile(const tile_table_t& unplayed_table, const useful_table_t& useful_table)
{
        int cnt = 0;
        for (int i = 0; i < 34; ++i) {
            tile_t t = all_tiles[i];
            if (useful_table[t]) {
                cnt += unplayed_table[t];
            }
        }
        return cnt;
}
int Handtiles_ShangTing()
{
        string str = memory.getFormatHandSting();
        hand_tiles_t hand_p;
        tile_t serving_p;
        long sign = string_to_tiles(str.c_str(), &hand_p, &serving_p);
        if (sign != 0)
        {
            printf("error at line %d error = %ld\n", __LINE__, sign);
            return -3;
        }
        tile_table_t unplayed_table = { 0 };
        useful_table_t useful_count = { 0 };
        vector<pair<int, int>> r;
        Unplayed_totiletable(unplayed_table);
        int r[5], result = INT_MAX, perfectlo = 0;
        r[0].first= thirteen_orphans_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
        r[0].second = count_useful_tile(unplayed_table, useful_count);
        r[1].first = seven_pairs_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
        r[1].second = count_useful_tile(unplayed_table, useful_count);
        r[2].first = honors_and_knitted_tiles_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
        r[2].second = count_useful_tile(unplayed_table, useful_count);
        r[3].first = knitted_straight_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
        r[3].second = count_useful_tile(unplayed_table, useful_count);
        r[4].first = basic_form_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
        r[4].second = count_useful_tile(unplayed_table, useful_count);
        if (r[4].first == -1)
        {
            int fans = Handtiles_Point(DISCARD);
            if (fans < 8)r[4].first = INT_MAX;
        }
        for (int i = 0; i < 5; i++)
        {
            if (r[i].first < result&&r[i].second>0)
            {
                perfectlo = i;
                result = r[i].first;
            }
        }
        if (result != INT_MAX)return result;
        else return -100;
}
wind_t intowind(int a)
{
    switch (a)
    {
    case 0:return mahjong::wind_t::EAST;
    case 1:return mahjong::wind_t::SOUTH;
    case 2:return mahjong::wind_t::WEST;
    case 3:return  mahjong::wind_t::NORTH;
    default:;
    }
}
int Handtiles_Point(Win_flag_t win_flag)
{
    string str = memory.getFormatHandSting();
    wind_t p_wind = intowind(memory.getQuan()), s_wind = intowind(memory.getMyPosistion());
    return Hpoint(str.c_str(), win_flag, p_wind, s_wind);
}
void Unplayed_totiletable(tile_table_t &target)
{
    int* a = memory.getUnPlayed();
    for (int i = 10;i < 60;i++)
    {
        if (a[i] && i < 20)
        {
            target[i + 6] = a[i];
        }
        else if (a[i] && i < 30)
        {
            target[i + 28] = a[i];
        }
        else if (a[i] && i < 40)
        {
            target[i + 2] = a[i];
        }
        else if (a[i] && i < 54)
        {
            target[i + 18] = a[i];
        }
        else {}
    }
    return;
}


