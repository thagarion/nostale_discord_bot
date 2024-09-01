#pragma once

#include <chrono>
#include <map>
#include <string>
#include <utility>

inline bool operator<(const std::tm& lhs, const std::tm& rhs) {
    if (lhs.tm_hour < rhs.tm_hour) return true;
    if (lhs.tm_min < rhs.tm_min) return true;
    if (lhs.tm_sec < rhs.tm_sec) return true;
    return false;
}

inline bool operator>(const std::tm& lhs, const std::tm& rhs) {
    if (lhs.tm_hour > rhs.tm_hour) return true;
    if (lhs.tm_min > rhs.tm_min) return true;
    if (lhs.tm_sec > rhs.tm_sec) return true;
    return false;
}

typedef std::map<std::tm, std::vector<int> > Schedule;

class Event {
    [[nodiscard]] static std::string get_plural_form(long value, const std::string& one, const std::string& few,
                                                     const std::string& many);

protected:
    Schedule events;

    [[nodiscard]] virtual std::string to_string(const std::tm& time) const = 0;

    [[nodiscard]] static std::string channels_to_string(const std::vector<int>& channels);

    [[nodiscard]] static std::string time_to_string(const tm& time);

    [[nodiscard]] static std::string remaining_time_to_string(std::chrono::seconds time);

public:
    explicit Event(Schedule times) { events = std::move(times); }

    virtual ~Event() = default;

    [[nodiscard]] virtual std::string get_next() const;
};

class LandOfDeathEvent final : public Event {
protected:
    [[nodiscard]] std::string to_string(const std::tm& time) const override;

public:
    explicit LandOfDeathEvent(Schedule times) : Event(std::move(times)) {}
};

class LandOfLifeEvent final : public Event {
protected:
    [[nodiscard]] std::string to_string(const std::tm& time) const override;

public:
    explicit LandOfLifeEvent(Schedule times) : Event(std::move(times)) {}
};

class InstantCombatEvent final : public Event {
protected:
    [[nodiscard]] std::string to_string(const std::tm& time) const override;

public:
    explicit InstantCombatEvent(Schedule times) : Event(std::move(times)) {}
};