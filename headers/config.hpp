#pragma once

#include <string>
#include "event.hpp"
#include <yaml-cpp/yaml.h>

class Config {
    std::string token;
    uint64_t guild;
    uint64_t mara_channel;

    std::map<std::string, Event> events;

    template<typename T>
    [[nodiscard]] T get_value(YAML::Node config, const std::string &field) const {
        if (!config[field]) {
            printf("error: %s not found", field.c_str());
        }
        return config[field].as<T>();
    }

public:
    explicit Config(const std::string &path) {
        YAML::Node config = YAML::LoadFile(path + "/config.yaml");
        token = get_value<std::string>(config, "token");
        guild = get_value<uint64_t>(config, "guild_id");
        mara_channel = get_value<uint64_t>(config, "mara_channel_id");
    }

    [[nodiscard]] std::string get_bot_token() const { return token; }

    [[nodiscard]] uint64_t get_guild_id() const { return guild; }

    [[nodiscard]] uint64_t get_mara_channel_id() const { return mara_channel; }
};