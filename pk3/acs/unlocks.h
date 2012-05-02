//#include "unlockNames.h"

#define UNLOCK_COUNT        3
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
    {DOOMRL_GENERAL_APPLY, 1,0,0,   DOOMRL_GENERAL_UNLOCK, 1,1,0,   3,  -1, 0,0,0,  1},
    {DOOMRL_GENERAL_APPLY, 2,0,0,   DOOMRL_GENERAL_UNLOCK, 2,1,0,   3,  -1, 0,0,0,  1},
};

// This is in a seperate array because of ACS bugginess
int unlockNames[UNLOCK_COUNT] =
{
    "Ironman",
    "Hellrunner",
    "Jumpman"
};

// Line length should be max 50 chars

int unlockDescriptions[UNLOCK_COUNT] = 
{
// Ironman
"You're tough. You're tougher than tough. People\n\
bounce bullets off of you, and you don't care.\n\
For every level of this, your max health is\n\
increased by 20 percent.",
// Hellrunner
"You didn't start off as a child by walking. You\n\
ran. You've been running for as long as you can\n\
remember, and no one can out-race you. Every level\n\
of this makes you 15% faster.",
// Jumpman
"You might not be an Italian plumber, but you\n\
still have enough power in your legs to send you\n\
over a couple buildings. Every level of this\n\
increases your jump height by 2.",
};

int unlockQuotes[UNLOCK_COUNT] =
{
    "Who's a man and a half? I'm a man and a half!",
    "Lightspeed travel? Why do you ask?",
    "<@ijontichy> wtf is this, jumpmaze?",
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
