#pragma once

#include <html2md.h>

#include "bot.hpp"

class RSSFeed {
    inline static std::tm last_event = {};
    inline static const std::string url = "https://forum.nostale.gameforge.com/forum/board-feed/106/";

    static size_t write_callback(void* contents, const size_t size, const size_t count, std::string* result);

    static std::string fetch(const std::string& url);

    static void parse(const std::string& data);

public:
    static void listen();
};
