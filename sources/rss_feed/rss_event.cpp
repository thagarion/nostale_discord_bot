#include "rss_feed/rss_event.hpp"

#include <algorithm>
#include <regex>
#include <string>

void RSSEvent::set_content(std::string& text) {
    content.clear();
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

    size_t start = 0;
    const size_t text_size = text.size();
    auto link_it = links.begin();

    while (start < text_size) {
        size_t end = std::min(start + max_message_symbols, text_size);
        std::string associated_link;

        if (link_it != links.end() && link_it->first < static_cast<int>(end)) {
            end = link_it->first + 1;
            associated_link = link_it->second;
        } else {
            const size_t newline_pos = text.rfind('\n', end - 1);
            if (newline_pos != std::string::npos && newline_pos >= start) {
                end = newline_pos + 1;
            }
        }

        content.emplace_back(text.substr(start, end - start), associated_link);
        start = end;

        while (link_it != links.end() && link_it->first < static_cast<int>(start)) {
            ++link_it;
        }
    }

    content[0].first = std::format("# {}\n{}\n{}\n{}", title, get_date_string(), link, content[0].first);
}

const ContentVector* RSSEvent::get_content() const { return &content; }
