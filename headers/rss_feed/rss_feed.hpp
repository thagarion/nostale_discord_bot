#pragma once

#include <html2md.h>

#include "bot.hpp"

class RSSFeed {
    inline static bool is_stopped = false;
    inline const static bool is_gemini_available = !std::string(GEMINI_TOKEN).empty();

    inline static std::tm last_event = {};
    inline static const std::string rss_feed_url = "https://forum.nostale.gameforge.com/forum/board-feed/106/";
    inline static const std::string gemini_url =
        "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" +
        std::string(GEMINI_TOKEN);

    static size_t write_callback(void* contents, size_t size, size_t count, std::string* result);

    static std::string fetch_feed();
    static std::string fetch_gemini_result(const std::string& input);

    static void parse(const std::string& data);
    static std::string parse_content(std::string& text);

public:
    static void listen();
    static void stop() { is_stopped = true; }
};
