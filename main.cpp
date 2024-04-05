#include "tile.h"
#include "shanten.h"
#include "stringify.h"
#include "fan_calculator.h"
#include"FanCalculator.h"

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
#include "jason\jsoncpp\json.h"// 本地环境下包含JSON库
#endif

using namespace mahjong;

#define SIMPLEIO 0

using namespace std;


vector<string> rrequest, rresponse;

int main()
{
    Json::Value inputJSON;
    cin >> inputJSON;
    turn = inputJSON["responses"].size();

    string s;
    for (int i = 0; i < turn; i++) {
        istringstream sin(inputJSON["requests"][i].asString());
        rresponse.push_back(inputJSON["responses"][i].asString());
        request(sin);
    }

    Json::Value outputJSON;
    rresponse.push_back(response());
    outputJSON["response"] = rresponse[turn];
    cout << outputJSON << endl;
    return 0;
}