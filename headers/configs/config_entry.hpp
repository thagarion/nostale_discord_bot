#pragma once
#include <cstdint>
#include <yaml-cpp/node/convert.h>

struct ConfigEntry {
    uint64_t mara_channel_id = 0;
    uint64_t news_channel_id = 0;
    std::vector<uint64_t> roles;

    ConfigEntry() = default;
    ConfigEntry(const uint64_t mara, const uint64_t news) : mara_channel_id(mara), news_channel_id(news) {}
};

template <>
struct YAML::convert<ConfigEntry> {
    static bool decode(const Node& node, ConfigEntry& entry) {
        if (!node.IsMap()) {
            return false;
        }
        for (auto it = node.begin(); it != node.end(); ++it) {
            auto key = it->first.as<std::string>();
            if (key == "mara_channel_id") {
                entry.mara_channel_id = it->second.as<uint64_t>();
            } else if (key == "news_channel_id") {
                entry.news_channel_id = it->second.as<uint64_t>();
            } else if (key == "roles") {
                entry.roles = it->second.as<std::vector<uint64_t>>();
            }
        }
        return true;
    }

    static Node encode(const ConfigEntry& entry) {
        Node node;
        node["mara_channel_id"] = entry.mara_channel_id;
        node["news_channel_id"] = entry.news_channel_id;
        node["roles"] = entry.roles;
        return node;
    }
};