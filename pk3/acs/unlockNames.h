#define UNLOCK_BASE             450
#define UNLOCK_HBASE            21900
#define UNLOCK_HBASE2           11900
#define UNLOCK_HBASE3           16900

#define UNLOCK_ENTER            UNLOCK_BASE-1
#define UNLOCK_DEATH            UNLOCK_BASE-2
#define UNLOCK_RESPAWN          UNLOCK_BASE-3
#define UNLOCK_DISCONNECT       UNLOCK_BASE-4
#define UNLOCK_REGEN            UNLOCK_BASE-5

#define GENERAL_ACTIVATE        UNLOCK_BASE+1
#define GENERAL_UNLOCK          UNLOCK_BASE+2
#define GENERAL_CHECK           UNLOCK_BASE+3
#define SCALED_CHECK            UNLOCK_BASE+4

#define UNLOCK_MENU             UNLOCK_BASE+21
#define UNLOCK_LEVELHUD         UNLOCK_BASE+22

#define LEVEL_ADDXP             UNLOCK_BASE+31
#define LEVEL_RECALC            UNLOCK_BASE+32
#define LEVEL_ADDXP_ONKILL      UNLOCK_BASE+39

#define PUKE_UNLOCKANDUSE       UNLOCK_BASE+51
#define PUKE_REPORT             UNLOCK_BASE+52

#define MENU_REFRESH 70
#define HUD_REFRESH  150

#define MENU_USESPEED       10
#define MENU_UPSPEED        8
#define MENU_DOWNSPEED      8
#define MENU_SIZE           10
#define MENU_SPACING        11.0

#define AMMO_CLIPRATE       55
#define AMMO_SHELLRATE      245
#define AMMO_ROCKETRATE     300
#define AMMO_CELLRATE       70

#define HEALTH_REGENRATE    61
#define ARMOR_REGENRATE     81

#define STAT_COUNT              12

#define STAT_HP                 0
#define STAT_SPEED              1
#define STAT_JUMPZ              2
#define STAT_XP                 3
#define STAT_TOTALXP            4
#define STAT_LEVEL              5
#define STAT_NEXTL              6
#define STAT_WEPS               7
#define STAT_AMMO_REGENRATE     8
#define STAT_HP_REGENRATE       9
#define STAT_ARMOR_REGENRATE    10
#define STAT_ARMORLEVEL         11
