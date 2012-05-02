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
    int oldHP; int newHP; int HPdiff;

    switch (which)
    {
    case 0: // Ironman
        oldHP = GetActorProperty(0, APROP_SpawnHealth);
        newHP = 100 * powFloat(1.25, getStat(pln, STAT_IRONMAN));
        newHP >>= 16;

        SetActorProperty(0, APROP_SpawnHealth, newHP);

        giveHealth(newHP - oldHP);
        break;
    }
}

script DOOMRL_GENERAL_UNLOCK (int which, int a1, int a2)
{
    int pln = PlayerNumber();

    switch (which)
    {
    case 0: // Ironman
        Print(s:"OH PIE ", d:a1);
        addStat(pln, STAT_IRONMAN, 1);
        break;
    }

    ACS_ExecuteAlways(DOOMRL_GENERAL_APPLY, 0, which, a1, a2);
}
