#pragma once

#include <string>
#include <map>
#include <utility>
#include <chrono>
#include <date/date.h>
#include <date/tz.h>

typedef std::map<uint8_t, std::vector<uint8_t> > Schedule;

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

    [[nodiscard]] virtual std::string to_string(uint8_t time) const = 0;

    [[nodiscard]] static std::string time_to_string(const tm &time) {
        return std::format("{:%H:%M:%S}", time.tm_zone);
    }

    [[nodiscard]] static std::string remaining_time_to_string(long hours, long minutes, long seconds) {
        std::string hour_str = get_plural_form(hours, "час", "часа", "часов");
        std::string minute_str = get_plural_form(minutes, "минута", "минуты", "минут");
        std::string second_str = get_plural_form(seconds, "секунда", "секунды", "секунд");
        return std::format("Осталось {} {} {} {} {} {}", hours, hour_str, minutes, minute_str, seconds, second_str);
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
    [[nodiscard]] std::string to_string(const uint8_t time) const override {
        const std::time_t now = std::time(nullptr);
        std::tm *current_time = std::gmtime(&now);
        current_time->tm_hour += 2;
        // std::mktime(current_time);

        std::tm event_time = *current_time;
        event_time.tm_hour = time;
        event_time.tm_min = 0;
        event_time.tm_sec = 0;
        const std::time_t event_time_t = std::mktime(&event_time);

        auto remaining_time = std::chrono::seconds(event_time_t - std::mktime(current_time));

        std::tm msk_time = event_time;
        msk_time.tm_hour += 1;

        std::tm dark_horn_time = msk_time;
        dark_horn_time.tm_hour += 1;

        std::cout << "Event time (MSK): " << std::put_time(&msk_time, "%Y-%m-%d %H:%M:%S") << std::endl;

        const auto hours = std::chrono::duration_cast<std::chrono::hours>(remaining_time);
        remaining_time -= hours;
        const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(remaining_time);
        remaining_time -= minutes;
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(remaining_time);

        return std::format("Следующий лод будет в {} ({})\nДХ в {} ({})\nКаналы: {}\n",
                           time_to_string(msk_time),
                           remaining_time_to_string(hours.count(), minutes.count(), seconds.count()),
                           time_to_string(dark_horn_time),
                           remaining_time_to_string(hours.count() - 1, minutes.count(), seconds.count()),
                           events.at(time)[0]);
    }

public:
    explicit LandOfDeathEvent(Schedule times) : Event(std::move(times)) {
    }
};
