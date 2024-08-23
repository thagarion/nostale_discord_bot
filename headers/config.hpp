#pragma once

#include <string>
#include "yaml-cpp"

class Config {
    std::string token;
    uint64_t guild;
    uint64_t mara_channel;

public:
    Config() {
        YAML::Node config = YAML::LoadFile("config/config.yaml");
    }
};