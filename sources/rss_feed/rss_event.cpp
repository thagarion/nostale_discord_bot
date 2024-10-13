#include "rss_feed/rss_event.hpp"

#include <algorithm>
#include <format>
#include <iostream>

void RSSEvent::set_content(const std::string& text) {
    size_t start = 0;
    max_message_symbols = 1950 - std::format("# {} {} {}\n", title, "1", get_date_string()).size();

    if (text.size() <= max_message_symbols) {
        content.push_back(text);
        return;
    }

    while (start < text.size()) {
        const size_t end = std::min(start + max_message_symbols, text.size());

        size_t lastNewline = text.find_last_of('\n', end);
        if (lastNewline == std::string::npos || lastNewline <= start) {
            lastNewline = end;
        }

        content.push_back(text.substr(start, lastNewline - start));

        start = lastNewline + 1;
    }
}

std::vector<std::string> RSSEvent::to_string() const {
    std::vector<std::string> result;
    std::string part_number;
    for (int i = 0; i < content.size(); i++) {
        std::string text;
        if (content.size() > 1) {
            part_number = std::format("[{}/{}]", i + 1, content.size());
        }
        text += std::format("# {} {} {}\n", title, part_number, get_date_string());
        text += content[i];

        result.push_back(text);
    }
    return result;
}
