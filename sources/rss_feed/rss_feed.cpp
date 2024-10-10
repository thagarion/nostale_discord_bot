#include "rss_feed/rss_feed.hpp"

#include <curl/curl.h>
#include <tinyxml2.h>

#include "rss_feed/rss_event.hpp"
#include "utils/time.hpp"

size_t RSSFeed::write_callback(void* contents, const size_t size, const size_t count, std::string* result) {
    result->append(static_cast<char*>(contents), size * count);
    return size * count;
}

std::string RSSFeed::fetch(const std::string& url) {
    std::string buffer;

    if (CURL* curl = curl_easy_init()) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
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
        auto event = RSSEvent(item);
        if (event.is_valid() && event.get_date_time() > last_event) {
            Bot::SensNews(event);
            last_event = event.get_date_time();
        } else {
            break;
        }
        item = item->NextSiblingElement("item");
    }
}

void RSSFeed::listen() {
    Bot::Log(dpp::ll_info, "Start RSS Feed");

    last_event = *get_current_date_time_gmt_2();
    const int seconds_till_next_hour = (60 - last_event.tm_min) * 60 - last_event.tm_sec;

    std::this_thread::sleep_for(std::chrono::seconds(seconds_till_next_hour));

    while (true) {
        const std::string rss_data = fetch(url);
        parse(rss_data);

        std::this_thread::sleep_for(std::chrono::minutes(60));
    }
}