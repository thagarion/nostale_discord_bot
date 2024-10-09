#include "rss_feed/rss_event.hpp"

#include <algorithm>
#include <format>
#include <html2md.h>
#include <iostream>

#include "utils/time.hpp"

RSSEvent::RSSEvent(tinyxml2::XMLElement* item) {
    if (const tinyxml2::XMLElement* element = item->FirstChildElement("title")) {
        title = element->GetText();
    } else {
        has_error = true;
        return;
    }

    if (const tinyxml2::XMLElement* element = item->FirstChildElement("link")) {
        link = element->GetText();
    } else {
        has_error = true;
        return;
    }

    if (const tinyxml2::XMLElement* element = item->FirstChildElement("pubDate")) {
        date = element->GetText();
    } else {
        has_error = true;
        return;
    }

    if (const tinyxml2::XMLElement* element = item->FirstChildElement("content:encoded")) {
        const std::string text = html2md::Convert(element->GetText());

        size_t start = 0;
        while (start < text.size()) {
            const size_t end = std::min(start + max_message_symbols, text.size());

            size_t lastNewline = text.find_last_of('\n', end);
            if (lastNewline == std::string::npos || lastNewline <= start) {
                lastNewline = end;
            }

            content.push_back(text.substr(start, lastNewline - start));

            start = lastNewline + 1;
        }
        std::ranges::reverse(content);
    } else {
        has_error = true;
        return;
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
        text += std::format("## {} {} {}\n", title, part_number, date);
        text += std::format("{}\n", link);
        text += content[i];

        result.push_back(text);
    }
    return result;
}

std::tm RSSEvent::get_date_time() const {
    auto tm = std::tm();
    strptime(date.c_str(), "%a, %d %b %Y %H:%M:%S %z", &tm);
    return tm;
}