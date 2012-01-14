#include "zcommon.acs"
#library "level"

#include "unlockNames.h"
#include "unlockFuncs.h"
#include "unlocks.h"


global int 3:inGame[];
global int 4:unlocksLeft[];
global int 6:unlocksPerPlayer[];

// Layout: [hp, speed, jumpz, level, xp, next levelup, weapons, ...]
global int 5:startStats[];

int uMenuLock[32];
int hasEntered[32];

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
    int i; int weps; int wepName;

    hasEntered[pln] = 1;

    if (!inGame[pln])
    {
        inGame[pln] = 1;
        int hp = GetActorProperty(0, APROP_SpawnHealth);

        if (hp != 0)
        {
            setStat(pln, STAT_HP, hp);
        }
        else
        {
            setStat(pln, STAT_HP, 100);
            SetActorProperty(0, APROP_SpawnHealth, 100);
        }

        setStat(pln, STAT_SPEED, GetActorProperty(0, APROP_Speed));
        setStat(pln, STAT_JUMPZ, GetActorProperty(0, APROP_JumpZ));
        setStat(pln, STAT_XP, 0);
        setStat(pln, STAT_LEVEL, 1);
        setStat(pln, STAT_NEXTL, 50);
        setStat(pln, STAT_HP_REGENRATE, 0);
        setStat(pln, STAT_AMMO_REGENRATE, 0);
        setStat(pln, STAT_ARMOR_REGENRATE, 0);

        for (i = 0; i < UNLOCK_WEPCOUNT; i++)
        {
            wepName = unlockWeapons[i];

            if (CheckInventory(wepName))
            {
                weps |= pow(2, i);
            }
        }
        setStat(pln, STAT_WEPS, weps);

        for (i = 0; i < UNLOCK_COUNT; i++)
        {
            setUnlock(pln, i, 0);
        }
    }

    ACS_ExecuteAlways(UNLOCK_LEVELHUD, 0, 0,0,0);
    ACS_ExecuteAlways(UNLOCK_REGEN, 0, 0,0,0);
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

    if (uMenuLock[pln])
    {
        terminate;
    }

    SetHudSize(640, 480, 1);

    uMenuLock[pln] = 1;
    GiveInventory("SpawnProtection", 1);
    
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
    
    
    // Mainloop
    while (oneUnlocked(pln, 1) || alwaysShow)
    {
        left = unlocksLeft[pln];
        
        if (PlayerIsBot(pln) || (!left && !alwaysShow)) { break; }
        
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

            HudMessage(s:"\cfLEVEL UP\c- - \cd", d:left, s:"\c- left";
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
           

            HudMessage(s:"\cf", k:"+forward", s:"\c- scrolls up, \cf", k:"+back", s:"\c- down, \cf", k:"+use", s:"\c- selects";
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

    if (PlayerIsBot(pln))
    {
        while (unlocksLeft[pln] != 0)
        {
            selected = Random(0, UNLOCK_COUNT);

            while (!checkUnlock(pln, selected, 1))
            {
                selected = Random(0, UNLOCK_COUNT);
            }

            unlocksLeft[pln]--;
            unlockUnlock(pln, selected);
        }
    }

    GiveInventory("EndSpawnProtection", 1);
    uMenuLock[pln] = 0;
}


script UNLOCK_LEVELHUD (void)
{
    int pln = PlayerNumber();
    int tic;
    int xp; int oXp;
    int level; int oLevel;
    int nextLevel; int oNextLevel;

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

        if ((tic % HUD_REFRESH) == 0 || (xp != oXp) || (level != oLevel) || (nextLevel != oNextLevel))
        {
            SetFont("BIGFONT");
            HudMessage(s:"Level \ck", d:level;
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 1, CR_GREY, 480.1, 110.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);

            HudMessage(s:"/";
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 4, CR_GOLD, 554.4, 135.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);

            SetFont("SMALLFONT");
            HudMessage(s:"XP: \cd", d:xp;
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 2, CR_WHITE, 510.1, 130.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);
            
            HudMessage(d:nextLevel;
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 3, CR_GREEN, 555.1, 141.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);

            tic = 0;
        }

        tic++;
        Delay(1);
    }
}

script UNLOCK_REGEN (void)
{
    int pln = PlayerNumber();
    int tic;
    int aRegenRate; int hRegenRate; int rRegenRate;
    int aIndex; int curArm; int newArm; int armDif;
    int aLevel;

    int hpRate; int armRate; int clipRate; int shellRate; int rocketRate; int cellRate;
    int hpGet;  int armGet;  int clipGet;  int shellGet;  int rocketGet;  int cellGet;

    while (PlayerInGame(pln))
    {
        if (GetActorProperty(0, APROP_Health) < 0)
        {
            Delay(1);
            continue;
        }

        hRegenRate = getStat(pln, STAT_HP_REGENRATE);
        aRegenRate = getStat(pln, STAT_AMMO_REGENRATE);
        rRegenRate = getStat(pln, STAT_ARMOR_REGENRATE);

        hpRate  += hRegenRate;   hpGet  = hpRate  / HEALTH_REGENRATE;
        armRate += rRegenRate;   armGet = armRate / ARMOR_REGENRATE;
        
        clipRate   += aRegenRate;   clipGet   = clipRate   / AMMO_CLIPRATE;
        shellRate  += aRegenRate;   shellGet  = shellRate  / AMMO_SHELLRATE;
        rocketRate += aRegenRate;   rocketGet = rocketRate / AMMO_ROCKETRATE;
        cellRate   += aRegenRate;   cellGet   = cellRate   / AMMO_CELLRATE;

        hpRate     = mod(hpRate,     HEALTH_REGENRATE);
        armRate    = mod(armRate,    ARMOR_REGENRATE);
        clipRate   = mod(clipRate,   AMMO_CLIPRATE);
        shellRate  = mod(shellRate,  AMMO_SHELLRATE);
        rocketRate = mod(rocketRate, AMMO_ROCKETRATE);
        cellRate   = mod(cellRate,   AMMO_CELLRATE);

        giveHealth(hpGet);
        GiveInventory("ArmorBonus", armGet);
        
        GiveInventory("Clip",       clipGet);
        GiveInventory("Shell",      shellGet);
        GiveInventory("RocketAmmo", rocketGet);
        GiveInventory("Cell",       cellGet);
        
        // Monitor armor class
        
        aLevel = middle(0, getStat(pln, STAT_ARMORLEVEL), UNLOCK_ARMORCOUNT);
        curArm = CheckInventory("BasicArmor");
        
        if (curArm > 0 && aLevel > 0)
        {
            aIndex = getArmorIndex();
            
            if (aIndex < aLevel)
            {
                TakeInventory("BasicArmor", curArm);

                GiveInventory(unlockArmors[aLevel], 1);
                
                newArm = CheckInventory("BasicArmor");
                armDif = newArm - curArm;

                if (armDif == 0)
                {
                }
                else if (armDif < 0)
                {
                    GiveInventory("ArmorBonus", -armDif);
                }
                else
                {
                    TakeInventory("BasicArmor", newArm-1);
                    GiveInventory("ArmorBonus", curArm-1);
                }
            }
        }

        delay(1);
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

    while (xp > next)
    {
        xp -= next;

        level++;
        leveled++;
        unlocksLeft[pln]++;

        next += 25;
    }

    if (leveled)
    {
        ACS_ExecuteAlways(UNLOCK_MENU, 0, 0,0,0);
    }

    setStat(pln, STAT_XP, xp);
    setStat(pln, STAT_LEVEL, level);
    setStat(pln, STAT_NEXTL, next);
}

script LEVEL_ADDXP_ONKILL (int isMonster, int amount)
{
    int killedHP = GetActorProperty(0, APROP_SpawnHealth);
    int killedPln = PlayerNumber();
    int killedTeam = GetPlayerInfo(killedPln, PLAYERINFO_TEAM);

    SetActivatorToTarget(0);

    int killerPln = PlayerNumber();
    int killerTeam = GetPlayerInfo(killerPln, PLAYERINFO_TEAM);

    if (!amount)
    {
        amount = 5 + (getStat(killedPln, STAT_TOTALXP) / 2);
    }

    if (killerPln == -1)  // m-m-m-m-m-monster kill
    {
        terminate;
    }

    if (isMonster)
    {
        amount = (killedHP / 10) * 2;
        ACS_ExecuteAlways(LEVEL_ADDXP, 0, killerPln, amount, 0);
    }
    else
    {
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
    }
}


script GENERAL_ACTIVATE (int which)
{
    int pln = PlayerNumber();
    int i;

    switch (which)
    {
    case 0:
        int cHP    = GetActorProperty(0, APROP_Health);
        int cMaxHP = GetActorProperty(0, APROP_SpawnHealth);
        int nMaxHP = getStat(pln, STAT_HP);
        int dMaxHP = nMaxHP - cMaxHP;

        SetActorProperty(0, APROP_SpawnHealth, nMaxHP);
        SetActorProperty(0, APROP_Health, cHP + dMaxHP);
        break;

    case 1:
        SetActorProperty(0, APROP_SPEED, getStat(pln, STAT_SPEED));
        break;

    case 2:
        SetActorProperty(0, APROP_JUMPZ, getStat(pln, STAT_JUMPZ));
        break;

    case 6: case 7:
        int cName; int cAmmo; int cAmmoC; int cBit;
        int bestNewWeapon;

        int weps = getStat(pln, STAT_WEPS);
        
        for (i = 0; i < UNLOCK_WEPCOUNT; i++)
        {
            cBit   = pow(2, i);

            if (~weps & cBit)
            {
                continue;
            }

            cName  = unlockWeapons[i];
            cAmmo  = unlockAmmo[i];
            cAmmoC = unlockAmmoCount[i];

            if (!CheckInventory(cName))
            {
                bestNewWeapon = cName;
                GiveInventory(cName, 1);

                if (cAmmo != "<none>")
                {
                    GiveInventory(cAmmo, cAmmoC);
                }
            }
        }

        if (bestNewWeapon != 0)
        {
            SetWeapon(bestNewWeapon);
        }
        break;
    }
}

script GENERAL_UNLOCK (int which, int a1, int a2)
{
    int pln = PlayerNumber();
    int allweps; int choice; int cName; int cAmmo; int cAmmoC; int cBit; int weps;
    int i;

    switch (which)
    {
    case 0:
        addStat(pln, STAT_HP, a1);
        break;

    case 1:
        addStat(pln, STAT_SPEED, a1);
        break;

    case 2:
        addStat(pln, STAT_JUMPZ, a1);
        break;

    case 3:
        addStat(pln, STAT_AMMO_REGENRATE, a1);
        break;

    case 4:
        addStat(pln, STAT_HP_REGENRATE, a1);
        break;

    case 5:
        addStat(pln, STAT_ARMOR_REGENRATE, a1);
        break;

    case 6:
        weps = getStat(pln, STAT_WEPS);
        allweps = pow(2, a2 - a1) - 1;
        allweps <<= a1;

        if ((weps & allweps) == allweps)
        {
            for (i = 0; i < UNLOCK_MAXAMMOCOUNT; i++)
            {
                cAmmo  = unlockMaxAmmo[i];
                cAmmoC = unlockMaxAmmoCount[i];

                GiveInventory(cAmmo, cAmmoC);
            }
            break;
        }

        while (1)
        {
            choice = random(a1, a2-1);
            cName  = unlockWeapons[choice];
            cBit   = pow(2, choice);

            if (~weps & cBit)
            {
                break;
            }
        }

        setStat(pln, STAT_WEPS, weps | cBit);
        break;

    case 7:
        weps = getStat(pln, STAT_WEPS);
        setStat(pln, STAT_WEPS, weps | pow(2, a1));
        break;
    
    case 8:
        addStat(pln, STAT_ARMORLEVEL, 1);
        break;
    }
    ACS_ExecuteAlways(GENERAL_ACTIVATE, 0, which, a1, a2);
}

script GENERAL_CHECK (int which, int a1, int a2)
{
    int pln = PlayerNumber();
    int quiet; int check; int end;
    int diff; int addS = "";

    switch (which)
    {
    case 0:
        if (a1 < 0)
        {
            check = -a1;
            quiet = 1;
        }
        else
        {
            check = a1;
        }

        if (a2 == -1)
        {
            a2 = getMax(a1);
        }

        if (getUnlock(pln, check) < a2)
        {
            diff = a2 - getUnlock(pln, check);
            
            if (diff != 1)
            {
                addS = "s";
            }

            if (!quiet)
            {
                SetHudSize(640, 480, 1);
                HudMessage(s:"Need \cd", d:diff, s:"\c- more level", s:addS, s:" of \ck\"", s:getName(a1), s:"\"";
                    HUDMSG_FADEOUT, UNLOCK_HBASE3, CR_BRICK, 220.4, 130.1 + (MENU_SIZE * MENU_SPACING),
                        2.0, 1.0);
            }
            end = 0;
        }
        else
        {
            end = 1;
        }
        break;
    }

    SetResultValue(end);
}

script SCALED_CHECK (int base, int check, int scale)
{
    int pln = PlayerNumber();
    int quiet = 0;
    int ret = 1;
    int diff;
    int addS = "";
    if (check < 0)
    {
        check = -check;
        quiet = 1;
    }

    int necessaryLevel = (getUnlock(pln, base) + 1) * scale;

    if (getUnlock(pln, check) < necessaryLevel)
    {
        diff = necessaryLevel - getUnlock(pln, check);
        
        if (diff != 1)
        {
            addS = "s";
        }

        if (!quiet)
        {
            SetHudSize(640, 480, 1);
            HudMessage(s:"Need \cd", d:diff, s:"\c- more level", s:addS, s:" of \ck\"", s:getName(check), s:"\"";
                HUDMSG_FADEOUT, UNLOCK_HBASE3, CR_BRICK, 220.4, 130.1 + (MENU_SIZE * MENU_SPACING),
                    2.0, 1.0);
        }

        ret = 0;
    }

    SetResultValue(ret);
}

script PUKE_UNLOCKANDUSE (int which)
{
    int pln = PlayerNumber();

    unlockUnlock(pln, which);
    payForUnlock(pln, which);
}


script PUKE_REPORT (void)
{
    SetHudSize(800, 600, 1);

    int i; int j; int offs;

    int used; int left;
    int level; int xp; int totXP;
    int maxHP; int speed; int jumpZ;
    int hpReg; int arReg; int amReg;
    for (i = 0; i < 32; i++)
    {
        if (!PlayerInGame(i))
        {
            continue;
        }

        used = 0;
        for (j = 0; j < UNLOCK_COUNT; j++)
        {
            used += getUnlock(i, j);
        }
        
        left = unlocksLeft[i];
        level = getStat(i, STAT_LEVEL);
        xp    = getStat(i, STAT_XP);
        totXP = getStat(i, STAT_TOTALXP);
        maxHP = getStat(i, STAT_HP);
        speed = getStat(i, STAT_SPEED);
        jumpZ = getStat(i, STAT_JUMPZ);
        hpReg = getStat(i, STAT_HP_REGENRATE);
        arReg = getStat(i, STAT_ARMOR_REGENRATE);
        amReg = getStat(i, STAT_AMMO_REGENRATE);

        HudMessage(n:i+1, s:"\c- (used=", d:used, s:", left=", d:left, s:", level=", d:level, s:", xp=(", d:xp, s:", ", d:totXP, s:"), maxHP=", d:maxHP, s:",\nspeed=", f:speed, s:", jumpZ=", f:jumpZ, s:", hpReg=", d:hpReg, s:", arReg=", d:arReg, s:", amReg=", d:amReg, s:")";
                    HUDMSG_PLAIN, UNLOCK_HBASE-1000+i, CR_WHITE, 15.1, 50.0 + ((offs * 20) << 16), 5.0);
        offs++;
    }
}
