#include "StatusMemory.h"

extern int turn = 0;

Memory::Memory()
{
}

void Memory::initial(int myPos, int quan){//初始牌墙剩余

    myPosition = myPos;
    lastPosition = (myPos - 1 + 4) % 4;
    nextPosition = (myPos + 1) % 4;
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

void Memory::buHua(int num)
{
    Hana[myPosition] += num;
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

string Memory::getCurrAction()
{
    return actionEnumToStr(currAction);
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

const vector<Mahjong>& Memory::getChi(int idx)
{
    return Chi[idx];
}

const vector<Mahjong>& Memory::getPeng(int idx)
{
    return Peng[idx];
}

const vector<Mahjong>& Memory::getGang(int idx)
{
    return Gang[idx];
}

const vector<Mahjong>& Memory::getanGang(int idx)
{
    return anGang[idx];
}

const vector<Mahjong>& Memory::getEachPlayed(int idx)
{
    return eachPlayed[idx];
}

void Memory::playTile(Mahjong majang, action Action)
{
    switch (Action)
    {
    case PLAY: {
        eachPlayed[myPosition].push_back(majang);

        playTile(majang);

        break;
    }
    case CHI: {//吃传入的参数是中间的牌
        Chi[myPosition].push_back(majang);

        if (currPlayTile != majang.getLast()) {
            Mahjong tmp(majang.getLast());
            playTile(tmp);
        }

        if (currPlayTile != majang) {
            playTile(majang);
        }

        if (currPlayTile != majang.getNext()) {
            Mahjong tmp(majang.getNext());
            playTile(tmp);
        }

        break;
    }
    case PENG: {
        Peng[myPosition].push_back(majang);

        playTile(majang);
        playTile(majang);

        break;
    }
    case GANG: {//明杠
        Gang[myPosition].push_back(majang);


        playTile(majang);
        playTile(majang);
        playTile(majang);

        break;
    }
    case ANGANG: {
        anGang[myPosition].push_back(majang);

        playTile(majang);
        playTile(majang);
        playTile(majang);
        playTile(majang);

        break;
    }
    case BUGANG: {
        int tmpLen = Peng[myPosition].size();
        for (int i = 0; i < tmpLen; i++) {
            if (Peng[myPosition][i] == majang) {
                swap(Peng[myPosition][i], Peng[myPosition][tmpLen - 1]);
                Peng[myPosition].pop_back();
                Gang[myPosition].push_back(majang);

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
    if (currAction == PASS) {
        currAction = Action;
    }

}

void Memory::playTile(Mahjong majang)
{
    for (int i = 0; i < handNum[myPosition]; i++) {
        if (handTile[i] == majang) {
            swap(handTile[i], handTile[handNum[myPosition] - 1]);
            handTile.pop_back();
            break;
        }
    }
    handNum[myPosition]--;
}

void Memory::playTile(int idx, Mahjong majang, action Action)
{
    //吃碰明杠是建立在别人已经打出一张牌的基础上
    //因此我们只需要减少目标玩家打出的牌即可
    //对于吃，我们先保留上家打出的牌++
    //然后根据吃的中间牌三个--
    switch (Action)
    {
    case PLAY: {
        eachPlayed[idx].push_back(majang);

        handNum[idx]--;
        Unplayed[majang]--;


        break;
    }
    case CHI: {
        Chi[idx].push_back(majang);
        handNum[idx] -= 2;

        Unplayed[majang]--;
        Unplayed[majang.getLast()]--;
        Unplayed[majang.getNext()]--;

        Unplayed[currPlayTile]++;

        break;
    }
    case PENG: {
        Peng[idx].push_back(majang);

        handNum[idx] -= 2;
        Unplayed[majang] -= 2;

        break;
    }
    case GANG: {//明杠
        Gang[idx].push_back(majang);

        handNum[idx] -= 3;
        Unplayed[majang] -= 3;

        break;
    }
    case ANGANG: {
        anGang[idx].push_back(majang);

        handNum[idx] -= 4;
        Unplayed[majang] -= 4;

        break;
    }
    case BUGANG: {
        int tmpLen = Peng[idx].size();
        for (int i = 0; i < tmpLen; i++) {
            if (Peng[idx][i] == majang) {
                swap(Peng[idx][i], Peng[idx][tmpLen - 1]);
                Peng[idx].pop_back();
                Gang[idx].push_back(majang);

                handNum[idx]--;
                Unplayed[majang]--;

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
    currPlayTile = majang;
    if (currAction == PASS) {
        currAction = Action;
    }

}

void Memory::drawTile(Mahjong majang)
{
    handTile.push_back(majang);

    handNum[myPosition]++;
    totalTile--;

    Unplayed[majang]--;//手牌不算剩余的牌，用于计算可能的牌型
}

void Memory::drawTile(int idx,int num)
{
    handNum[idx]+=num;
    totalTile-=num;
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
}
