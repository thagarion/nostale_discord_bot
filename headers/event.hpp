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
        const auto now = std::chrono::system_clock::now();
        const auto server_time = date::make_zoned("Europe/Berlin", now);
        const auto local_time = server_time.get_local_time();
        const auto server_hour = date::floor<std::chrono::hours>(local_time).time_since_epoch().count() % 24;
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
        return std::format("Следующий лод будет в {} часа на {} канале", time, events.at(time)[0]);
    }

public:
    explicit LandOfDeathEvent(Schedule times) : Event(std::move(times)) {
    }
};
