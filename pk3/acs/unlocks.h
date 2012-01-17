//#include "unlockNames.h"

#define UNLOCK_COUNT        11
#define ENTRY_COUNT         14
#define UNLOCK_ARMORCOUNT   3
#define UNLOCK_WEPCOUNT     13
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
    {GENERAL_ACTIVATE, 0, 0, 0, GENERAL_UNLOCK, 0,  10, 0,   -1, -1,             0,0,0, 1},
    {GENERAL_ACTIVATE, 1, 0, 0, GENERAL_UNLOCK, 1,  0.05, 0, 10, -1,             0,0,0, 1},
    {GENERAL_ACTIVATE, 2, 0, 0, GENERAL_UNLOCK, 2,  0.5, 0,  10, -1,             0,0,0, 1},
    {-1,               0, 0, 0, GENERAL_UNLOCK, 3,  1, 0,    5,  -1,             0,0,0, 1},
    {-1,               0, 0, 0, GENERAL_UNLOCK, 4,  1, 0,    5,  -1,             0,0,0, 1},
    {-1,               0, 0, 0, GENERAL_UNLOCK, 5,  1, 0,    5,  -1,             0,0,0, 1},
    {GENERAL_ACTIVATE, 6, 0, 0, GENERAL_UNLOCK, 6,  2,UNLOCK_WEPCOUNT-1,10,   GENERAL_CHECK, 0,3,3, 2},
    {GENERAL_ACTIVATE, 7, 0, 0, GENERAL_UNLOCK, 7,  UNLOCK_WEPCOUNT-1,0,  1,  GENERAL_CHECK, 0,6,10, 2},
    {-1              , 0, 0, 0, GENERAL_UNLOCK, 8,  1,0,     2,  SCALED_CHECK,   8,5,2, 2},
    {GENERAL_ACTIVATE, 9, 0, 0, GENERAL_UNLOCK, 9,  1,0,     3,  -1,             0,0,0, 3},
    {-1              , 0, 0, 0, GENERAL_UNLOCK, 10, 1,0,     3,  -1,             0,0,0, 2},
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

int unlockArmors[UNLOCK_ARMORCOUNT] =
{
    "GreenArmor",
    "BlueArmor",
    "RedArmor",
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
