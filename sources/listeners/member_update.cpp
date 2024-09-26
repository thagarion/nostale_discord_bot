#include <listeners.hpp>

void listener::on_guild_member_update(const dpp::guild_member_update_t& update) {

    const auto name = update.updated.get_nickname();

    const auto start = name.find_first_of('[') + 1;
    const auto end = name.find_last_of(']');
    if (start == std::string::npos || end == std::string::npos || end <= start) {
        // bot.log(dpp::ll_error, std::format("Can not parse levels in name [{}]", name));
        return;
    }
    auto levels_string = name.substr(start, end - start);
    for (auto i = levels_string.find_first_of('/'); i != std::string::npos; i = levels_string.find_first_of('/')) {
        auto level_string = levels_string.substr(0, i);
        if (levels_string.front() == 'C' || levels_string.front() == 'c') {
            level_string = level_string.substr(1, levels_string.size());
            // bot.log(dpp::ll_debug, "got clevel = " + level_string);
        } else {
            // bot.log(dpp::ll_debug, "got level = " + level_string);
        }
        levels_string = levels_string.substr(i + 1, levels_string.size());
    }
    // bot.log(dpp::ll_debug, "got number = " + levels_string);
}
