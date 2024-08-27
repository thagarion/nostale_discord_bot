#pragma once

#include <string>
#include <map>
#include <utility>
#include <chrono>
#include <date/date.h>
#include <date/tz.h>

typedef std::map<uint8_t, std::vector<uint8_t> > Schedule;

class Event {
protected:
    Schedule events;

    [[nodiscard]] virtual std::string to_string(uint8_t time) const = 0;

public:
    explicit Event(Schedule times) {
        events = std::move(times);
    }

    virtual ~Event() = default;

    [[nodiscard]] virtual std::string get_next() const {
        const std::time_t now = std::time(nullptr);
        const std::tm* current_time = std::gmtime(&now);
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
    [[nodiscard]] std::string to_string(uint8_t time) const override {
        std::time_t now = std::time(nullptr);
        std::tm* current_time = std::gmtime(&now);
        current_time->tm_hour += 2;
        std::mktime(current_time);

        std::tm event_time = *current_time;
        event_time.tm_hour = time;
        event_time.tm_min = 0;
        event_time.tm_sec = 0;
        const std::time_t event_time_t = std::mktime(&event_time);

        auto remaining_time = std::chrono::seconds(event_time_t - std::mktime(current_time));

        std::tm* msk_time = std::gmtime(&event_time_t);
        msk_time->tm_hour += 3;
        std::mktime(msk_time);

        std::cout << "Event time (MSK): " << std::put_time(msk_time, "%Y-%m-%d %H:%M:%S") << std::endl;

        auto hours = std::chrono::duration_cast<std::chrono::hours>(remaining_time);
        remaining_time -= hours;
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(remaining_time);
        remaining_time -= minutes;
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(remaining_time);

        std::cout << "Remaining time: " << hours.count() << " hours, "
                  << minutes.count() << " minutes, " << seconds.count() << " seconds."
                  << std::endl;

        return std::format("Следующий лод будет в {} часа на {} канале. Осталось ", time, events.at(time)[0]);
    }

public:
    explicit LandOfDeathEvent(Schedule times) : Event(std::move(times)) {
    }
};
