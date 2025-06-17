#pragma once

#include <dpp.h>
#include <format>
#include <string>
#include <vector>

typedef std::pair<std::string, std::string> ContentItem;
typedef std::vector<ContentItem> ContentVector;

class RSSEvent {
    size_t max_message_symbols = 1500;

    std::string title;
    std::string link;
    std::tm date{};
    ContentVector content;

public:
    RSSEvent() = default;

    void set_title(const std::string& value) { title = value; }
    void set_link(const std::string& value) { link = value; }
    void set_date(const std::string& value) { strptime(value.c_str(), "%a, %d %b %Y %H:%M:%S %z", &date); }
    void set_content(std::string& text);

    [[nodiscard]] const ContentVector* get_content() const;
    [[nodiscard]] std::tm* get_date() { return &date; };
    [[nodiscard]] std::string get_title() const { return title; };
    [[nodiscard]] std::string get_date_string() const {
        return std::format("Publication Time: {:02}:{:02} CEST", date.tm_hour, date.tm_min);
    }
};