#include "zcommon.acs"
#library "level"

#include "unlockNames.h"
#include "unlockFuncs.h"
#include "unlocks.h"


global int 3:inGame[];
global int 4:unlocksLeft[];
global int 5:startStats[];
global int 6:unlocksPerPlayer[];

int uMenuLock[32];
int hasEntered[32];
int nagForMenu[32];

function int getName(int num)
{
    return unlockNames[num];
}

function int getMax(int num)
{
    return unlockables[num][8];
}

function int getCost(int num)
{
    return unlockables[num][13];
}

function void payForUnlock(int pln, int num)
{
    int subAmount = unlockables[num][13];

    unlocksLeft[pln] = max(unlocksLeft[pln] - subAmount, 0);
}

function int getUnlock(int pln, int num)
{
    int i = (pln * UNLOCK_COUNT) + num;
    return unlocksPerPlayer[i];
}

function void setUnlock(int pln, int num, int value)
{
    int i = (pln * UNLOCK_COUNT) + num;
    unlocksPerPlayer[i] = value;
}

function void addUnlock(int pln, int num, int value)
{
    int i = (pln * UNLOCK_COUNT) + num;
    unlocksPerPlayer[i] += value;
}

function void applyUnlock(int pln, int num)
{
    int s  = unlockables[num][0];

    if (s == -1)
    {
        return;
    }
    else if (s < 0 || s > 999)
    {
        Log(s:"ERROR: Unlock \"", s:getName(num), s:"\" has apply script ", d:s, s:", which makes no sense");
        return;
    }

    int a1 = unlockables[num][1];
    int a2 = unlockables[num][2];
    int a3 = unlockables[num][3];

    ACS_ExecuteAlways(s, 0, a1,a2,a3);
}

function void unlockUnlock(int pln, int num)
{
    if (!checkUnlock(pln, num, 0))
    {
        return;
    }

    int s  = unlockables[num][4];

    if (s < 0 || s > 999)
    {
        Log(s:"ERROR: Unlock \"", s:getName(num), s:"\" has unlock script ", d:s, s:", which makes no sense");
        return;
    }

    int a1 = unlockables[num][5];
    int a2 = unlockables[num][6];
    int a3 = unlockables[num][7];

    addUnlock(pln, num, 1);

    ACS_ExecuteAlways(s, 0, a1,a2,a3);
}

function int checkUnlock(int pln, int num, int quiet)
{
    // THIS IS WHY FUNCTIONS NEED SCOPE
    SetHudSize(640, 480, 1);

    int s  = unlockables[num][9];
    int ret; int diff; int addS = "";

    if (getMax(num) <= getUnlock(pln, num) && getMax(num) != -1 )
    {
        if (!quiet)
        {
                HudMessage(s:"Already at max";
                HUDMSG_FADEOUT, UNLOCK_HBASE3, CR_BRICK, 220.4, 130.1 + (MENU_SIZE * MENU_SPACING),
                    2.0, 1.0);
        }
        return false;
    }

    if (s == -1)
    {
        ret = 1;
    }
    else if (s < 0 || s > 999)
    {
        Log(s:"ERROR: Unlock \"", s:getName(num), s:"\" has check script ", d:s, s:", which makes no sense");
        return false;
    }

    int a1 = unlockables[num][10];
    int a2 = unlockables[num][11];
    int a3 = unlockables[num][12];

    if (quiet)  // Assume negative a2 means 'quiet'
    {
        a2 = -a2;
    }

    if (ret != 1)  // if there's an unlock script
    {
        ret = ACS_ExecuteWithResult(s, a1,a2,a3);
    }

    if (ret == 1 && (getCost(num) > unlocksLeft[pln]))
    {
        diff = getCost(num) - unlocksLeft[pln];

        if (diff != 1)
        {
            addS = "s";
        }

        if (!quiet)
        {
            HudMessage(s:"Need \cd", d:diff, s:"\c- more unlock", s:addS;
                HUDMSG_FADEOUT, UNLOCK_HBASE3, CR_BRICK, 220.4, 130.1 + (MENU_SIZE * MENU_SPACING),
                    2.0, 1.0);
        }
        return false;
    }

    return ret;
}


function void giveHealth(int amount)
{
    int curHP = GetActorProperty(0, APROP_Health);
    int newHP = middle(curHP, curHP+amount, GetActorProperty(0, APROP_SpawnHealth));
    SetActorProperty(0, APROP_Health, newHP);
}


function int getStat(int pln, int which)
{
    return startStats[(pln * STAT_COUNT) + which];
}

function void setStat(int pln, int which, int value)
{
    startStats[(pln * STAT_COUNT) + which] = value;
}

function void addStat(int pln, int which, int value)
{
    startStats[(pln * STAT_COUNT) + which] += value;
}

function int getArmorIndex(void)
{
    int pln = PlayerNumber();
    int i;

    for (i = 0; i < UNLOCK_ARMORCOUNT; i++)
    {
        if (GetArmorType(unlockArmors[i], pln) )
        {
            return i;
        }
    }

    return -1;
}

function int oneUnlocked(int pln, int quiet)
{
    int i;

    for (i = 0; i < UNLOCK_COUNT; i++)
    {
        if (checkUnlock(pln, i, quiet))
        {
            return 1;
        }
    }

    return 0;
}

function void printMenuItem(int pln, int pos, int which, int selected)
{
    int nameColor; int maxColor; int curColor; int costColor;

    int left    = unlocksLeft[pln];
    int name    = getName(which);
    int l_max   = getMax(which);
    int cost    = getCost(which);
    int current = getUnlock(pln, which);

    if (selected == which)
    {
        maxColor = CR_LIGHTBLUE;
        costColor = CR_TAN;

        if (l_max == current) { curColor = CR_RED; }
        else                  { curColor = CR_GREEN; }

        if (checkUnlock(pln, which, 1)) { nameColor = CR_GOLD; }
        else                        { nameColor = CR_DARKGREY; }

    }
    else
    {
        maxColor = CR_BLUE;
        costColor = CR_BROWN;

        if (l_max == current) { curColor = CR_DARKRED; }
        else                  { curColor = CR_DARKGREEN; }

        if (checkUnlock(pln, which, 1)) { nameColor = CR_YELLOW; }
        else                        { nameColor = CR_BLACK; }
    }

    // For convenience
    int FIELDBASE   = UNLOCK_HBASE + (4 * pos);
    int POS_Y       = (MENU_SPACING * pos) + 110.1;
    int REFRESHRATE = ((MENU_REFRESH << 16) / 35) + 2.0;

    HudMessage(s:name; HUDMSG_FADEOUT, FIELDBASE, nameColor, 215.2, POS_Y, REFRESHRATE, 2.0);

    if (l_max == -1)
    {
        HudMessage(s:"---"; HUDMSG_FADEOUT, FIELDBASE + 1, maxColor, 245.4, POS_Y, REFRESHRATE, 2.0);
    }
    else
    {
        HudMessage(d:l_max; HUDMSG_FADEOUT, FIELDBASE + 1, maxColor, 245.4, POS_Y, REFRESHRATE, 2.0);
    }

    if (l_max == current)
    {
        HudMessage(s:"MAX"; HUDMSG_FADEOUT, FIELDBASE + 2, curColor, 275.4, POS_Y, REFRESHRATE, 2.0);
    }
    else
    {
        HudMessage(d:current; HUDMSG_FADEOUT, FIELDBASE + 2, curColor, 275.4, POS_Y, REFRESHRATE, 2.0);
    }

    HudMessage(d:cost; HUDMSG_FADEOUT, FIELDBASE + 3, costColor, 305.4, POS_Y, REFRESHRATE, 2.0);
}




script UNLOCK_ENTER enter
{
    int pln = PlayerNumber();
    int i; int hp; int weps; int wepName;

    hasEntered[pln] = 1;
    GiveInventory("EndSpawnProtection", 1);

    if (!inGame[pln])
    {
        inGame[pln] = 1;

        // I'm lazy
        for (i = 0; i < STAT_COUNT; i++)
        {
            setStat(pln, i, 0);
        }

        setStat(pln, STAT_LEVEL, 1);
        setStat(pln, STAT_NEXTL, 250);
        unlocksLeft[pln] = 1;

        hp = GetActorProperty(0, APROP_SpawnHealth);

        if (hp != 0)
        {
            setStat(pln, STAT_HP, hp);
        }
        else
        {
            setStat(pln, STAT_HP, 100);
            SetActorProperty(0, APROP_SpawnHealth, 100);
        }
        
        ACS_ExecuteAlways(UNLOCK_HOOK_ENTER, 0, 0,0,0);
    }

    ACS_ExecuteAlways(UNLOCK_LEVELHUD, 0, 0,0,0);
    ACS_ExecuteAlways(UNLOCK_HOOK_DAEMON, 0, 0,0,0);
}

script UNLOCK_RESPAWN respawn
{
    int i;
    int pln = PlayerNumber();

    if (!hasEntered[pln])   // wasn't in the game - do ENTER
    {
        ACS_ExecuteAlways(UNLOCK_ENTER, 0, 0,0,0);
    }

    if (GetActorProperty(0, APROP_SpawnHealth) == 0)
    {
        SetActorProperty(0, APROP_SpawnHealth, 100);
    }

    for (i = 0; i < UNLOCK_COUNT; i++)
    {
        applyUnlock(pln, i);
    }

    ACS_ExecuteAlways(UNLOCK_HOOK_RESPAWN, 0, 0,0,0);
}

script UNLOCK_DISCONNECT (int pln) disconnect
{
    int i;
    for (i = 0; i < STAT_COUNT; i++)
    {
        setStat(pln, i, 0);
    }
}

script UNLOCK_MENU (int alwaysShow)
{
    int pln = PlayerNumber();

    int i; int j;
    int refresh; int top;
    int tic; int left; int oLeft;
    int name; int current; int cost;
    int msgColor;

    int selected;
    int oSelected;

    int usekey; int upkey; int downkey;
    int useScr = MENU_USESPEED;
    int upScr = MENU_UPSPEED;
    int downScr = MENU_DOWNSPEED;

    if (uMenuLock[pln] == 1)
    {
        uMenuLock[pln] = 2;
        GiveInventory("EndSpawnProtection", 1);
        terminate;
    }

    SetHudSize(640, 480, 1);

    uMenuLock[pln] = 1;
    nagForMenu[pln] = -1;

    if (!alwaysShow) { GiveInventory("SpawnProtection", 1); }

    // Mainloop
    while (oneUnlocked(pln, 1) || alwaysShow)
    {
        left = unlocksLeft[pln];

        if ((!left && !alwaysShow) || uMenuLock[pln] == 2) { break; }

        GiveInventory("SpawnStop", 1);

        if (keyDown(BT_USE))
        {
            if (usekey == -1)
            {
                usekey = useScr;
                useScr = max(1, useScr - 1);

                if (selected == UNLOCK_COUNT)
                {
                    break;
                }

                if (checkUnlock(pln, selected, 0))
                {
                    unlockUnlock(pln, selected);
                    payForUnlock(pln, selected);
                    refresh = 1;
                }
            }
        }
        else
        {
            usekey = 0;
            useScr = MENU_USESPEED;
        }

        // Begin scrolling
        if (keyDown(BT_FORWARD))
        {
            if (upkey == -1)
            {
                upkey = upScr;
                upScr = max(1, upScr - 1);
            }
        }
        else
        {
            upkey = 0;
            upScr = MENU_UPSPEED;
        }

        if (keyDown(BT_BACK))
        {
            if (downkey == -1)
            {
                downkey = downScr;
                downScr = max(1, downScr - 1);
            }
        }
        else
        {
            downkey = 0;
            downScr = MENU_DOWNSPEED;
        }

        if (upScr == 1) { selected -= (upkey / upScr); }
        else { selected -= (upkey / (upScr + 1)); }

        if (downScr == 1) { selected += (downkey / downScr); }
        else { selected += (downkey / (downScr + 1)); }
        // End scrolling

        selected = mod(selected, UNLOCK_COUNT+1);

        if ((tic % MENU_REFRESH == 0) || (oSelected != selected) || (oLeft != left) || (refresh))
        {
            tic = 0;
            refresh = 0;

            if (selected == UNLOCK_COUNT)
            {
                top = adjustBottom(top, top+MENU_SIZE-1, 0);
            }
            else
            {
                top = adjustBottom(top, top+MENU_SIZE-1, selected);
            }

            SetFont("BIGFONT");

            HudMessage(s:"UNLOCK MENU";
                HUDMSG_FADEOUT, UNLOCK_HBASE - 1, CR_WHITE, 220.4, 34.1, ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);

            SetFont("SMALLFONT");

            if (top > 0)
            {
                HudMessage(s:"/\\";
                        HUDMSG_FADEOUT, UNLOCK_HBASE - 5, CR_GOLD, 320.4, 100.1,
                        ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
            }
            else
            {
                HudMessage(s:"";
                        HUDMSG_FADEOUT, UNLOCK_HBASE - 5, CR_GOLD, 320.4, 100.1,
                        ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
            }

            if (top < UNLOCK_COUNT-MENU_SIZE)
            {
                HudMessage(s:"\\/";
                        HUDMSG_FADEOUT, UNLOCK_HBASE - 6, CR_GOLD, 320.4, 110.1 + (MENU_SIZE * MENU_SPACING),
                        ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
            }
            else
            {
                HudMessage(s:"";
                        HUDMSG_FADEOUT, UNLOCK_HBASE - 6, CR_GOLD, 320.4, 110.1,
                        ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
            }


            // For convenience
            int FIELDBASE   = UNLOCK_HBASE - 100;
            int POS_Y       = 84.1;
            int REFRESHRATE = ((MENU_REFRESH << 16) / 35) + 2.0;

            HudMessage(s:"Name";    HUDMSG_FADEOUT, FIELDBASE,     CR_GOLD,      155.4, POS_Y+10.0, REFRESHRATE, 2.0);
            HudMessage(s:"Max";     HUDMSG_FADEOUT, FIELDBASE - 1, CR_LIGHTBLUE, 245.4, POS_Y,      REFRESHRATE, 2.0);
            HudMessage(s:"Current"; HUDMSG_FADEOUT, FIELDBASE - 2, CR_GREEN,     275.4, POS_Y+10.0, REFRESHRATE, 2.0);
            HudMessage(s:"Cost";    HUDMSG_FADEOUT, FIELDBASE - 3, CR_TAN,       305.4, POS_Y,      REFRESHRATE, 2.0);


            HudMessage(s:"\cf", k:"+forward", s:"\c- scrolls up, \cf", k:"+back", s:"\c- down, \cf", k:"+use", s:"\c- selects, \cf", k:"unlockmenu", s:"\c- exits";
                    HUDMSG_FADEOUT, UNLOCK_HBASE - 3, CR_GREEN, 220.4, 50.1,
                    ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);

            // (x of y)
            HudMessage(s:"\(\cd", d:mod(selected+1, UNLOCK_COUNT+1), s:"\c- of \cf", d:UNLOCK_COUNT, s:"\c-)";
                    HUDMSG_FADEOUT, UNLOCK_HBASE - 4, CR_DARKGREY, 220.4, 110.1 + (MENU_SIZE * MENU_SPACING),
                    ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);



            if (selected == UNLOCK_COUNT) { msgColor = CR_BRICK; }
            else                          { msgColor = CR_WHITE; }

            HudMessage(s:"Exit unlock menu";
                HUDMSG_FADEOUT, UNLOCK_HBASE - 2, msgColor, 220.4, 70.1, ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);

            if (selected == UNLOCK_COUNT)
            {
                HudMessage(s:"";
                    HUDMSG_FADEOUT, UNLOCK_HBASE - 8, CR_BRICK, 440.4, 240.1, ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);

                HudMessage(s:"";
                    HUDMSG_FADEOUT, UNLOCK_HBASE - 9, CR_BRICK, 440.4, 240.1, ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
            }
            else
            {
                if (unlockQuotes[selected] != "")
                {
                    HudMessage(s:"\"", s:unlockQuotes[selected], s:"\"";
                        HUDMSG_FADEOUT, UNLOCK_HBASE - 8, CR_GOLD, 440.4, 250.1, ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
                }
                else
                {
                    HudMessage(s:"";
                        HUDMSG_FADEOUT, UNLOCK_HBASE - 8, CR_GOLD, 440.4, 250.1, ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
                }

                HudMessage(s:unlockDescriptions[selected];
                    HUDMSG_FADEOUT, UNLOCK_HBASE - 9, CR_BRICK, 440.4, 265.1, ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
            }

            for (i = 0; i < min(MENU_SIZE, UNLOCK_COUNT); i++)
            {
                j = mod(i + top, UNLOCK_COUNT);
                printMenuItem(pln, i, j, selected);
            }
        }

        tic++;
        oLeft     = left;
        oSelected = selected;
        usekey--; upkey--; downkey--;

        Delay(1);
    }

    GiveInventory("EndSpawnProtection", 1);
    nagForMenu[pln] = 0;
    uMenuLock[pln] = 0;
}


script UNLOCK_LEVELHUD (void)
{
    int pln = PlayerNumber();
    int tic;
    int xp; int oXp;
    int level; int oLevel;
    int nextLevel; int oNextLevel;
    int left; int oLeft;

    SetHudSize(640, 480, 1);
    SetFont("BIGFONT");

    while (PlayerInGame(pln))
    {
        oXp = xp;
        xp = getStat(pln, STAT_XP);

        oLevel = level;
        level = getStat(pln, STAT_LEVEL);

        oNextLevel = nextLevel;
        nextLevel = getStat(pln, STAT_NEXTL);

        oLeft = left;
        left = unlocksLeft[pln];

        if ((tic % HUD_REFRESH) == 0 || (xp != oXp) || (level != oLevel) || (nextLevel != oNextLevel) || (left != oLeft))
        {
            SetFont("BIGFONT");
            HudMessage(s:"Level \ck", d:level;
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 1, CR_GREY, 480.1, 110.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);

            HudMessage(s:"/";
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 4, CR_DARKRED, 554.4, 145.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);

            SetFont("SMALLFONT");
            HudMessage(s:"XP: \cd", d:xp;
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 2, CR_WHITE, 510.1, 140.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);

            HudMessage(d:nextLevel;
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 3, CR_GREEN, 555.1, 151.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);

            HudMessage(s:"\cf", d:left, s:"\c- left";
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 5, CR_TAN,  555.2, 125.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);

            tic = 0;
        }

        tic++;
        Delay(1);
    }
}

script UNLOCK_NAG (void)
{
    int pln = PlayerNumber();

    if (PlayerIsBot(pln))
    {
        while (unlocksLeft[pln] != 0)
        {
            int selected = Random(0, UNLOCK_COUNT);

            while (!checkUnlock(pln, selected, 1))
            {
                selected = Random(0, UNLOCK_COUNT);
            }

            unlocksLeft[pln]--;
            unlockUnlock(pln, selected);

            Delay(1);
        }
    }

    SetHudSize(640, 480, 1);

    if (nagForMenu[pln] == 1) { terminate; }

    if (nagForMenu[pln] == -1)
    {
        HudMessage(s:"You leveled up!";
            HUDMSG_FADEOUT, UNLOCK_HBASE2 + 50, CR_GOLD, 320.4, 30.2, 0.5, 1.0);

        terminate;
    }

    nagForMenu[pln] = 1;

    while (nagForMenu[pln] == 1)
    {
        HudMessage(s:"You leveled up! Press \ck\"", k:"unlockmenu", s:"\"\c- to use it";
            HUDMSG_FADEOUT, UNLOCK_HBASE2 + 50, CR_GOLD, 320.4, 30.2, 0.2, 1.0);

        Delay(35);
    }
}
script LEVEL_ADDXP (int pln, int addXP)
{
    if (pln == -1)
    {
        pln = PlayerNumber();
    }

    addStat(pln, STAT_XP, addXP);
    addStat(pln, STAT_TOTALXP, addXP);
    ACS_ExecuteAlways(LEVEL_RECALC, 0, pln, 0,0);
}

script LEVEL_RECALC (int pln)
{
    int xp     = getStat(pln, STAT_XP);
    int level  = getStat(pln, STAT_LEVEL);
    int next   = getStat(pln, STAT_NEXTL);
    int leveled;
    int i; int j;

    while (xp >= next)
    {
        xp -= next;

        level++;
        leveled++;
        unlocksLeft[pln]++;

        next += 25;
    }

    setStat(pln, STAT_XP, xp);
    setStat(pln, STAT_LEVEL, level);
    setStat(pln, STAT_NEXTL, next);

    if (leveled)
    {
        ACS_ExecuteAlways(UNLOCK_NAG, 0, 0,0,0);

        leveled = min(leveled, LEVELEFFECT_MAX);

        for (i = 0; i < 70; i++)
        {
            for (j = 0; j < leveled; j++)
            {
                GiveInventory("LevelUpEffect", 1);
            }
            Delay(1);
        }
    }
}

script LEVEL_ADDXP_ONKILL (int isMonster, int amount)
{
    ACS_ExecuteAlways(UNLOCK_HOOK_ONKILL, 0, isMonster,amount,0);

    int monOldHP;

    int killedHP = GetActorProperty(0, APROP_SpawnHealth);
    int killedPln = PlayerNumber();
    int killedTeam = GetPlayerInfo(killedPln, PLAYERINFO_TEAM);

    SetActivatorToTarget(0);

    int killerPln = PlayerNumber();
    int killerTeam = GetPlayerInfo(killerPln, PLAYERINFO_TEAM);
    int curStreak; int nowStreak;

    if (!amount)
    {
        amount = 5 + max((getStat(killedPln, STAT_TOTALXP) - getStat(killerPln, STAT_TOTALXP)) / 2, 0);
    }

    if (killerPln == -1)  // m-m-m-m-m-monster kill
    {
        terminate;
    }

    if (isMonster)
    {
        amount = killedHP / 5 ;
        
        // Difficulty hook
        amount *= difficultyMults[GameSkill()];
        amount >>= 16;

        ACS_ExecuteAlways(LEVEL_ADDXP, 0, killerPln, amount, 0);
    }
    else
    {
        amount *= getStat(killerPln, STAT_CURSTREAK) + 1;

        if (killedTeam == killerTeam)
        {
            if ((killedTeam == 4) && (killedPln != killerPln))
            {
                ACS_ExecuteAlways(LEVEL_ADDXP, 0, killerPln, amount, 0);
            }
        }
        else
        {
            ACS_ExecuteAlways(LEVEL_ADDXP, 0, killerPln, amount, 0);
        }

        addStat(killerPln, STAT_CURSTREAK, 1);
        addStat(killerPln, STAT_KILLCOUNT, 1);
        curStreak = getStat(killerPln, STAT_KILLCOUNT);
        Delay(105);
        nowStreak = getStat(killerPln, STAT_KILLCOUNT);

        if (curStreak == nowStreak)
        {
            setStat(killerPln, STAT_CURSTREAK, 0);
        }
    }
}

#include "level_unlocks.h"
