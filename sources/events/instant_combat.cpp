#include "events/event.hpp"
#include "utils/time.hpp"

std::string InstantCombatEvent::to_string(const std::tm& time) const {
    auto current_time = get_current_date_time_mks();

    const std::tm event_time = time;
    std::tm event_msk = get_date_time_mks(&event_time);

    const auto remaining_time = std::chrono::seconds(std::mktime(&event_msk) - std::mktime(&current_time));

    return std::format("Следующая ББ будет в {} ({})\nКаналы: {}", time_to_string(event_msk),
                       remaining_time_to_string(remaining_time), channels_to_string(events.at(time)));
}