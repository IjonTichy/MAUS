//#include "unlockNames.h"

#define UNLOCK_COUNT        11
#define ENTRY_COUNT         14
#define UNLOCK_ARMORCOUNT   3
#define UNLOCK_WEPCOUNT     13
#define U_WC                UNLOCK_WEPCOUNT  // space saving
#define UNLOCK_MAXAMMOCOUNT 4

/* Unlockable contents:
 * activate script, arg 1, arg 2, arg 3,
 * unlock script, arg 1, arg 2, arg 3,
 * max, req. script, arg 1, arg 2, arg 3, cosmore t
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
    {GENERAL_ACTIVATE, 0, 0, 0, GENERAL_UNLOCK, 0,  10, 0,      -1, -1,             0,0,0,  1},
    {GENERAL_ACTIVATE, 1, 0, 0, GENERAL_UNLOCK, 1,  0.05, 0,    10, -1,             0,0,0,  1},
    {GENERAL_ACTIVATE, 2, 0, 0, GENERAL_UNLOCK, 2,  0.5, 0,     10, -1,             0,0,0,  1},
    {-1,               0, 0, 0, GENERAL_UNLOCK, 3,  1, 0,       5,  -1,             0,0,0,  1},
    {-1,               0, 0, 0, GENERAL_UNLOCK, 4,  1, 0,       5,  -1,             0,0,0,  1},
    {-1,               0, 0, 0, GENERAL_UNLOCK, 5,  1, 0,       5,  -1,             0,0,0,  1},
    {GENERAL_ACTIVATE, 6, 0, 0, GENERAL_UNLOCK, 6,  2, U_WC-1,  10, GENERAL_CHECK,  0,3,3,  2},
    {GENERAL_ACTIVATE, 7, 0, 0, GENERAL_UNLOCK, 7,  U_WC-1,0,   1,  GENERAL_CHECK,  0,6,10, 2},
    {-1              , 0, 0, 0, GENERAL_UNLOCK, 8,  1,0,        2,  SCALED_CHECK,   8,5,2,  2},
    {GENERAL_ACTIVATE, 9, 0, 0, GENERAL_UNLOCK, 9,  1,0,        3,  -1,             0,0,0,  3},
    {-1              , 0, 0, 0, GENERAL_UNLOCK, 10, 1,0,        3,  -1,             0,0,0,  2},
};

// This is in a seperate array because of ACS bugginess
int unlockNames[UNLOCK_COUNT] =
{
    "Max Health (+10)",
    "Speed (+5%)",
    "Jump Height (+0.5)",
    "Ammo Regen",
    "Health Regen",
    "Armor Regen",
    "Random New Weapon",
    "BFG10k",
    "Permanent Armor Upgrade",
    "Rune Togglers",
    "Ammo Leech",
};

// Line length should be max 50 chars

int unlockDescriptions[UNLOCK_COUNT] = 
{
    "Exactly what it says on the tin.",
    "Still exactly what it says on the tin.",
    "Yep, exactly what it says on the tin.",
"Regenerate ammo on a reciprocal scale. Clips\n\
regenerate the fastest along with cells, shells\n\
are medium, rockets are slow.",
    "Regenerate health on a reciprocal scale.",
    "Regenerate armor on an reciprocal scale.",
    "Get a random new weapon, barring the BFG10k.",
    "Get the BFG10k.",
"Your armor will be permanently upgraded. Level 1\n\
means your worst armor you can get is blue armor,\n\
level 2 means red armor.",
"Get items that will toggle runes on and off.\n\
Level 1 gives regen, prosperity, and speed; level\n\
2 gives rage, high jump, and protection; level 3\n\
gives spread, strength, and drain.",
"Whenever you get a kill on a monster, you will\n\
get ammo according to their health. When killing\n\
a player, you'll get their ammo, multiplied by\n\
(<level> * 0.2).",
};

int unlockQuotes[UNLOCK_COUNT] =
{
    "Why won't you DIE?!",
    "Fuck it, I'm outta here.",
    "<@ijontichy> wtf is this, jumpmaze?",
    "Oh, the humanity! My big gun is out of bullets!",
    "'Tis only a flesh wound.",
    "",
    "Jackpot!",
    "Dig the prowess, the capcity for violence!",
    "Never settle for less.",
    "",
    "That's your \caspinal cord\c-, baby! Dig it!",
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
int unlockMaxAmmoCount[UNLOCK_MAXAMMOCOUNT] = {300, 40, 20, 300};
