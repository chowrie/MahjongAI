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

bool isHeJueZhang(Mahjong& majang,action Action);

bool isQiangGangHe(action&currAction);

bool isGangShangKaiHua(action&currAction, bool GangFlag);//��������

bool isSeaMoon(int idx);

bool isHandSpring();//��������