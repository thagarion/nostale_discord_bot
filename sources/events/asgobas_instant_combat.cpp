#include "events/event.hpp"
#include "utils/time.hpp"

std::string AsgobasInstantCombatEvent::to_string(const std::tm& time) const {
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
    if (msk_time.tm_hour > 23) {
        msk_time.tm_hour -= 24;
    }

    return std::format("Следующая Асго ББ будет в {} ({})\nКаналы: {}", time_to_string(msk_time),
                       remaining_time_to_string(remaining_time), channels_to_string(events.at(time)));
}