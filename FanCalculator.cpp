#pragma once
#include <stdio.h>
#include <iostream>
#include <climits>
#include <assert.h>
#include <time.h>
#include "FanCalculator.h"

//test
using namespace mahjong;
using namespace std;
int Hpoint(const char* str, Win_flag_t win_flag, wind_t prevalent_wind, wind_t seat_wind, tile_t wintile)
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
int Count_usefultile(const tile_table_t& unplayed_table, const useful_table_t& useful_table)
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
tile_t int_totile(int card)
{
    int card_type = card / 10;  // 获取牌的类型
    int card_number = card % 10; // 获取牌的编号

    switch (card_type) {
    case 2: // 条子
        card_type = 3; // 注意这里条子和饼子的顺序要调整
        break;
    case 3: // 饼子
        card_type = 2; // 注意这里条子和饼子的顺序要调整
        break;
    case 5: // 箭牌
        card_type = 4;
        card_number = 1 + 10 - 6;
        break;
    default:
        break;
    }

    return make_tile(card_type, card_number);
}
int Handtiles_ShangTing()//寄存器手牌上听数
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
    for (int i = 0; i < 5; i++)
    {
        r.push_back(make_pair(0, 0));
    }
    Unplayed_totiletable(unplayed_table);
    int result = INT_MAX;

    r[0].first = thirteen_orphans_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[0].first != std::numeric_limits<int>::max())
        r[0].second = Count_usefultile(unplayed_table, useful_count);

    r[1].first = seven_pairs_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[1].first != std::numeric_limits<int>::max())
        r[1].second = Count_usefultile(unplayed_table, useful_count);

    r[2].first = honors_and_knitted_tiles_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[2].first != std::numeric_limits<int>::max())
        r[2].second = Count_usefultile(unplayed_table, useful_count);

    r[3].first = knitted_straight_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[3].first != std::numeric_limits<int>::max())
        r[3].second = Count_usefultile(unplayed_table, useful_count);

    r[4].first = basic_form_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[4].first != std::numeric_limits<int>::max())
        r[4].second = Count_usefultile(unplayed_table, useful_count);
    if (r[4].first <= 0)
    {
        int fans = Handtiles_Point(str.c_str(), DISCARD, memory.getCurrPlayTile());
        if (fans > 0 && fans < 8)return (0 - fans);
    }
    for (int i = 0; i < 5; i++)
    {
        if (r[i].first < result && r[i].second>0)
        {
            result = r[i].first;
        }
    }
    return result;
}
wind_t intowind(int a)
{
    switch (a)
    {
    case 0:return mahjong::wind_t::EAST;
    case 1:return mahjong::wind_t::SOUTH;
    case 2:return mahjong::wind_t::WEST;
    case 3:return  mahjong::wind_t::NORTH;
    default:return wind_t::EAST;
    }
}
int Handtiles_Point(string str, Win_flag_t win_flag, int tile)
{
    wind_t p_wind = intowind(memory.getQuan()), s_wind = intowind(memory.getMyPosistion());

    tile_t wintile = int_totile(tile);

    return Hpoint(str.c_str(), win_flag, p_wind, s_wind, wintile);

}
void Unplayed_totiletable(tile_table_t& target)
{
    int* a = memory.getUnPlayed();
    for (int i = 10; i < 60; i++)
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
int Handtiles_ShangTing_Temp(string& a, int& useTilenum)//正常返回上听数；已胡但不够8番，返回-100；听牌返回0
{//牌a上听数
    hand_tiles_t hand_p;
    tile_t serving_p;
    long sign = string_to_tiles(a.c_str(), &hand_p, &serving_p);
    if (sign != 0)
    {
        printf("error at line %d error = %ld\n", __LINE__, sign);
        return -3;
    }
    tile_table_t unplayed_table = { 0 };
    useful_table_t useful_count = { 0 };
    vector<pair<int, int>> r;
    for (int i = 0; i < 5; i++)
    {
        r.push_back(make_pair(0, 0));
    }
    Unplayed_totiletable(unplayed_table);
    int result = INT_MAX;

    r[0].first = thirteen_orphans_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[0].first != std::numeric_limits<int>::max())
        r[0].second = Count_usefultile(unplayed_table, useful_count);

    r[1].first = seven_pairs_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[1].first != std::numeric_limits<int>::max())
        r[1].second = Count_usefultile(unplayed_table, useful_count);

    r[2].first = honors_and_knitted_tiles_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[2].first != std::numeric_limits<int>::max())
        r[2].second = Count_usefultile(unplayed_table, useful_count);

    r[3].first = knitted_straight_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[3].first != std::numeric_limits<int>::max())
        r[3].second = Count_usefultile(unplayed_table, useful_count);

    r[4].first = basic_form_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[4].first != std::numeric_limits<int>::max())
        r[4].second = Count_usefultile(unplayed_table, useful_count);
    if (r[4].first <= 0)
    {
        int fans = Handtiles_Point(a.c_str(), DISCARD, memory.getCurrPlayTile());
        if (fans < 8 && fans>0)return (0 - fans);//已胡，但番数<8
    }
    for (int i = 0; i < 5; i++)
    {
        if (r[i].first < result && r[i].second > 0)
        {
            result = r[i].first;
            useTilenum = r[i].second;
        }
    }
    return result;//返回0代表已听牌
}
Mahjong Search_playtile(Hand_Claim hands, int initShang)
{
    //int len = hands.handTile.size();
    //int perfectlo = 0;
    //bool isChange = false;
    //for (int i = 0; i < len; i++)
    //{
    //    Mahjong tmp = hands.handTile[i];
    //    if (i > 0 && tmp == hands.handTile[i - 1])
    //        continue;
    //    hands.removeHand(hands.handTile[i]);
    //    string t1 = hands.getFormatHandSting();
    //    int Ts = Handtiles_ShangTing_Temp(t1);
    //    if (Ts <= initShang)
    //    {
    //        perfectlo = i;
    //        initShang = Ts;
    //        isChange = true;
    //    }
    //    hands.addHand(tmp);
    //}
    //if (isChange)return hands.handTile[perfectlo];
    return 0;
}
bool quitHu(int Shangting, vector<Mahjong>& unused)//要弃胡，返回true;
{

    int pao = 0;
    int subPao = 0;

    int currPlayer = memory.getCurrPlayer();

    if (currPlayer == memory.getMyPosistion()) {

        Mahjong currDrawTile = memory.getCurrPlayTile();

        //后期
        if (turn >= 135) {

            bool dangerousFlag = true;

            for (auto& it : unused) {
                if (memory.getUnPlayed()[currDrawTile] < 3) {
                    dangerousFlag = false;
                }
            }

            //无用牌全危险&&后期状态
            if (dangerousFlag)return true;
        }
    }

    if (Shangting == 0)return false;

    for (int i = 0; i < 4; i++)
    {
        if (i == memory.getMyPosistion())continue;
        int nums = memory.gethandNum(i);

        //自己没听牌，并且确定有人听牌
        if (nums == 1) {
            return true;
        }

        if (nums <= 4)pao++;
        else if (nums <= 7)subPao++;
    }

    if (pao >= 2&& Shangting > 2)return true;
    if (turn >= 135 && pao && Shangting > 2)return true;

    //130回合还差3步+，大概是真胡不了了
    if (turn >= 130 && Shangting > 3)return true;

    return false;

}
int Tingtilenum(string& a)
{
    hand_tiles_t hand_p;
    tile_t serving_p;
    long sign = string_to_tiles(a.c_str(), &hand_p, &serving_p);
    if (sign != 0)
    {
        printf("error at line %d error = %ld\n", __LINE__, sign);
        return -3;
    }
    tile_table_t unplayed_table = { 0 };
    useful_table_t useful_count = { 0 };
    vector<pair<int, int>> r;
    for (int i = 0; i < 5; i++)
    {
        r.push_back(make_pair(0, 0));
    }

    // memory.getUnPlayed()[51] = 2;
    Unplayed_totiletable(unplayed_table);
    //  unplayed_table[69] = 2;
    int result = 0;

    r[0].first = thirteen_orphans_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[0].first != std::numeric_limits<int>::max())
        r[0].second = Count_usefultile(unplayed_table, useful_count);

    r[1].first = seven_pairs_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[1].first != std::numeric_limits<int>::max())
        r[1].second = Count_usefultile(unplayed_table, useful_count);

    r[2].first = honors_and_knitted_tiles_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[2].first != std::numeric_limits<int>::max())
        r[2].second = Count_usefultile(unplayed_table, useful_count);

    r[3].first = knitted_straight_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[3].first != std::numeric_limits<int>::max())
        r[3].second = Count_usefultile(unplayed_table, useful_count);

    r[4].first = basic_form_shanten(hand_p.standing_tiles, hand_p.tile_count, &useful_count);
    if (r[4].first != std::numeric_limits<int>::max())
        r[4].second = Count_usefultile(unplayed_table, useful_count);
    if (r[4].first <= 0)
    {
        int fans = Handtiles_Point(a.c_str(), DISCARD, memory.getCurrPlayTile());
        if (fans < 8 && fans>0)return (0 - fans);//已胡，但番数<8
    }
    for (int i = 0; i < 5; i++)
    {
        if (r[i].first != std::numeric_limits<int>::max() && r[i].second >= result)
        {
            result = r[i].second;
        }
    }
    return result;//返回听牌后 听牌的数量
    return 0;
}
Mahjong Searchting(map<Mahjong, int>& a)
{
    Mahjong pt = 0;
    int result = 0;
    for (map<Mahjong, int>::iterator it = a.begin(); it != a.end(); ++it)
    {
        if (it->second >= result)
        {
            pt = it->first;
            result = it->second;
        }
    }
    return pt;
}

