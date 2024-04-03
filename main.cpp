#include "tile.h"
#include "shanten.h"
#include "stringify.h"
#include "fan_calculator.h"

#include <stdio.h>
#include <iostream>
#include <limits>
#include <assert.h>
#include <time.h>
#include <vector>
#include <algorithm>

#include "StatusMemory.h"
#include "RequestInfor.h"
#include "ResponseInfor.h"


#ifdef _BOTZONE_ONLINE
#include "jsoncpp/json.h" // 在线环境下包含JSON库
#else
#include "json/json.h "// 本地环境下包含JSON库
#endif

using namespace mahjong;

static int count_useful_tile(const tile_table_t &used_table, const useful_table_t &useful_table) {
    int cnt = 0;
    for (int i = 0; i < 34; ++i) {
        tile_t t = all_tiles[i];
        if (useful_table[t]) {
            cnt += 4 - used_table[t];
        }
    }
    return cnt;
}

void test_wait(const char *str) {
    hand_tiles_t hand_tiles;
    tile_t serving_tile;
    string_to_tiles(str, &hand_tiles, &serving_tile);

    std::cout << "----------------" << std::endl;
    puts(str);
    useful_table_t useful_table;
    bool is_wait = mahjong::is_waiting(hand_tiles, &useful_table);
    if (is_wait) {
        puts(" waiting:");
        char buf[64];
        for (tile_t t = TILE_1m; t < TILE_TABLE_SIZE; ++t) {
            if (useful_table[t]) {
                tiles_to_string(&t, 1, buf, sizeof(buf));
                printf("%s ", buf);
            }
        }
    }
    else {
        puts("not wait!");
    }
    puts("");
}

void test_points(const char *str, win_flag_t win_flag, wind_t prevalent_wind, wind_t seat_wind) {
    calculate_param_t param;

    long ret = string_to_tiles(str, &param.hand_tiles, &param.win_tile);
    if (ret != PARSE_NO_ERROR) {
        printf("error at line %d error = %ld\n", __LINE__, ret);
        return;
    }

    fan_table_t fan_table/* = { 0 }*/;
    puts("----------------");
    puts(str);

    param.flower_count = 0;

    param.win_flag = win_flag;
    param.prevalent_wind = prevalent_wind;
    param.seat_wind = seat_wind;
    int points = calculate_fan(&param, &fan_table);

    printf("max points = %d\n\n", points);
    if (points < 0) {
        return;
    }

    for (int i = 1; i < FLOWER_TILES; ++i) {
        if (fan_table[i] == 0) {
            continue;
        }
        if (fan_table[i] == 1) {
            printf("%s %d\n", fan_name[i], fan_value_table[i]);
        }
        else {
            printf("%s %d*%hd\n", fan_name[i], fan_value_table[i], fan_table[i]);
        }
    }
}

void test_shanten(const char *str) {
    hand_tiles_t hand_tiles;
    tile_t serving_tile;
    long ret = string_to_tiles(str, &hand_tiles, &serving_tile);
    if (ret != 0) {
        printf("error at line %d error = %ld\n", __LINE__, ret);
        return;
    }

    char buf[20];
    ret = hand_tiles_to_string(&hand_tiles, buf, sizeof(buf));
    puts(buf);

    auto display = [](const hand_tiles_t *hand_tiles, useful_table_t &useful_table) {
        char buf[64];
        for (tile_t t = TILE_1m; t < TILE_TABLE_SIZE; ++t) {
            if (useful_table[t]) {
                tiles_to_string(&t, 1, buf, sizeof(buf));
                printf("%s ", buf);
            }
        }

        tile_table_t cnt_table;
        map_hand_tiles(hand_tiles, &cnt_table);

        printf("%d枚", count_useful_tile(cnt_table, useful_table));
    };

    puts(str);
    useful_table_t useful_table/* = {false}*/;
    int ret0;
    ret0 = thirteen_orphans_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    printf("131=== %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()) display(&hand_tiles, useful_table);
    puts("\n");

    ret0 = seven_pairs_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    printf("7d=== %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()) display(&hand_tiles, useful_table);
    puts("\n");

    ret0 = honors_and_knitted_tiles_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    printf("honors and knitted tiles  %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()) display(&hand_tiles, useful_table);
    puts("\n");

    ret0 = knitted_straight_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    printf("knitted straight in basic form %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()) display(&hand_tiles, useful_table);
    puts("\n");

    ret0 = basic_form_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    printf("basic form %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()) display(&hand_tiles, useful_table);
    puts("\n");
}

#define SIMPLEIO 0

using namespace std;

int main()
{
    while (1) {
        Json::Value inputJSON;
        cin >> inputJSON;
        turn = inputJSON["responses"].size();

        for (int i = 0; i <= turn; i++) {
            istringstream sin(inputJSON["requests"][i].asString());
            request(sin);
        }

        Json::Value outputJSON;
        outputJSON["response"] = response();
        cout << outputJSON << endl;
    }
    return 0;
}