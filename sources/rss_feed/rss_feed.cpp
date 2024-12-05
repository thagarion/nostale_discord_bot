#include "rss_feed/rss_feed.hpp"

#include <curl/curl.h>
#include <regex>
#include <tinyxml2.h>

#include "rss_feed/rss_event.hpp"
#include "utils/time.hpp"

size_t RSSFeed::write_callback(void* contents, const size_t size, const size_t count, std::string* result) {
    result->append(static_cast<char*>(contents), size * count);
    return size * count;
}

std::string RSSFeed::fetch_feed() {
    std::string buffer;

    if (CURL* curl = curl_easy_init()) {
        curl_easy_setopt(curl, CURLOPT_URL, rss_feed_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        const CURLcode result = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if (result != CURLE_OK) {
            Bot::Log(dpp::ll_error, std::string("curl_easy_perform() failed: ") + curl_easy_strerror(result));
        }
    }

    return buffer;
}

std::string RSSFeed::fetch_gemini_result(const std::string& input) {
    std::string buffer;

    const auto payload = R"({
            "system_instruction": {
                "parts": {
                    "text": "1. Convert HTML to markdown.
2. You must not change the text.
3. You can use heading 2 or 3 level if needed (## or ###).
4. Convert tables into lists.
5. Dont use markdowns images syntax. Just put image link instead."
                }
            },
            "contents": {
                "parts": {
                    "text": ")" +
                         std::regex_replace(input, std::regex("\""), "\\\"") + R"("
                }
            }
        })";

    if (CURL* curl = curl_easy_init()) {
        curl_easy_setopt(curl, CURLOPT_URL, gemini_url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        const CURLcode result = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (result != CURLE_OK) {
            Bot::Log(dpp::ll_error, std::string("curl_easy_perform() failed: ") + curl_easy_strerror(result));
            return "";
        }
    }

    auto json_doc = nlohmann::json::parse(buffer);
    buffer = json_doc.at("candidates").at(0).at("content").at("parts").at(0).at("text").dump();
    buffer = std::regex_replace(buffer, std::regex("\\\\n"), "\n");
    buffer = std::regex_replace(buffer, std::regex("\""), "");
    buffer = std::regex_replace(buffer, std::regex("#{4,}"), "###");

    return buffer;
}

std::string RSSFeed::fetch_gemini_translate(const std::string& input) {
    std::string buffer;

    const auto payload = R"({
            "system_instruction": {
                "parts": {
                    "text": "1. Translate to Russian.
2. Change dates and time to MSK timezone.
3. Leave the names in the original language.
4. Do not change formatting."
                }
            },
            "contents": {
                "parts": {
                    "text": ")" +
                         std::regex_replace(input, std::regex("\""), "\\\"") + R"("
                }
            }
        })";

    if (CURL* curl = curl_easy_init()) {
        curl_easy_setopt(curl, CURLOPT_URL, gemini_url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        const CURLcode result = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (result != CURLE_OK) {
            Bot::Log(dpp::ll_error, std::string("curl_easy_perform() failed: ") + curl_easy_strerror(result));
            return "";
        }
    }

    auto json_doc = nlohmann::json::parse(buffer);
    buffer = json_doc.at("candidates").at(0).at("content").at("parts").at(0).at("text").dump();
    buffer = std::regex_replace(buffer, std::regex("\\\\n"), "\n");
    buffer = std::regex_replace(buffer, std::regex("\""), "");
    buffer = std::regex_replace(buffer, std::regex("#{4,}"), "###");

    return buffer;
}

void RSSFeed::parse(const std::string& data) {
    tinyxml2::XMLDocument doc;

    if (doc.Parse(data.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to parse RSS feed" << std::endl;
        return;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("rss");
    if (!root) {
        std::cerr << "No <rss> root element found!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* channel = root->FirstChildElement("channel");
    if (!channel) {
        std::cerr << "No <channel> element found!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* item = channel->FirstChildElement("item");

    while (item) {
        auto event = RSSEvent();

        if (const tinyxml2::XMLElement* element = item->FirstChildElement("title")) {
            event.set_title(element->GetText());
        } else {
            Bot::Log(dpp::ll_error, "RSS Event not contained title");
            continue;
        }

        if (const tinyxml2::XMLElement* element = item->FirstChildElement("link")) {
            event.set_link(element->GetText());
        } else {
            Bot::Log(dpp::ll_error, "RSS Event not contained link");
            continue;
        }

        if (const tinyxml2::XMLElement* element = item->FirstChildElement("pubDate")) {
            event.set_date(element->GetText());
        } else {
            Bot::Log(dpp::ll_error, "RSS Event not contained pubDate");
            continue;
        }

        if (*event.get_date() > last_event) {
            bool is_event_translated = false;
            auto event_translated = event;
            if (const tinyxml2::XMLElement* element = item->FirstChildElement("content:encoded")) {
                std::string content = element->GetText();
                auto result = parse_content(content);
                event.set_content(result);
                if (is_gemini_available) {
                    auto result_translated = fetch_gemini_translate(result);
                    event_translated.set_content(result_translated);
                    is_event_translated = true;
                }
            } else {
                Bot::Log(dpp::ll_error, "RSS Event not contained pubDate");
                continue;
            }
            Bot::SendNews(event);
            if (is_event_translated) {
                Bot::SendNewsTranslated(event_translated);
            }
            last_event = *event.get_date();
        } else {
            break;
        }
        item = item->NextSiblingElement("item");
    }
}

std::string RSSFeed::parse_content(std::string& text) {
    if (is_gemini_available) {
        return fetch_gemini_result(text);
    }
    return html2md::Convert(text);
}

void RSSFeed::listen() {
    Bot::Log(dpp::ll_info, "Start RSS Feed");

    last_event = *get_current_date_time_gmt_2();
    const int seconds_till_next_hour = (60 - last_event.tm_min) * 60 - last_event.tm_sec;

    std::this_thread::sleep_for(std::chrono::seconds(seconds_till_next_hour));

    while (!is_stopped) {
        const std::string rss_data = fetch_feed();
        parse(rss_data);

        std::this_thread::sleep_for(std::chrono::minutes(60));
    }
}