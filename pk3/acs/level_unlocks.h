// Do not delete these - just blank their contents

script UNLOCK_HOOK_ENTER (void)
{

}

script UNLOCK_HOOK_RESPAWN (void)
{

}

script UNLOCK_HOOK_DAEMON (void)
{

}

script UNLOCK_HOOK_ONKILL (int isMonster, int amount)
{

}

// Okay, delete as you will


script DOOMRL_GENERAL_APPLY (int which, int a1, int a2)
{
    int pln = PlayerNumber();
    int i;
    int oldHP; int newHP; int HPdiff; // for Ironman
    int newSpeed; // for Hellrunner
    int newJumpZ; // for Jumpman

    switch (which)
    {
    case 0: // Ironman
        oldHP = GetActorProperty(0, APROP_SpawnHealth);
        newHP = 100 * powFloat(1.21, getStat(pln, STAT_IRONMAN));
        newHP >>= 16;

        SetActorProperty(0, APROP_SpawnHealth, newHP);
        giveHealth(newHP - oldHP);
        break;

    case 1: // Hellrunner
        newSpeed = powFloat(1.1, getStat(pln, STAT_HELLRUNNER));
        SetActorProperty(0, APROP_Speed, newSpeed);
        break;

    case 2: // Jumpman
        newJumpZ = 8.0 + (getStat(pln, STAT_JUMPMAN) << 17);
        SetActorProperty(0, APROP_JumpZ, newJumpZ);
        break;

    case 4: // Tough as Nails
        for (i = 0; i < DOOMRL_TANPOWERS; i++)
        {
            TakeInventory(TaNPowers[i], 1);
        }

        for (i = 0; i < min(DOOMRL_TANPOWERS, getStat(pln, STAT_TOUGHASNAILS)); i++)
        {
            GiveInventory(TaNPowers[i], 1);
        }
        break;

    case 3: // Son of a Bitch
    case 5: // Son of a Gun
    case 6: // Eagle Eye
        break;
    case 7: // Shottyman
        break;
    }
}

script DOOMRL_GENERAL_UNLOCK (int which, int a1, int a2)
{
    int pln = PlayerNumber();

    switch (which)
    {
    case 0: // Ironman
        addStat(pln, STAT_IRONMAN, a1);
        break;

    case 1: // Hellrunner
        addStat(pln, STAT_HELLRUNNER, a2);
        break;

    case 2: // Jumpman
        addStat(pln, STAT_JUMPMAN, a1);
        break;

    case 3: // Son of a Bitch
        addStat(pln, STAT_SONOFABITCH, a1);
        break;

    case 4: // Tough as Nails
        addStat(pln, STAT_TOUGHASNAILS, a1);
        break;

    case 5: // Son of a Gun
        addStat(pln, STAT_SONOFAGUN, a1);
        break;

    case 6: // Eagle Eye
        addStat(pln, STAT_EAGLEEYE, a1);
        break;

    case 7: // Shottyman
        addStat(pln, STAT_SHOTTYMAN, a1);
        break;
    }

    ACS_ExecuteAlways(DOOMRL_GENERAL_APPLY, 0, which, a1, a2);
}

/// Weapon type:
//
// 1 = Pistol
// 2 = Shotgun

script DOOMRL_DAMAGE (int base, int wepType)
{
    int ret = base << 16;

    int pln = PlayerNumber();

    int soabLevel = getStat(pln, STAT_SONOFABITCH);
    int soagLevel = getStat(pln, STAT_SONOFAGUN);

    ret = FixedMul(ret, powFloat(1.15, soabLevel));

    switch (wepType)
    {
    case 0: // What /is/ this strange device?
        break;

    case 1:
       ret = FixedMul(ret, powFloat(1.35, soagLevel));
       break;

    case 2:
       ret = FixedMul(ret, powFloat(1.05, soagLevel));
       break;
    }

    SetResultValue(round(ret)); 
}

script DOOMRL_ACCURACY (int base, int baseFrac, int wepType)
{
    int ret = (base << 16) + ((baseFrac << 16) / 100);
    int pln = PlayerNumber();

    int eeLevel = getStat(pln, STAT_EAGLEEYE);

    ret = FixedMul(ret, powFloat(0.8, eeLevel));

    SetResultValue((floatIntg(ret) * 100) + percFrac(ret));
}

script DOOMRL_GETSTAT (int which)
{
    SetResultValue(getStat(PlayerNumber(), which));
}
