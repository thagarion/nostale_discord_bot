#include "rss_feed/rss_event.hpp"

#include <algorithm>
#include <regex>
#include <string>

void RSSEvent::set_content(std::string& text) {
    std::map<size_t, std::string> links;
    max_message_symbols = 2000 - std::format("# {}\n{}\n", title, get_date_string()).size();

    const std::regex link_regex(R"(https?:\/\/[^\s'"]+\.(pdf|zip|docx|jpg|png))");

    std::smatch match;
    auto it = text.cbegin();
    size_t offset = 0;

    while (std::regex_search(it, text.cend(), match, link_regex)) {
        size_t index = match.position();
        std::string link = match[0];
        links[index] = link;
        text.erase(index, link.length());

        offset += link.length();
        it = text.cbegin();
    }

    if (links.size() <= 1 && text.size() <= max_message_symbols) {
        ContentItem item;
        item.first = text;
        if (links.size() == 1) {
            item.second = links[0];
        }
        content.push_back(item);
        return;
    }

    size_t start = 0;
    for (const auto& [index, link] : links) {
        if ((index - start) < max_message_symbols) {
            ContentItem item;
            item.first = text.substr(start, index);
            content.push_back(item);
        } else {
            auto text_part = text.substr(start, index);
            auto part_start = 0;
            while (part_start < text_part.size()) {
                const size_t end = std::min(part_start + max_message_symbols, text.size());
                size_t last_newline = text.find_last_of('\n', end);
                if (last_newline == std::string::npos || last_newline <= start) {
                    last_newline = end;
                }
                ContentItem item;
                item.first = text_part.substr(part_start, last_newline - part_start);
                content.push_back(item);

                part_start = static_cast<int>(last_newline) + 1;
            }
        }
        start = index + 1;
        content.back().second = link;
    }
}

ContentVector RSSEvent::get_content() {
    std::string part_number;
    for (int i = 0; i < content.size(); i++) {
        if (i == 0) {
            content[i].first = std::format("# {}\n{}\n{}\n{}", title, get_date_string(), link, content[i].first);
        }
        content[i].first = std::regex_replace(content[i].first, std::regex("(\\n){2,}"), "\n");
    }
    return content;
}
