#include "event.hpp"

std::string LandOfLifeEvent::to_string(const std::tm& time) const {
    const std::time_t now = std::time(nullptr);
    std::tm* current_time = std::gmtime(&now);
    current_time->tm_hour += 2;

    std::tm event_time = *current_time;
    event_time.tm_hour = time.tm_hour;
    event_time.tm_min = time.tm_min;
    event_time.tm_sec = 0;
    const std::time_t event_time_t = std::mktime(&event_time);

    const auto remaining_time = std::chrono::seconds(event_time_t - std::mktime(current_time));

    std::tm msk_time = event_time;
    msk_time.tm_hour += 1;

    std::tm asgobas_time = msk_time;
    asgobas_time.tm_hour += 1;

    const auto dark_horn_remaining_time = remaining_time + std::chrono::hours(1);

    return std::format("Следующий ЛоЛ будет в {} ({})\nАсго в {} ({})\nКаналы: {}\n", time_to_string(msk_time),
                       remaining_time_to_string(remaining_time), time_to_string(asgobas_time),
                       remaining_time_to_string(dark_horn_remaining_time), channels_to_string(events.at(time)));
}