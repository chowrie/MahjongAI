#pragma once

#include"FanCalculator.h"
#include "tile.h"
#include "shanten.h"
#include "stringify.h"
#include "fan_calculator.h"

#include <stdio.h>
#include <iostream>
#include <limits>
#include <assert.h>
#include <time.h>
//test
using namespace mahjong;
using namespace std;
int handtiles_ShangTing(const char* str)
{
    hand_tiles_t hand_p;
    tile_t serving_p;
    long sign = string_to_tiles(str, &hand_p, &serving_p);
    if (sign != 0)
    {
        printf("error at line %d error = %ld\n", __LINE__, sign);
        return -3;
    }

    useful_table_t useful_count;
    int r[5], result = INT_MAX;
    r[0] = thirteen_orphans_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    r[1] = seven_pairs_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    r[2] = honors_and_knitted_tiles_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    r[3] = knitted_straight_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    r[4] = basic_form_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    for (int i = 0;i < 5;i++)result = min(r[i], result);
    return result;
}
int handtiles_point(const char* str, win_flag_t win_flag, wind_t prevalent_wind, wind_t seat_wind)
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
