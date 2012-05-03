// Core stuff
#define UNLOCK_BASE             450
#define UNLOCK_HBASE            21900
#define UNLOCK_HBASE2           11900
#define UNLOCK_HBASE3           16900

#define UNLOCK_ENTER            UNLOCK_BASE-1
#define UNLOCK_DEATH            UNLOCK_BASE-2
#define UNLOCK_RESPAWN          UNLOCK_BASE-3
#define UNLOCK_DISCONNECT       UNLOCK_BASE-4

#define UNLOCK_MENU             UNLOCK_BASE+21
#define UNLOCK_LEVELHUD         UNLOCK_BASE+22
#define UNLOCK_NAG              UNLOCK_BASE+23

#define LEVELEFFECT_MAX         5

#define LEVEL_ADDXP             UNLOCK_BASE+31
#define LEVEL_RECALC            UNLOCK_BASE+32
#define LEVEL_ADDXP_ONKILL      UNLOCK_BASE+39

#define MENU_REFRESH 70
#define HUD_REFRESH  150

#define MENU_USESPEED       10
#define MENU_UPSPEED        8
#define MENU_DOWNSPEED      8
#define MENU_SIZE           10
#define MENU_SPACING        11.0

// Hooks
#define UNLOCK_HOOK_ENTER      UNLOCK_BASE + 101
#define UNLOCK_HOOK_RESPAWN    UNLOCK_BASE + 102
#define UNLOCK_HOOK_DAEMON     UNLOCK_BASE + 103
#define UNLOCK_HOOK_ONKILL     UNLOCK_BASE + 104

// Stats
#define STAT_COUNT  14

// !!! REQUIRED
#define STAT_LEVEL      0
#define STAT_XP         1
#define STAT_TOTALXP    2
#define STAT_NEXTL      3
#define STAT_CURSTREAK  4
#define STAT_KILLCOUNT  5
#define STAT_HP         6

#define STAT_IRONMAN        7
#define STAT_HELLRUNNER     8
#define STAT_JUMPMAN        9
#define STAT_SONOFABITCH    10
#define STAT_TOUGHASNAILS   11
#define STAT_SONOFAGUN      12
#define STAT_EAGLEEYE       13


// Non-core scripts

#define DOOMRL_GENERAL_APPLY    UNLOCK_BASE - 11
#define DOOMRL_GENERAL_UNLOCK   UNLOCK_BASE - 12
#define DOOMRL_GENERAL_CHECK    UNLOCK_BASE - 13

#define DOOMRL_DAMAGE           UNLOCK_BASE - 14
#define DOOMRL_GETSTAT          UNLOCK_BASE - 15
#define DOOMRL_ACCURACY         UNLOCK_BASE - 16
