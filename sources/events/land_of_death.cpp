#include <ranges>

#include "events/event.hpp"
#include "utils/time.hpp"

std::string LandOfDeathEvent::get_next() const {
    const auto current_time = get_current_time_gmt_2();

    for (const auto& time : std::views::keys(events)) {
        auto dark_horn_time = time;
        dark_horn_time.tm_hour += 1;
        if (dark_horn_time.tm_hour > 23) {
            dark_horn_time.tm_hour -= 24;
        }

        if (time > current_time) {
            return to_string(time);
        }
        if (dark_horn_time > current_time) {
            return to_string_current(time);
        }
    }
    return to_string(events.cbegin()->first);
}

std::string LandOfDeathEvent::to_string(const std::tm& time) const {
    auto current_time = get_current_date_time_mks();

    std::tm event_time = time;
    std::tm event_msk = get_date_time_mks(&event_time);

    const auto remaining_time = std::chrono::seconds(std::mktime(&event_msk) - std::mktime(&current_time));

    event_time.tm_hour += 1;

    std::tm dark_horn_msk = get_date_time_mks(&event_time);

    const auto dark_horn_remaining_time =
        std::chrono::seconds(std::mktime(&dark_horn_msk) - std::mktime(&current_time));

    return std::format("Следующий ЛоД будет в {} ({})\nДХ в {} ({})\nКаналы: {}", time_to_string(event_msk),
                       remaining_time_to_string(remaining_time), time_to_string(dark_horn_msk),
                       remaining_time_to_string(dark_horn_remaining_time), channels_to_string(events.at(time)));
}

std::string LandOfDeathEvent::to_string_current(const std::tm& time) const {
    auto current_time = get_current_date_time_mks();

    std::tm event_time = time;
    std::tm event_msk = get_date_time_mks(&event_time);

    const auto past_time = -std::chrono::seconds(std::mktime(&event_msk) - std::mktime(&current_time));

    event_time.tm_hour += 1;

    std::tm dark_horn_msk = get_date_time_mks(&event_time);

    const auto dark_horn_remaining_time =
        std::chrono::seconds(std::mktime(&dark_horn_msk) - std::mktime(&current_time));

    return std::format("ЛоД начался в {} ({})\nДХ в {} ({})\nКаналы: {}", time_to_string(event_msk),
                       past_time_to_string(past_time), time_to_string(dark_horn_msk),
                       remaining_time_to_string(dark_horn_remaining_time), channels_to_string(events.at(time)));
}
