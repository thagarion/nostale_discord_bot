#include <ranges>

#include "events/event.hpp"
#include "utils/time.hpp"

std::string LandOfLifeEvent::get_next() const {
    const auto current_time = get_current_time_gmt_2();

    for (const auto& time : std::views::keys(events)) {
        auto asgobas_time = time;
        asgobas_time.tm_hour += 1;
        if (asgobas_time.tm_hour > 23) {
            asgobas_time.tm_hour -= 24;
        }

        if (time > current_time) {
            return to_string(time);
        }
        if (asgobas_time > current_time) {
            return to_string_current(time);
        }
    }
    return to_string(events.cbegin()->first);
}

std::string LandOfLifeEvent::to_string(const std::tm& time) const {
    const std::time_t now = std::time(nullptr);
    std::tm* current_time = std::gmtime(&now);
    current_time->tm_hour += 2;

    std::tm event_time = *current_time;
    event_time.tm_hour = time.tm_hour;
    event_time.tm_min = time.tm_min;
    event_time.tm_sec = 0;

    auto remaining_time = std::chrono::seconds(std::mktime(&event_time) - std::mktime(current_time));
    if (remaining_time.count() < 0) {
        event_time.tm_mday += 1;
        remaining_time = std::chrono::seconds(std::mktime(&event_time) - std::mktime(current_time));
    }

    std::tm msk_time = event_time;
    msk_time.tm_hour += 1;

    std::tm asgobas_time = msk_time;
    asgobas_time.tm_hour += 1;

    const auto dark_horn_remaining_time = remaining_time + std::chrono::hours(1);

    return std::format("Следующий ЛоЛ будет в {} ({})\nАсго в {} ({})\nКаналы: {}", time_to_string(msk_time),
                       remaining_time_to_string(remaining_time), time_to_string(asgobas_time),
                       remaining_time_to_string(dark_horn_remaining_time), channels_to_string(events.at(time)));
}

std::string LandOfLifeEvent::to_string_current(const std::tm& time) const {
    const std::time_t now = std::time(nullptr);
    std::tm* current_time = std::gmtime(&now);
    current_time->tm_hour += 2;

    std::tm event_time = *current_time;
    event_time.tm_hour = time.tm_hour;
    event_time.tm_min = time.tm_min;
    event_time.tm_sec = 0;

    const auto past_time = -std::chrono::seconds(std::mktime(&event_time) - std::mktime(current_time));

    std::tm msk_time = event_time;
    msk_time.tm_hour += 1;

    std::tm asgobas_time = msk_time;
    asgobas_time.tm_hour += 1;

    const auto asgobas_remaining_time = std::chrono::seconds(std::mktime(&msk_time) - std::mktime(current_time));

    return std::format("Лол начался в {} ({})\nАсго в {} ({})\nКаналы: {}", time_to_string(msk_time),
                       past_time_to_string(past_time), time_to_string(asgobas_time),
                       remaining_time_to_string(asgobas_remaining_time), channels_to_string(events.at(time)));
}
