#pragma once

#include <string>
#include <map>
#include <utility>
#include <chrono>

typedef std::map<int, std::vector<int> > Schedule;

class Event {
    [[nodiscard]] static std::string get_plural_form(const long value, const std::string &one, const std::string &few,
                                                     const std::string &many) {
        if (value % 10 == 1 && value % 100 != 11) {
            return one;
        }
        if (value % 10 >= 2 && value % 10 <= 4 && (value % 100 < 10 || value % 100 >= 20)) {
            return few;
        }
        return many;
    }

protected:
    Schedule events;

    [[nodiscard]] virtual std::string to_string(int time) const = 0;

    [[nodiscard]] static std::string channels_to_string(const std::vector<int> &channels) {
        std::stringstream ss;
        auto it = channels.cbegin();
        for (; it != channels.cend() - 1; ++it) {
            ss << *it << ", ";
        }
        ss << *it;
        std::cout << ss.str();
        return ss.str();
    }

    [[nodiscard]] static std::string time_to_string(const tm &time) {
        return std::format("{:02}:{:02}", time.tm_hour, time.tm_min);
    }

    [[nodiscard]] static std::string remaining_time_to_string(std::chrono::seconds time) {
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

public:
    explicit Event(Schedule times) {
        events = std::move(times);
    }

    virtual ~Event() = default;

    [[nodiscard]] virtual std::string get_next() const {
        const std::time_t now = std::time(nullptr);
        const std::tm *current_time = std::gmtime(&now);
        const auto server_hour = current_time->tm_hour + 2;
        for (const auto &hour: std::views::keys(events)) {
            if (hour > server_hour) {
                return to_string(hour);
            }
        }
        return to_string(events.cbegin()->first);
    };
};

class LandOfDeathEvent final : public Event {
protected:
    [[nodiscard]] std::string to_string(const int time) const override {
        const std::time_t now = std::time(nullptr);
        std::tm *current_time = std::gmtime(&now);
        current_time->tm_hour += 2;

        std::tm event_time = *current_time;
        event_time.tm_hour = time;
        event_time.tm_min = 0;
        event_time.tm_sec = 0;
        const std::time_t event_time_t = std::mktime(&event_time);

        const auto remaining_time = std::chrono::seconds(event_time_t - std::mktime(current_time));

        std::tm msk_time = event_time;
        msk_time.tm_hour += 1;

        std::tm dark_horn_time = msk_time;
        dark_horn_time.tm_hour += 1;

        const auto dark_horn_remaining_time = remaining_time + std::chrono::hours(1);

        return std::format("Следующий лод будет в {} ({})\nДХ в {} ({})\nКаналы: {}\n",
                           time_to_string(msk_time),
                           remaining_time_to_string(remaining_time),
                           time_to_string(dark_horn_time),
                           remaining_time_to_string(dark_horn_remaining_time),
                           channels_to_string(events.at(time)));
    }

public:
    explicit LandOfDeathEvent(Schedule times) : Event(std::move(times)) {
    }
};
