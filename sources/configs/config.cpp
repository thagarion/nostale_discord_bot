#include "configs/config.hpp"

#include <fstream>

#include "bot.hpp"
#include "utils/time.hpp"

Config::Config(const std::string& path) : config_path(path) {
    for (const YAML::Node config_node = YAML::LoadFile(path + "/config.yaml"); const auto& config_entry : config_node) {
        auto guild_id = config_entry.first.as<uint64_t>();
        auto entry = config_entry.second.as<ConfigEntry>();
        settings[guild_id] = entry;
        news_channels.push_back(entry.news_channel_id);
    }

    for (const YAML::Node events_node = YAML::LoadFile(path + "/events.yaml"); const auto& event : events_node) {
        if (event.first.as<std::string>() == "ic") {
            Schedule ics;
            for (const auto& entry : event.second) {
                std::istringstream ss(entry["time"].as<std::string>());
                std::tm time = {};
                ss >> std::get_time(&time, "%H:%M");
                ics[time] = entry["channels"].as<std::vector<int>>();
            }
            events[INSTANT_COMBAT] = std::make_shared<InstantCombatEvent>(ics);
        }
        if (event.first.as<std::string>() == "asgo") {
            Schedule asgos;
            for (const auto& entry : event.second) {
                std::istringstream ss(entry["time"].as<std::string>());
                std::tm time = {};
                ss >> std::get_time(&time, "%H:%M");
                asgos[time] = entry["channels"].as<std::vector<int>>();
            }
            events[ASGOBAS_INSTANT_COMBAT] = std::make_shared<AsgobasInstantCombatEvent>(asgos);
        }
        if (event.first.as<std::string>() == "lod") {
            Schedule lods;
            for (const auto& entry : event.second) {
                std::istringstream ss(entry["time"].as<std::string>());
                std::tm time = {};
                ss >> std::get_time(&time, "%H:%M");
                lods[time] = entry["channels"].as<std::vector<int>>();
            }
            events[LAND_OF_DEATH] = std::make_shared<LandOfDeathEvent>(lods);
        }
        if (event.first.as<std::string>() == "lol") {
            Schedule lols;
            for (const auto& entry : event.second) {
                std::istringstream ss(entry["time"].as<std::string>());
                std::tm time = {};
                ss >> std::get_time(&time, "%H:%M");
                lols[time] = entry["channels"].as<std::vector<int>>();
            }
            events[LAND_OF_LIFE] = std::make_shared<LandOfLifeEvent>(lols);
        }
    }

    for (auto& [type, event] : EventTypeString) {
        if (!events.contains(type)) {
            const auto error_string = std::format("{} event not found in config", event);
            throw std::runtime_error(error_string);
        }
    }
}

void Config::set_value(const uint64_t guild_id, const std::string& key, const std::string& value) {
    if (!settings.contains(guild_id)) {
        settings[guild_id] = ConfigEntry();
    }
    if (key == MARA_CONF) {
        settings[guild_id].mara_channel_id = strtoull(value.c_str(), nullptr, 10);
    } else if (key == NEWS_CONF) {
        settings[guild_id].news_channel_id = strtoull(value.c_str(), nullptr, 10);
        news_channels.clear();
        for (auto [key, value] : settings) {
            news_channels.push_back(value.news_channel_id);
        }
    }

    save();
}

void Config::save() const {
    const auto file_name = config_path + "/config.yaml";

    std::ofstream config_file(file_name, std::ios::out | std::ios::trunc);
    if (!config_file) {
        Bot::Log(log_level::ll_error, "Cannot open file for writing " + file_name);
        return;
    }

    YAML::Node node;
    for (auto& [key, value] : settings) {
        node[key] = value;
    }

    config_file << node;
    config_file.close();
}