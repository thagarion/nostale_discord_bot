#pragma once

#include <html2md.h>
#include <sqlite3.h>

#include "bot.hpp"

class RSSFeed {
    inline static bool is_stopped = false;
    inline const static bool is_gemini_available = !std::string(GEMINI_TOKEN).empty();

    inline static std::tm last_event = {};
    inline static const std::string rss_feed_url = "https://forum.nostale.gameforge.com/forum/board-feed/106/";
    inline static const std::string gemini_url =
        "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash-preview-05-20:generateContent?key=" +
        std::string(GEMINI_TOKEN);

    inline static sqlite3* data_base = nullptr;

    static size_t write_callback(void* contents, size_t size, size_t count, std::string* result);

    static std::string fetch_feed();
    static std::string fetch_gemini_result(const std::string& input);
    static std::string fetch_gemini_translate(const std::string& input);

    static void parse(const std::string& data);
    static std::string parse_content(std::string& text);
    static void store_to_sqlite3(int date, std::string title, std::string original, std::string formatted, std::string translated);

public:
    static void listen();
    static void stop() { is_stopped = true; }
};
