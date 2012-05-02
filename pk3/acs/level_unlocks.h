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
        addStat(pln, STAT_HELLRUNNER, a1);
        break;

    case 2: // Jumpman
        addStat(pln, STAT_JUMPMAN, a1);
    }

    ACS_ExecuteAlways(DOOMRL_GENERAL_APPLY, 0, which, a1, a2);
}
