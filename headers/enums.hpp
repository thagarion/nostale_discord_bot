#pragma once

#define PING_COMMAND "ping"
#define TIME_COMMAND "time"
#define CONF_COMMAND "conf"

#define IC_EVENT "ic"
#define AIC_EVENT "asgo"
#define LOD_EVENT "lod"
#define LOL_EVENT "lol"

#define MARA_CONF "mara"
#define NEWS_CONF "news"
#define NEWS_TR_CONF "news_tr"
#define ROLES_CONF "roles"

enum Level_Roles {
    LVL85_ROLE,
    CLVL1_ROLE,
    CLVL10_ROLE,
    CLVL20_ROLE,
    CLVL30_ROLE,
    CLVL40_ROLE,
    CLVL50_ROLE,
    CLVL60_ROLE,
    CLVL70_ROLE,
    CLVL80_ROLE,
    CLVL90_ROLE
};
inline std::map<Level_Roles, std::string> roles_to_string = {
    {LVL85_ROLE, "L85-"},  {CLVL1_ROLE, "C10-"},  {CLVL10_ROLE, "C10+"}, {CLVL20_ROLE, "C20+"},
    {CLVL30_ROLE, "C30+"}, {CLVL40_ROLE, "C40+"}, {CLVL50_ROLE, "C50+"}, {CLVL60_ROLE, "C60+"},
    {CLVL70_ROLE, "C70+"}, {CLVL80_ROLE, "C80+"}, {CLVL90_ROLE, "C90+"}};

