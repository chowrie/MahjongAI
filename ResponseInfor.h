#pragma once
#include <string>
#include "FanCalculator.h"
#include "StatusMemory.h"
using namespace std;

string response();

int canChi();

bool canPeng();

bool canMinGang();

bool canAnGang();

bool canBuGang();

bool isHeJueZhang(Mahjong& majang);

bool isQiangGangHe(action&currAction);

bool isGangShangKaiHua(action&currAction,action&lastAction);//不计自摸

bool isSeaMoon(int idx);

bool isHandSpring();//不计自摸