#pragma once

#include <dpp/dpp.h>

#include "configs/config.hpp"
#include "logger/logger.hpp"
#include "rss_feed/rss_event.hpp"

using log_level = dpp::loglevel;

class Bot {
    inline static std::unique_ptr<dpp::cluster> bot_ptr = nullptr;
    inline static auto config = Config("configs");
    inline static auto logger = Logger();

    inline static std::vector<uint64_t> updated_users = {};

    static void on_log(const dpp::log_t& log);
    static void on_ready(const dpp::ready_t& event);
    static void on_slashcommand(const dpp::slashcommand_t& event);
    static void on_autocomplete(const dpp::autocomplete_t& event);
    static void on_guild_member_update(const dpp::guild_member_update_t& update);

public:
    static void Init();
    static void Log(log_level level, const std::string& message);
    static void SendNews(RSSEvent& event);
};