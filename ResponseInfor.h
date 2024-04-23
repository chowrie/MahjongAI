#pragma once
#include <string>
#include "FanCalculator.h"
#include "StatusMemory.h"
using namespace std;

string response();

int canChi();


vector<int> CanChi();

bool canPeng();

bool canMinGang();

bool canAnGang();

bool canBuGang();

//

bool isHeJueZhang(Mahjong& majang,action Action);

bool isQiangGangHe(action&currAction);

bool isGangShangKaiHua(action&currAction, bool GangFlag);//²»¼Æ×ÔÃþ

bool isNoTile();

vector<Mahjong>getTingTile(string formatStr);