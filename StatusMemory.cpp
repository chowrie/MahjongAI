#include "StatusMemory.h"
#include<algorithm>

int turn = 0;

Memory memory;

Memory::Memory()
{
}

void Memory::initial(int myPos, int quan){//初始牌墙剩余

    myPosition = myPos;
    lastPosition = (myPos - 1 + 4) % 4;
    nextPosition = (myPos + 1) % 4;
    oppoPosition = (myPos + 2) % 4;
    Quan = quan;
    totalTile = allTile;

    for (int i = 11; i <= 19; i++) {//万
        Unplayed[i] = 4;
    }
    for (int i = 21; i <= 29; i++) {//筒
        Unplayed[i] = 4;
    }

    for (int i = 31; i <= 39; i++) {//条
        Unplayed[i] = 4;
    }


    for (int i = 41; i <= 44; i++) {//风
        Unplayed[i] = 4;
    }

    for (int i = 51; i <= 53; i++) {//箭
        Unplayed[i] = 4;
    }

    for (int i = 61; i <= 68; i++) {//花
        Unplayed[i] = 1;
    }

    for (int i = 0; i < 4; i++) {//番
        currFan[i] = 0;
        Hana[i] = 0;
    }
}

void Memory::buHua(int idx, int num)
{
    Hana[idx] += num;
}

int Memory::getQuan()
{
    return Quan;
}

int Memory::getMyPosistion()
{
    return myPosition;
}

int Memory::getLastPosition()
{
    return lastPosition;
}

int Memory::getNextPosition()
{
    return nextPosition;
}

int Memory::getoppoPosition()
{
    return oppoPosition;
}

int Memory::getFormatPosition(int target)
{
    if (target == (myPosition + 3) % 4)return 1;
    if (target == (myPosition + 2) % 4)return 2;
    if (target == (myPosition + 1) % 4)return 3;

    return 0;
}

int Memory::getLastPosition(int idx)
{
    return (idx + 3) % 4;
}

int Memory::getNextPosition(int idx)
{
    return (idx + 1) % 4;
}

int Memory::getoppoPosition(int idx)
{
    return (idx + 2) % 4;
}

int Memory::getFormatPosition(int own, int target)
{
    if (target == (own + 3) % 4)return 1;
    if (target == (own + 2) % 4)return 2;
    if (target == (own + 1) % 4)return 3;

    return 0;
}

vector<Mahjong>& Memory::getHandTile()
{
    return handTile;
}

void Memory::setCurrPlayer(int idx)
{
    currPlayer = idx;
}

int Memory::getCurrPlayer()
{
    return currPlayer;
}

void Memory::setCurrAction(action Action)
{
    currAction = Action;
}

void Memory::setCurrAction(string Action)
{
    currAction = actionStrToEnum(Action);
}

action Memory::getCurrAction()
{
    return currAction;
}

Mahjong Memory::getCurrPlayTile()
{
    return currPlayTile;
}

int Memory::getTargetTileLeft(int tileNum)
{
    return Unplayed[tileNum];
}

int Memory::getTargetTileLeft(const Mahjong& majang)
{
    return Unplayed[majang];
}

int Memory::getTargetTileLeft(const Mahjong majang)
{
    return Unplayed[majang];
}

int Memory::getTargetTileLeft(string& majang)
{
    return Unplayed[Mahjong(majang)];
}

int Memory::getTargetTileLeft(string majang)
{
    return Unplayed[Mahjong(majang)];
}

vector<pair<Mahjong, int>>& Memory::getChi(int idx)
{
    return Chi[idx];
}

vector<pair<Mahjong, int>>& Memory::getPeng(int idx)
{
    return Peng[idx];
}

vector<pair<Mahjong, int>>& Memory::getGang(int idx)
{
    return Gang[idx];
}

vector<Mahjong>& Memory::getEachPlayed(int idx)
{
    return eachPlayed[idx];
}

int Memory::getHana(int idx)
{
    return Hana[idx];
}

int* Memory::getUnPlayed()
{
    return Unplayed;
}

void Memory::playTile(Mahjong majang, action Action)
{
    int previousPlayer = getFormatPosition(currPlayer);

    switch (Action)
    {
    case PLAY: {
        eachPlayed[myPosition].push_back(majang);

        playTile(majang);

        break;
    }
    case CHI: {//吃传入的参数是中间的牌

        if (currPlayTile != majang.getLast()) {
            Mahjong tmp(majang.getLast());
            playTile(tmp);
        }
        else {
            Chi[myPosition].push_back(make_pair(majang, 1));
        }

        if (currPlayTile != majang) {
            playTile(majang);
        }
        else {
            Chi[myPosition].push_back(make_pair(majang, 2));
        }

        if (currPlayTile != majang.getNext()) {
            Mahjong tmp(majang.getNext());
            playTile(tmp);
        }
        else {
            Chi[myPosition].push_back(make_pair(majang, 3));
        }
        break;
    }
    case PENG: {
        Peng[myPosition].push_back(make_pair(majang, previousPlayer));

        playTile(majang);
        playTile(majang);

        break;
    }
    case GANG: {//明杠
        Gang[myPosition].push_back(make_pair(majang, previousPlayer));


        playTile(majang);
        playTile(majang);
        playTile(majang);

        break;
    }
    case ANGANG: {
        Gang[myPosition].push_back(make_pair(majang, 0));

        playTile(majang);
        playTile(majang);
        playTile(majang);
        playTile(majang);

        break;
    }
    case BUGANG: {


        for (auto it = Peng[myPosition].begin();
            it != Peng[myPosition].end(); ++it) {
            if (it->first == majang) {

                Gang[myPosition].push_back(make_pair(majang, it->second + 3));

                Peng[myPosition].erase(it);

                playTile(majang);

                break;
            }

        }


        break;
    }
    case BUHUA: {
        Hana[myPosition]++;

        playTile(majang);
        break;
    }
    default:
        break;
    }

    currPlayer = myPosition;
    currPlayTile = majang;
    currAction = Action;

}

void Memory::playTile(Mahjong majang)
{
    for (auto it = handTile.begin();
        it != handTile.end(); ++it) {
        if (*it == majang) {

            handTile.erase(it);

            break;
        }
    }
    handNum[myPosition]--;
    cnt_hand[majang]--;

}

void Memory::playTile(int idx, Mahjong majang, action Action)
{
    //吃碰明杠是建立在别人已经打出一张牌的基础上
    //因此我们只需要减少目标玩家打出的牌即可
    //对于吃，我们先保留上家打出的牌++
    //然后根据吃的中间牌三个--

    int previousPlayer = getFormatPosition(idx,currPlayer);

    switch (Action)
    {
    case PLAY: {
        eachPlayed[idx].push_back(majang);

        handNum[idx]--;
        Unplayed[majang]--;


        break;
    }
    case CHI: {
        handNum[idx] -= 2;

        Unplayed[majang]--;
        Unplayed[majang.getLast()]--;
        Unplayed[majang.getNext()]--;

        if (currPlayTile == majang.getLast()) {
            Chi[idx].push_back(make_pair(majang,1));
        }
        else if (currPlayTile == majang){
            Chi[idx].push_back(make_pair(majang, 2));
        }
        else {
            Chi[idx].push_back(make_pair(majang, 3));
        }
        Unplayed[currPlayTile]++;

        break;
    }
    case PENG: {
        Peng[idx].push_back(make_pair(majang, previousPlayer));

        handNum[idx] -= 2;
        Unplayed[majang] -= 2;

        break;
    }
    case GANG: {//明杠
        Gang[idx].push_back(make_pair(majang, previousPlayer));

        handNum[idx] -= 3;
        Unplayed[majang] -= 3;

        break;
    }
    case ANGANG: {
        Gang[idx].push_back(make_pair(majang, previousPlayer));

        handNum[idx] -= 4;
        Unplayed[majang] -= 4;

        break;
    }
    case BUGANG: {

        for (auto it = Peng[idx].begin();
            it != Peng[idx].end(); ++it) {
            if (it->first == majang) {

                Gang[idx].push_back(make_pair(majang, it->second + 3));

                Peng[idx].erase(it);

                handNum[idx] --;
                Unplayed[majang] --;

                break;
            }

        }

        break;
    }
    case BUHUA: {
        Hana[idx]++;

        handNum[idx]--;
        Unplayed[majang]--;

        break;
    }
    default:
        break;
    }


    currPlayer = idx;
    currPlayTile = majang;//打出的，响应的
    currAction = Action;
}


void Memory::drawTile(Mahjong majang)
{
    handTile.push_back(majang);

    handNum[myPosition]++;
    totalTile--;

    Unplayed[majang]--;//手牌不算剩余的牌，用于计算可能的牌型
    cnt_hand[majang]++;

    currPlayer = myPosition;
    currPlayTile = majang;
    currAction = DRAW;
}

void Memory::drawTile(int idx,int num)
{
    handNum[idx]+=num;
    totalTile-=num;

    currPlayer = idx;
    currPlayTile = 0;
    currAction = DRAW;
}

int Memory::getCntHand(Mahjong majang)
{
    return cnt_hand[majang];
}

void Memory::sortHand()
{
    sort(handTile.begin(), handTile.end(), cmp());
}

string Memory::getFormatHandSting()
{
    string res="";
    for (auto it : Chi[myPosition]) {
        Mahjong tmpTile = it.first;

        res+= "[" + tmpTile.getLast().getFormatStr() + tmpTile.getFormatStr() + tmpTile.getNext().getFormatStr();
        res+= "," + to_string(it.second) + "]";
    }

    for (auto it : Peng[myPosition]) {
        Mahjong tmpTile = it.first;

        res+= "[" + tmpTile.getLast().getFormatStr() + tmpTile.getFormatStr() + tmpTile.getNext().getFormatStr();
        res+= "," + to_string(it.second) + "]";
    }


    for (auto it : Gang[myPosition]) {
        Mahjong tmpTile = it.first;

        res+= "[" + tmpTile.getLast().getFormatStr() + tmpTile.getFormatStr() + 
            tmpTile.getNext().getFormatStr() + tmpTile.getNext().getFormatStr();
        res+= "," + to_string(it.second) + "]";
    }

    for (auto it : handTile) {
        res+= it.getFormatStr();
    }

    return res;
}

action actionStrToEnum(string Action)
{
    switch (Action[0])
    {
    case'P': {
        switch (Action[1])
        {
        case'A':return PASS; break;
        case'L':return PLAY; break;
        case'E':return PENG; break;
        default:
            break;
        }
        break;
    }
    case'D':return DRAW; break;
    case'C':return CHI; break;
    case'G':return GANG; break;
    case'B': {
        if (Action[2] == 'G') {
            return BUGANG;
        }
        else {
            return BUHUA;
        }
        break;
    }
    case'A':return ANGANG; break;
    case'H':return HU; break;
    default:
        break;
    }
}

string actionEnumToStr(action Action)
{
        switch (Action)
        {
        case PASS:return "PASS";
            break;
        case DRAW:return "DRAW";
            break;
        case PLAY:return "PLAY";
            break;
        case CHI:return "CHI";
            break;
        case PENG:return "PENG";
            break;
        case GANG:return "GANG";
            break;
        case BUGANG:return "BUGANG";
            break;
        case ANGANG:return "ANGANG";
            break;
        case BUHUA:return "BUHUA";
            break;
        case HU:return "HU";
            break;
        default:
            break;
        }
}

Hand_Claim::Hand_Claim()
{
    memory.sortHand();
    myPosition = memory.getMyPosistion();
    handTile = memory.getHandTile();
    Chi[memory.getMyPosistion()] = memory.getChi(memory.getMyPosistion());
    Peng[memory.getMyPosistion()] = memory.getPeng(memory.getMyPosistion());
    Gang[memory.getMyPosistion()] = memory.getGang(memory.getMyPosistion());

    Hana = memory.getHana(memory.getMyPosistion());
}

void Hand_Claim::addHand(Mahjong majang)
{
    handTile.push_back(majang);
    sort(handTile.begin(), handTile.end(), cmp());
}

void Hand_Claim::removeHand(Mahjong majang)
{
    auto it = find(handTile.begin(), handTile.end(), majang);
    if (it != handTile.end()) {
        handTile.erase(it);
    }

}

void Hand_Claim::addChi(Mahjong majang, int target)
{
    if (target == 1) {
        Chi[myPosition].push_back(make_pair(majang.getNext(), target));
        removeHand(majang.getNext());
        removeHand(majang.getNext().getNext());
    }
    else if (target == 2) {
        Chi[myPosition].push_back(make_pair(majang, target));
        removeHand(majang.getNext());
        removeHand(majang.getLast());

    }
    else {
        Chi[myPosition].push_back(make_pair(majang.getLast(), target));
        removeHand(majang.getLast());
        removeHand(majang.getLast().getLast());

    }

}

void Hand_Claim::removeChi(Mahjong majang, int target)
{
    for (auto it = Chi[myPosition].begin();
        it != Chi[myPosition].end(); ++it) {
        if (it->first == majang) {

            Chi[myPosition].erase(it);

            break;
        }
    }

    if (target != 1) {
        handTile.push_back(majang.getLast());
    }

    if (target != 2) {
        handTile.push_back(majang);
    }

    if (target != 3) {
        handTile.push_back(majang.getNext());
    }
}

void Hand_Claim::addPeng(Mahjong majang, int target)
{

    Peng[myPosition].push_back(make_pair(majang, target));

    removeHand(majang);
    removeHand(majang);
}

int Hand_Claim::removePeng(Mahjong majang)
{
    int target = -1;

    for (auto it = Peng[myPosition].begin();
        it != Peng[myPosition].end(); ++it) {
        if (it->first == majang) {

            Peng[myPosition].erase(it);

            break;
        }
    }

    handTile.push_back(majang);
    handTile.push_back(majang);

    return target;
}

void Hand_Claim::addAnGang(Mahjong majang)
{
    Gang[myPosition].push_back(make_pair(majang, 0));

    int num = 4;

    while (num--) {
        removeHand(majang);
    }
}

void Hand_Claim::addMinGang(Mahjong majang, int target)
{
    Gang[memory.getMyPosistion()].push_back(make_pair(majang, target));

    int num = 3;

    while (num--) {
        removeHand(majang);
    }

}

void Hand_Claim::addBuGang(Mahjong majang)
{
    int target = removePeng(majang);
    Gang[memory.getMyPosistion()].push_back(make_pair(majang, target+3));
    int num = 3;

    while (num--) {
        removeHand(majang);
    }
}

void Hand_Claim::removeAnGang(Mahjong majang)
{
    for (auto it = Gang[myPosition].begin();
        it != Gang[myPosition].end(); ++it) {
        if (it->first == majang) {

            Gang[myPosition].erase(it);
            break;
        }
    }

    int num = 4;
    while (num--) {
        handTile.push_back(majang);
    }
}

void Hand_Claim::removeMinGang(Mahjong majang)
{
    for (auto it = Gang[myPosition].begin();
        it != Gang[myPosition].end(); ++it) {
        if (it->first == majang) {

            Gang[myPosition].erase(it);

            break;
        }
    }

    int num = 3;
    while (num--) {
        handTile.push_back(majang);
    }
}

void Hand_Claim::removeBuGang(Mahjong majang)
{

    int target = -1;
    for (auto it = Gang[myPosition].begin();
        it != Gang[myPosition].end(); ++it) {
        if (it->first == majang) {

            Gang[myPosition].erase(it);

            target = it->second - 3;
            break;
        }
    }

    int num = 3;
    while (num--) {
        handTile.push_back(majang);
    }

    addPeng(majang, target);
}

string Hand_Claim::getFormatHandSting()
{

    string res = "";
    for (auto it : Chi[myPosition]) {
        Mahjong tmpTile = it.first;

        res += "[" + tmpTile.getLast().getFormatStr() + tmpTile.getFormatStr() + tmpTile.getNext().getFormatStr();
        res += "," + to_string(it.second) + "]";
    }

    for (auto it : Peng[myPosition]) {
        Mahjong tmpTile = it.first;

        res += "[" + tmpTile.getFormatStr() + tmpTile.getFormatStr() + tmpTile.getFormatStr();
        res += "," + to_string(it.second) + "]";
    }


    for (auto it : Gang[myPosition]) {
        Mahjong tmpTile = it.first;

        res += "[" + tmpTile.getFormatStr() + tmpTile.getFormatStr() +
            tmpTile.getFormatStr() + tmpTile.getFormatStr();
        res += "," + to_string(it.second) + "]";
    }

    for (auto it : handTile) {
        res += it.getFormatStr();
    }

    return res;
}

