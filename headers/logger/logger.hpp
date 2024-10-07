#pragma once

#include <bits/fs_fwd.h>
#include <bits/fs_path.h>
#include <fstream>
#include <iostream>
#include <string>

class Logger {
    const std::string dir_name = "logs";
    const long max_log_file_size = 100 * 1024 * 1024;

    std::ofstream log_file;
    std::string log_file_mame;

    void open_file() {
        std::filesystem::create_directory(dir_name);
        log_file.open(log_file_mame, std::ios::app);
        if (!log_file) {
            std::cerr << "Failed to open log file: " << log_file_mame << std::endl;
            exit(1);
        }
    }

    static std::string get_current_time() {
        const std::time_t now = std::time(nullptr);
        const std::tm* now_tm = std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(now_tm, "%Y-%m-%d-%H-%M-%S");
        return oss.str();
    }

    void rotate_file() {
        log_file.close();
        log_file_mame = dir_name + "/" + get_current_time() + ".log";
        open_file();
    }

public:
    Logger() : log_file_mame(dir_name + "/" + get_current_time() + ".log") { open_file(); }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(const std::string& message) {
        if (log_file.is_open()) {
            const std::uintmax_t size = file_size(std::filesystem::path(log_file_mame));
            if (size > max_log_file_size) {
                rotate_file();
            }

            log_file << message << std::endl;
        }
    }
};