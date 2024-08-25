#pragma once

#include <string>
#include <map>
#include <utility>

typedef std::map<uint8_t, std::vector<uint8_t> > Schedule;

class Event {
protected:
    std::map<uint8_t, std::vector<uint8_t> > schedule;

    [[nodiscard]] virtual std::string to_string(uint8_t time) const = 0;

public:
    explicit Event(Schedule times) {
        schedule = std::move(times);
    }

    virtual ~Event() = default;

    [[nodiscard]] virtual std::string get_next() const = 0;
};

class LandOfDeathEvent : public Event {
    [[nodiscard]] std::string to_string(uint8_t time) const override {
        return std::format("Следующий лод будет в {} часа на {} канале", time, schedule.at(time)[0]);
    }

public:
    explicit LandOfDeathEvent(Schedule times) : Event(std::move(times)) {
    }
};
