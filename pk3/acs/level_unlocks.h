// Do not delete this one - just blank its contents

script LEVEL_EXTRA_ONKILL (int isMonster, int amount)
{
    ACS_ExecuteAlways(LEVEL_LOOTAMMO, 0, 0,0,0);
}

// Okay, delete as you will

script UNLOCK_REGEN (void)
{
    int pln = PlayerNumber();
    int tic;
    int aRegenRate; int hRegenRate; int rRegenRate;
    int aIndex; int curArm; int newArm; int armDif;
    int aLevel;

    int hpRate; int armRate; int clipRate; int shellRate; int rocketRate; int cellRate;
    int hpGet;  int armGet;  int clipGet;  int shellGet;  int rocketGet;  int cellGet;

    int hp = GetActorProperty(0, APROP_Health);
    int oHp;

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

script LEVEL_LOOTAMMO (void)
{
    int kHp     = GetActorProperty(0, APROP_SpawnHealth);
    int kClip   = CheckInventory("Clip");
    int kShell  = CheckInventory("Shell");
    int kRocket = CheckInventory("RocketAmmo");
    int kCell   = CheckInventory("Cell");
    
    int i; int oAmmo; int nAmmo; int aDiff; int aType;
    int isMonster = (PlayerNumber() == -1);
  
    SetActivatorToTarget(0);

    int aPln   = PlayerNumber();
    int aLevel = getStat(aPln, STAT_AMMOLEECHLEVEL);

    if (isMonster)
    {
        kHp *= aLevel;
        
        kClip   = kHp / 20;
        kShell  = kHp / 100;
        kRocket = kHp / 150;
        kCell   = kHp / 30;
    }
    else
    {
        Print(d:aLevel);

        aLevel *= 0.2;

        Print(f:aLevel);

        kClip = (kClip * aLevel) >> 16;
        kShell = (kShell * aLevel) >> 16;
        kRocket = (kRocket * aLevel) >> 16;
        kCell = (kCell * aLevel) >> 16;
    }

    GiveInventory("Clip", kClip);
    GiveInventory("Shell", kShell);
    GiveInventory("RocketAmmo", kRocket);
    GiveInventory("Cell", kCell);

    aLevel = getStat(aPln, STAT_AMMOLEECHLEVEL);
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

    case 9:
        switch (getStat(pln, STAT_RUNELEVEL))
        {
        case 3:
            GiveInventory("DrainRuneToggler", 1);
            GiveInventory("SpreadRuneToggler", 1);
            GiveInventory("StrengthRuneToggler", 1);

        case 2:
            GiveInventory("HighJumpRuneToggler", 1);
            GiveInventory("RageRuneToggler", 1);
            GiveInventory("ResistanceRuneToggler", 1);

        case 1:
            GiveInventory("HasteRuneToggler", 1);
            GiveInventory("ProsperityRuneToggler", 1);
            GiveInventory("RegenerationRuneToggler", 1);
            break;
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
        addStat(pln, STAT_ARMORLEVEL, a1);
        break;

    case 9:
        addStat(pln, STAT_RUNELEVEL, a1);
        break;

    case 10:
        addStat(pln, STAT_AMMOLEECHLEVEL, a1);
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

