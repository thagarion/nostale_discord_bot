#include <thread>

#include "bot.hpp"
#include "rss_feed/rss_feed.hpp"

int main() {
    std::thread bot(Bot::Init);
    std::thread rss_thread(RSSFeed::listen);
    bot.join();
    rss_thread.join();
}
