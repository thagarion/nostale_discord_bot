#pragma once

#include <string>
#include <memory>
#include "event.hpp"
#include <yaml-cpp/yaml.h>

typedef std::shared_ptr<Event> EventPtr;

class Config {
    std::string token;
    uint64_t guild;
    uint64_t mara_channel;

    std::map<std::string, EventPtr> events;

    template<typename T>
    [[nodiscard]] static T get_value(YAML::Node config, const std::string &field) {
        if (!config[field]) {
            printf("error: %s not found", field.c_str());
        }
        return config[field].as<T>();
    }

public:
    explicit Config(const std::string &path) {
        const YAML::Node config = YAML::LoadFile(path + "/config.yaml");
        token = get_value<std::string>(config, "token");
        guild = get_value<uint64_t>(config, "guild_id");
        mara_channel = get_value<uint64_t>(config, "mara_channel_id");

        for (const YAML::Node events_node = YAML::LoadFile(path + "/events.yaml"); const auto &event: events_node) {
            if (event.first.as<std::string>() == "lod") {
                Schedule lods;
                for (const auto &entry: event.second) {
                    lods[entry["time"].as<int>()] = entry["channels"].as<std::vector<int>>();
                }
                events["lod"] = std::make_shared<LandOfDeathEvent>(lods);
            }
        }
    }

    [[nodiscard]] std::string get_bot_token() const { return token; }

    [[nodiscard]] uint64_t get_guild_id() const { return guild; }

    [[nodiscard]] uint64_t get_mara_channel_id() const { return mara_channel; }

    [[nodiscard]] std::string get_next_lod() const { return events.at("lod")->get_next(); }
};
