#include "config.hpp"

Config::Config(const std::string& path) {
    const YAML::Node config = YAML::LoadFile(path + "/config.yaml");
    token = get_value<std::string>(config, "token");
    guild = get_value<uint64_t>(config, "guild_id");
    mara_channel = get_value<uint64_t>(config, "mara_channel_id");

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