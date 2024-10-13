#pragma once

#include <memory>
#include <string>
#include <yaml-cpp/yaml.h>

#include "configs/config_entry.hpp"
#include "enums.hpp"
#include "events/event.hpp"

typedef std::shared_ptr<Event> EventPtr;

class Config {
    std::unordered_map<uint64_t, ConfigEntry> settings;
    std::vector<uint64_t> news_channels;
    std::unordered_map<EventType, EventPtr> events;

    const std::string config_path;
    const std::string storage_path;
    const std::string schedule_path;

    void save() const;

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

    void set_value(uint64_t guild_id, const std::string& key, const std::string& value);

    [[nodiscard]] uint64_t get_mara_channel_id(const uint64_t guild_id) const {
        if (settings.contains(guild_id)) {
            return settings.at(guild_id).mara_channel_id;
        }
        return 0;
    }

    [[nodiscard]] const std::vector<uint64_t>* get_news_channels() const {
        return &news_channels;
    }

    [[nodiscard]] std::string get_next_ic() const { return events.at(INSTANT_COMBAT)->get_next(); }

    [[nodiscard]] std::string get_next_aic() const { return events.at(ASGOBAS_INSTANT_COMBAT)->get_next(); }

    [[nodiscard]] std::string get_next_lod() const { return events.at(LAND_OF_DEATH)->get_next(); }

    [[nodiscard]] std::string get_next_lol() const { return events.at(LAND_OF_LIFE)->get_next(); }
};
