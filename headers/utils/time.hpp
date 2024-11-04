#pragma once

inline bool operator>(const std::tm& lhs, const std::tm& rhs) {
    if (lhs.tm_year != rhs.tm_year) return lhs.tm_year > rhs.tm_year;
    if (lhs.tm_mon != rhs.tm_mon) return lhs.tm_mon > rhs.tm_mon;
    if (lhs.tm_mday != rhs.tm_mday) return lhs.tm_mday > rhs.tm_mday;
    if (lhs.tm_hour != rhs.tm_hour) return lhs.tm_hour > rhs.tm_hour;
    if (lhs.tm_min != rhs.tm_min) return lhs.tm_min > rhs.tm_min;
    return lhs.tm_sec > rhs.tm_sec;
}

inline bool operator<(const std::tm& lhs, const std::tm& rhs) {
    if (lhs.tm_year != rhs.tm_year) return lhs.tm_year < rhs.tm_year;
    if (lhs.tm_mon != rhs.tm_mon) return lhs.tm_mon < rhs.tm_mon;
    if (lhs.tm_mday != rhs.tm_mday) return lhs.tm_mday < rhs.tm_mday;
    if (lhs.tm_hour != rhs.tm_hour) return lhs.tm_hour < rhs.tm_hour;
    if (lhs.tm_min != rhs.tm_min) return lhs.tm_min < rhs.tm_min;
    return lhs.tm_sec < rhs.tm_sec;
}

inline bool is_greater_time(const std::tm& lhs, const std::tm& rhs) {
    if (lhs.tm_hour != rhs.tm_hour) return lhs.tm_hour > rhs.tm_hour;
    if (lhs.tm_min != rhs.tm_min) return lhs.tm_min > rhs.tm_min;
    return lhs.tm_sec > rhs.tm_sec;
}

inline std::tm* get_current_date_time_gmt_2() {
    const std::time_t now = std::time(nullptr);
    std::tm* current_time = std::gmtime(&now);

    current_time->tm_hour += 1;
    if (current_time->tm_mon >= 2 && current_time->tm_mon <= 9) {
        current_time->tm_hour += 1;
    }

    mktime(current_time);
    return current_time;
}

inline std::tm get_current_time_gmt_2() {
    const std::time_t now = std::time(nullptr);
    std::tm* current_time = std::gmtime(&now);

    current_time->tm_hour += 1;
    if (current_time->tm_mon >= 2 && current_time->tm_mon <= 9) {
        current_time->tm_hour += 1;
    }

    if (current_time->tm_hour > 23) {
        current_time->tm_hour -= 24;
    }

    current_time->tm_year = 0;
    current_time->tm_mon = 0;
    current_time->tm_mday = 0;

    return *current_time;
}

inline std::tm get_current_date_time_mks() {
    const std::time_t now = std::time(nullptr);
    std::tm* current_time = std::gmtime(&now);

    current_time->tm_hour += 3;

    mktime(current_time);
    return *current_time;
}

inline std::tm get_date_time_mks(const tm* time) {
    std::tm event_time = *time;

    const std::time_t now = std::time(nullptr);
    const std::tm* utc_time = std::gmtime(&now);

    event_time.tm_year = utc_time->tm_year;
    event_time.tm_mon = utc_time->tm_mon;
    event_time.tm_mday = utc_time->tm_mday;

    if (is_greater_time(*utc_time, event_time)) {
        event_time.tm_mday += 1;
    }

    event_time.tm_hour += 1;
    if (event_time.tm_mon < 2 || event_time.tm_mon > 9) {
        event_time.tm_hour += 1;
    }

    mktime(&event_time);
    return event_time;
}

inline std::tm get_current_time_mks() {
    const std::time_t now = std::time(nullptr);
    std::tm* current_time = std::gmtime(&now);

    current_time->tm_hour += 3;

    if (current_time->tm_hour > 23) {
        current_time->tm_hour -= 24;
    }

    current_time->tm_year = 0;
    current_time->tm_mon = 0;
    current_time->tm_mday = 0;

    return *current_time;
}