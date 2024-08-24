#pragma once

#include <string>
#include <map>
#include <utility>

class Event {
    std::map<uint8_t, uint8_t> schedule;

    [[nodiscard]] virtual std::string to_string(size_t index) const = 0;

public:
    explicit Event(std::map<uint8_t, uint8_t> times) {
        schedule = std::move(times);
    }

    virtual ~Event() = default;

    [[nodiscard]] virtual std::string get_next() const = 0;
};

class LandOfDeathEvent : public Event {
    [[nodiscard]] std::string to_string(size_t index) const override {
        return std::format("Следующий лод будет в {} часа на {} канале");
    }

public:
    explicit LandOfDeathEvent(std::map<uint8_t, uint8_t> times) : Event(std::move(times)) {}
};
