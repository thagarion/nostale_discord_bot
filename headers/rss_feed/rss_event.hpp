#pragma once

#include <format>
#include <regex>
#include <string>
#include <vector>

class RSSEvent {
    size_t max_message_symbols = 1500;

    std::string title;
    std::tm date{};
    std::vector<std::string> content;

public:
    RSSEvent() = default;

    void set_title(const std::string& value) { title = value; }
    void set_date(const std::string& value) { strptime(value.c_str(), "%a, %d %b %Y %H:%M:%S %z", &date); }
    void set_content(const std::string& text);

    [[nodiscard]] std::vector<std::string> to_string() const;
    [[nodiscard]] const std::tm* get_date() const { return &date; };
    [[nodiscard]] std::string get_date_string() const {
        return std::format("{:02}:{:02}", date.tm_hour, date.tm_min);
    }
};