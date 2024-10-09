#pragma once

#include <regex>
#include <string>
#include <tinyxml2.h>
#include <vector>

class RSSEvent {
    const size_t max_message_symbols = 1600;

    std::string title;
    std::string link;
    std::string date;
    std::vector<std::string> content;

    bool has_error = false;

public:
    explicit RSSEvent(tinyxml2::XMLElement* item);

    [[nodiscard]] bool is_valid() const { return !has_error; }
    [[nodiscard]] std::vector<std::string> to_string() const;
    [[nodiscard]] std::tm get_date_time() const;
};