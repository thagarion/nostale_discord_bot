#pragma once

#include <dpp/dpp.h>

class listener {

public:

    static void on_guild_member_update(const dpp::guild_member_update_t& update);

};