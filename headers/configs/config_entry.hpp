#pragma once
#include <cstdint>
#include <yaml-cpp/node/convert.h>

struct ConfigEntry {
    uint64_t mara_channel_id;
    uint64_t news_channel_id;

    ConfigEntry() = default;
    ConfigEntry(const uint64_t mara, const uint64_t news) : mara_channel_id(mara), news_channel_id(news) {}
};

template <>
struct YAML::convert<ConfigEntry> {
    static bool decode(const Node& node, ConfigEntry& entry) {
        if (!node.IsMap()) {
            return false;
        }
        entry.mara_channel_id = node["mara_channel_id"].as<uint64_t>();
        entry.news_channel_id = node["news_channel_id"].as<uint64_t>();
        return true;
    }

    static Node encode(const ConfigEntry& entry) {
        Node node;
        node["mara_channel_id"] = entry.mara_channel_id;
        node["news_channel_id"] = entry.news_channel_id;
        return node;
    }
};