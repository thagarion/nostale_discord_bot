#include <iostream>
#include <ranges>

#include "events.hpp"

std::string Event::get_plural_form(const long value, const std::string& one, const std::string& few,
                                   const std::string& many) {
    if (value % 10 == 1 && value % 100 != 11) {
        return one;
    }
    if (value % 10 >= 2 && value % 10 <= 4 && (value % 100 < 10 || value % 100 >= 20)) {
        return few;
    }
    return many;
}

std::string Event::channels_to_string(const std::vector<int>& channels) {
    std::stringstream ss;
    auto it = channels.cbegin();
    for (; it != channels.cend() - 1; ++it) {
        ss << *it << ", ";
    }
    ss << *it;
    return ss.str();
}

std::string Event::time_to_string(const tm& time) { return std::format("{:02}:{:02}", time.tm_hour, time.tm_min); }

std::string Event::remaining_time_to_string(std::chrono::seconds time) {
    const auto hours = std::chrono::duration_cast<std::chrono::hours>(time);
    time -= hours;
    const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(time);
    time -= minutes;
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time);

    std::string hour_str = get_plural_form(hours.count(), "час", "часа", "часов");
    std::string minute_str = get_plural_form(minutes.count(), "минута", "минуты", "минут");
    std::string second_str = get_plural_form(seconds.count(), "секунда", "секунды", "секунд");
    return std::format("Осталось {} {} {} {} {} {}", hours.count(), hour_str, minutes.count(), minute_str,
                       seconds.count(), second_str);
}

std::string Event::get_next() const {
    const std::time_t now = std::time(nullptr);
    std::tm* current_time = std::gmtime(&now);

    current_time->tm_hour += 1;
    if (current_time->tm_mon >= 2 && current_time->tm_mon <= 9) {
        current_time->tm_hour += 1;
        if (current_time->tm_hour >= 24) {
            current_time->tm_hour -= 24;
        }
    }

    for (const auto& time : std::views::keys(events)) {
        if (time > *current_time) {
            return to_string(time);
        }
    }
    return to_string(events.cbegin()->first);
}
