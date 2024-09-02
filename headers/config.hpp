#pragma once

#include <memory>
#include <string>
#include <yaml-cpp/yaml.h>

#include "event.hpp"

typedef std::shared_ptr<Event> EventPtr;

class Config {
    std::string token;
    uint64_t guild;
    uint64_t mara_channel;

    std::map<EventType, EventPtr> events;

    template <typename T>
    [[nodiscard]] static T get_value(YAML::Node config, const std::string& field) {
        if (!config[field]) {
            const auto error_string = std::format("field [{}] not found", field);
            throw std::runtime_error(error_string);
        }
        return config[field].as<T>();
    }

public:
    explicit Config(const std::string& path);

    [[nodiscard]] std::string get_bot_token() const { return token; }

    [[nodiscard]] uint64_t get_guild_id() const { return guild; }

    [[nodiscard]] uint64_t get_mara_channel_id() const { return mara_channel; }

    [[nodiscard]] std::string get_next_ic() const { return events.at(INSTANT_COMBAT)->get_next(); }

    [[nodiscard]] std::string get_next_aic() const { return events.at(ASGOBAS_INSTANT_COMBAT)->get_next(); }

    [[nodiscard]] std::string get_next_lod() const { return events.at(LAND_OF_DEATH)->get_next(); }

    [[nodiscard]] std::string get_next_lol() const { return events.at(LAND_OF_LIFE)->get_next(); }
};
