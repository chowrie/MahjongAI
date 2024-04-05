#pragma once

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
//test
using namespace mahjong;
using namespace std;
extern Memory memory;
typedef uint8_t Win_flag_t;
int Hpoint(const char* str, Win_flag_t win_flag, wind_t prevalent_wind, wind_t seat_wind);
int Handtiles_ShangTing();//¼ÆËãÉÏÌıÊı

int Handtiles_Point(Win_flag_t win_flag, wind_t prevalent_wind, wind_t seat_wind);
