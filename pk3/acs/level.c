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
    int s  = unlockables[num][9];
    
    if (getMax(num) <= getUnlock(pln, num) && getMax(num) != -1 )
    {
        if (!quiet)
        {
            Print(s:"\caAlready at max level (\cd", d:getMax(num), s:"\ca) for \ck\"", s:getName(num), s:"\"\ca");
        }
        return false;
    }

    if (s == -1)
    {
        return true;
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
    
    int ret = ACS_ExecuteWithResult(s, a1,a2,a3);
    
    return ret;
}


function void giveHealth(int amount)
{
    int newHP = min(GetActorProperty(0, APROP_Health)+amount, GetActorProperty(0, APROP_SpawnHealth));
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

script UNLOCK_REPORT (void)
{
    SetHudSize(800, 600, 1);

    int i; int j; int offs;
    
    int unlocksUsed;

    for (i = 0; i < 32; i++)
    {
        if (!PlayerInGame(i))
        {
            continue;
        }
        
        unlocksUsed = 0;
        for (j = 0; j < UNLOCK_COUNT; j++)
        {
            unlocksUsed += getUnlock(i, j);
        }

        HudMessage(n:i+1, s:"\c- (unlocksUsed=", d:unlocksUsed, s:", unlocksLeft=", d:unlocksLeft[i], s:", level=", d:getStat(i, STAT_LEVEL), s:", XP=(", d:getStat(i, STAT_XP), s:", ", d:getStat(i, STAT_TOTALXP), s:"))";
                    HUDMSG_PLAIN, UNLOCK_HBASE-1000+i, CR_WHITE, 200.1, 200.0 + ((offs * 10) << 16), 5.0);
        offs++;
    }
}

script UNLOCK_MENU (void)
{
    int pln = PlayerNumber();

    if (uMenuLock[pln])
    {
        terminate;
    }

    SetHudSize(640, 480, 1);

    uMenuLock[pln] = 1;
    GiveInventory("SpawnProtection", 1);

    int i; int tic; int selected; int oSelected; int left; int oLeft;
    int name; int l_max; int current;
    int msgColor; int nameColor;
    int oneUnlocked; int startloop;
    
    int usekey; int upkey; int downkey;
    int useScr = MENU_USESPEED;
    int upScr = MENU_UPSPEED;
    int downScr = MENU_DOWNSPEED;

    for (i = 0; i < UNLOCK_COUNT; i++)
    {
        if (getUnlock(pln, i) < getMax(i))
        {
            startloop = 1;
            break;
        }
    }
    
    // Mainloop
    while (startloop)
    {
        if (PlayerIsBot(pln))
        {
            break;    
        }
        if (keyPressed(BT_JUMP))
        {
            break;
        }

        GiveInventory("SpawnStop", 1);
        
        if (keyDown(BT_USE))
        {
            if (usekey == -1)
            {
                usekey = useScr;
                useScr = max(1, useScr - 1);

                if (checkUnlock(pln, selected, 0))
                {
                    unlocksLeft[pln]--;
                    
                    unlockUnlock(pln, selected);

                    name    = getName(selected);
                    l_max   = getMax(selected);
                    current = getUnlock(pln, selected);

                    if (l_max == -1)
                    {
                        HudMessage(s:nameColor, s:name, s:"\c- (Current: \cd", d:current, s:"\c-)";
                            HUDMSG_FADEOUT, UNLOCK_HBASE + selected, CR_BRICK, (100.0 * (selected / 30)) + 80.1, (15.0 * (selected % 30)) + 80.1, 2.0, 2.0);
                    }
                    else
                    {
                        HudMessage(s:nameColor, s:name, s:"\c- (Max: \cf", d:l_max, s:"\c-, current: \cd", d:current, s:"\c-)";
                            HUDMSG_FADEOUT, UNLOCK_HBASE + selected, CR_BRICK, (100.0 * (selected / 30)) + 80.1, (15.0 * (selected % 30)) + 80.1, 2.0, 2.0);
                    }
                }
            }
        }
        else
        {
            usekey = 0;
            useScr = MENU_USESPEED;
        }

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

        left = unlocksLeft[pln];
       
        if (upScr == 1)
        {
            selected -= (upkey / upScr); 
        }
        else
        {
            selected -= (upkey / (upScr + 1)); 
        }

        if (downScr == 1)
        {
            selected += (downkey / downScr); 
        }
        else
        {
            selected += (downkey / (downScr + 1)); 
        }


        selected = mod(selected, UNLOCK_COUNT);

        if (tic % MENU_REFRESH == 0 || oSelected != selected || oLeft != left)
        {
            tic = 0;
            oneUnlocked = 0;
            
            SetFont("BIGFONT");

            HudMessage(s:"\cfLEVEL UP\c- - \cd", d:left, s:"\c- left";
                HUDMSG_FADEOUT, UNLOCK_HBASE - 1, CR_WHITE, 77.3, 34.1, ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
            
            SetFont("SMALLFONT");

            for (i = 0; i < UNLOCK_COUNT; i++)
            {
                if (selected == i)
                {
                    msgColor = CR_BRICK;
                }
                else
                {
                    msgColor = CR_DARKGREY;
                }
                
                if (checkUnlock(pln, i, 1))
                {
                    nameColor = "\ck";
                }
                else
                {
                    nameColor = "\cm";
                }

                name    = getName(i);
                l_max   = getMax(i);
                current = getUnlock(pln, i);
                
                if (current < l_max || l_max == -1)
                {
                    oneUnlocked = 1;
                }
                
                if (l_max == -1)
                {
                    HudMessage(s:nameColor, s:name, s:"\c- (Current: \cd", d:current, s:"\c-)";
                        HUDMSG_FADEOUT, UNLOCK_HBASE + i, msgColor, (75.0 * (i / 30)) + 80.1, (11.0 * (i % 30)) + 60.1,
                        ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
                }
                else
                {
                    HudMessage(s:nameColor, s:name, s:"\c- (Max: \cf", d:l_max, s:"\c-, current: \cd", d:current, s:"\c-)";
                        HUDMSG_FADEOUT, UNLOCK_HBASE + i, msgColor, (75.0 * (i / 30)) + 80.1, (11.0 * (i % 30)) + 60.1,
                        ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
                }
                    HudMessage(s:"\cf", k:"+forward", s:"\c- scrolls down, \cf", k:"+back", s:"\c- up, \cf",
                                k:"+use", s:"\c- selects, \cf", k:"+jump", s:"\c- exits"; 
                            HUDMSG_FADEOUT, UNLOCK_HBASE - 2, CR_GREEN, 320.4, 460.2,
                            ((MENU_REFRESH << 16) / 35) + 2.0, 2.0);
            }
        }
        

        tic++;
        oLeft     = left;
        oSelected = selected;
    
        if (!left || !oneUnlocked)
        {
            break;
        }
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
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 1, CR_GREY, 580.2, 110.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);
            
            SetFont("SMALLFONT");
            HudMessage(s:"XP: \cd", d:xp, s:"\cf/\cd", d:nextLevel;
                HUDMSG_FADEOUT, UNLOCK_HBASE2 + 2, CR_WHITE, 590.2, 130.2, ((HUD_REFRESH << 16) / 35) + 2.0, 2.0);
            
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
    int aRegenRate; int hRegenRate;
    
    int hpRate; int clipRate; int shellRate; int rocketRate; int cellRate;
    int hpGet;  int clipGet;  int shellGet;  int rocketGet;  int cellGet;

    while (PlayerInGame(pln))
    {
        hRegenRate = getStat(pln, STAT_HP_REGENRATE);
        aRegenRate = getStat(pln, STAT_AMMO_REGENRATE);
        
        hpRate += hRegenRate;
        hpGet   = hpRate / HEALTH_REGENRATE;
        
        clipRate   += aRegenRate;   clipGet   = clipRate   / AMMO_CLIPRATE;
        shellRate  += aRegenRate;   shellGet  = shellRate  / AMMO_SHELLRATE;
        rocketRate += aRegenRate;   rocketGet = rocketRate / AMMO_ROCKETRATE;
        cellRate   += aRegenRate;   cellGet   = cellRate   / AMMO_CELLRATE;
        
        hpRate     = mod(hpRate,     HEALTH_REGENRATE);
        clipRate   = mod(clipRate,   AMMO_CLIPRATE);
        shellRate  = mod(shellRate,  AMMO_SHELLRATE);
        rocketRate = mod(rocketRate, AMMO_ROCKETRATE);
        cellRate   = mod(cellRate,   AMMO_CELLRATE);
        
        GiveInventory("Clip",       clipGet);
        GiveInventory("Shell",      shellGet);
        GiveInventory("RocketAmmo", rocketGet);
        GiveInventory("Cell",       cellGet);
        
        giveHealth(hpGet);
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
    int killedHP = GetActorProperty(0, APROP_SpawnHealth);  // in case monster
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

    case 5: case 6:
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
        
            //print(d:i, s:"   ", s:cName, s:"   ", s:cAmmo, s:"   ", d:cAmmoC);
            
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

    case 6:
        weps = getStat(pln, STAT_WEPS);
        setStat(pln, STAT_WEPS, weps | pow(2, a1));
        break;
    }

    applyUnlock(pln, which);
}

script GENERAL_CHECK (int which, int a1, int a2)
{
    int pln = PlayerNumber();
    int quiet; int check; int end;
    
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
            if (!quiet)
            {
                Print(s:"\caNeed to unlock \cd", d:a2, s:"\ca levels of \ck\"", s:getName(a1), s:"\"\ca to unlock this");
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
