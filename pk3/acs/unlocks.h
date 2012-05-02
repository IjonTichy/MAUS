//#include "unlockNames.h"

#define UNLOCK_COUNT        1
#define ENTRY_COUNT         14
#define UNLOCK_ARMORCOUNT   3
#define UNLOCK_WEPCOUNT     13
#define UNLOCK_MAXAMMOCOUNT 4

/* Unlockable contents:
 * activate script, arg 1, arg 2, arg 3,
 * unlock script, arg 1, arg 2, arg 3,
 * max, req. script, arg 1, arg 2, arg 3, cost
 *
 * If you use GENERAL_UNLOCK as the unlock script,
 * arg 1 should be the unlock's array index -
 * otherwise, you get unexpected results (this is a
 * limitation of ACS scripts only supporting three
 * arguments)
 *
 * -1 as the requirement script = no requirement
 *
 * GENERAL_ACTIVATE only uses the first argument
 */

int unlockables[UNLOCK_COUNT][ENTRY_COUNT] =
{
    // Activate            args     Unlock                 args    Max Lock  args  cost
    {DOOMRL_GENERAL_APPLY, 0,0,0,   DOOMRL_GENERAL_UNLOCK, 0,1,0,   3,  -1, 0,0,0,  1},
};

// This is in a seperate array because of ACS bugginess
int unlockNames[UNLOCK_COUNT] =
{
    "Ironman",
};

// Line length should be max 50 chars

int unlockDescriptions[UNLOCK_COUNT] = 
{
// Ironman
"You're tough. You're tougher than tough. People\n\
bounce bullets off of you, and you don't care.\n\
For every level of this, your max health is\n\
increased by 20 percent.",
};

int unlockQuotes[UNLOCK_COUNT] =
{
    "Who's a man and a half? I'm a man and a half!",
};

int unlockArmors[UNLOCK_ARMORCOUNT] =
{
    "GreenArmor",
    "BlueArmor",
    "RedArmor",
};

int difficultyMults[8] = 
{
    2.0,
    2.0,
    1.5,
    1.0,
    1.5,
    1.5,
    2.0,
    2.0,
};

int unlockWeapons[UNLOCK_WEPCOUNT] =
{
    "Fist", "Chainsaw",
    "Pistol",
    "Shotgun", "SuperShotgun",
    "Chaingun", "Minigun",
    "RocketLauncher", "GrenadeLauncher",
    "PlasmaRifle", "Railgun",
    "BFG9000", "BFG10k"
};

int unlockAmmo[UNLOCK_WEPCOUNT] =
{
    "<none>", "<none>",
    "Clip",
    "Shell", "Shell",
    "Clip", "Clip",
    "RocketAmmo", "RocketAmmo",
    "Cell", "Cell", "Cell", "Cell",
};

int unlockAmmoCount[UNLOCK_WEPCOUNT] =
{
    0, 0,
    30,
    12, 12,
    80, 180,
    8, 8,
    60, 80, 80, 60
};

int unlockMaxAmmo[UNLOCK_MAXAMMOCOUNT] = {"Clip", "Shell", "RocketAmmo", "Cell"};
int unlockMaxAmmoCount[UNLOCK_MAXAMMOCOUNT] = {200, 50, 50, 300};
