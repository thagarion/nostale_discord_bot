#pragma once

#include <chrono>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>

typedef std::map<std::tm, std::vector<int> > Schedule;

enum EventType { INSTANT_COMBAT, ASGOBAS_INSTANT_COMBAT, LAND_OF_DEATH, LAND_OF_LIFE };

const std::unordered_map<EventType, std::string> EventTypeString = {
    {INSTANT_COMBAT, "ic"}, {ASGOBAS_INSTANT_COMBAT, "asgo"}, {LAND_OF_DEATH, "lod"}, {LAND_OF_LIFE, "lol"}};

class Event {
    [[nodiscard]] static std::string get_plural_form(long value, const std::string& one, const std::string& few,
                                                     const std::string& many);

protected:
    Schedule events;

    [[nodiscard]] virtual std::string to_string(const std::tm& time) const = 0;

    [[nodiscard]] static std::string channels_to_string(const std::vector<int>& channels);

    [[nodiscard]] static std::string time_to_string(const tm& time);

    [[nodiscard]] static std::string remaining_time_to_string(std::chrono::seconds time);
    [[nodiscard]] static std::string past_time_to_string(std::chrono::seconds time);

public:
    explicit Event(Schedule times) { events = std::move(times); }

    virtual ~Event() = default;

    [[nodiscard]] virtual std::string get_next() const;
};

class LandOfDeathEvent final : public Event {
protected:
    [[nodiscard]] std::string to_string(const std::tm& time) const override;
    [[nodiscard]] std::string to_string_current(const std::tm& time) const;

public:
    explicit LandOfDeathEvent(const Schedule& times) : Event(times) {}

    [[nodiscard]] std::string get_next() const override;
};

class LandOfLifeEvent final : public Event {
protected:
    [[nodiscard]] std::string to_string(const std::tm& time) const override;
    [[nodiscard]] std::string to_string_current(const std::tm& time) const;

public:
    explicit LandOfLifeEvent(const Schedule& times) : Event(times) {}

    [[nodiscard]] std::string get_next() const override;
};

class InstantCombatEvent final : public Event {
protected:
    [[nodiscard]] std::string to_string(const std::tm& time) const override;

public:
    explicit InstantCombatEvent(const Schedule& times) : Event(times) {}
};

class AsgobasInstantCombatEvent final : public Event {
protected:
    [[nodiscard]] std::string to_string(const std::tm& time) const override;

public:
    explicit AsgobasInstantCombatEvent(const Schedule& times) : Event(times) {}
};