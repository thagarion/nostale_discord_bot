#include "bot.hpp"

#include "enums.hpp"
#include "rss_feed/rss_event.hpp"

void Bot::Init() {
    bot_ptr = std::make_unique<dpp::cluster>(BOT_TOKEN);

    bot_ptr->on_log(on_log);
    bot_ptr->on_ready(on_ready);
    bot_ptr->on_slashcommand(on_slashcommand);
    bot_ptr->on_autocomplete(on_autocomplete);
    bot_ptr->on_guild_member_update(on_guild_member_update);

    bot_ptr->start(dpp::st_wait);
}

void Bot::Log(const log_level level, const std::string& message) {
#ifdef LOG_CONSOLE
    std::cout << std::format("{}\t[{}]\t{}", dpp::utility::current_date_time(), dpp::utility::loglevel(level), message)
              << std::endl;
#else
    logger.log(std::format("{}\t[{}]\t{}", dpp::utility::current_date_time(), dpp::utility::loglevel(level), message));
#endif
}

void Bot::SendNews(const RSSEvent& event) {
    const auto channels = config.get_news_channels();
    for (const uint64_t channel : *channels) {
        for (auto& text : event.to_string()) {
            bot_ptr->message_create(dpp::message(channel, text));
            sleep(5);
        }
    }
}

void Bot::on_log(const dpp::log_t& log) {
#ifdef LOG_CONSOLE
    std::cout << std::format("{}\t[{}]\t{}", dpp::utility::current_date_time(), dpp::utility::loglevel(log.severity),
                             log.message)
              << std::endl;
#elif
    logger.log(std::format("{}\t[{}]\t{}", dpp::utility::current_date_time(), dpp::utility::loglevel(log.severity),
                           log.message));
#endif
}

void Bot::on_ready(const dpp::ready_t& event) {
    if (dpp::run_once<struct register_bot_commands>()) {
        bot_ptr->global_command_create(dpp::slashcommand(PING_COMMAND, "Проверка", bot_ptr->me.id));
        bot_ptr->global_command_create(dpp::slashcommand(CONF_COMMAND, "Настройки", bot_ptr->me.id)
                                           .add_option(dpp::command_option(dpp::co_string, "key", "Имя", true))
                                           .add_option(dpp::command_option(dpp::co_string, "value", "Значение", true)));
        bot_ptr->global_command_create(
            dpp::slashcommand(TIME_COMMAND, "Узнать время следующего ивента", bot_ptr->me.id)
                .add_option(dpp::command_option(dpp::co_string, "event", "Название ивента", true)
                                .add_choice(dpp::command_option_choice("Instant Combat", IC_EVENT))
                                .add_choice(dpp::command_option_choice("Asgobas' Instant Combat", AIC_EVENT))
                                .add_choice(dpp::command_option_choice("Land Of Death", LOD_EVENT))
                                .add_choice(dpp::command_option_choice("Land Of Life", LOL_EVENT))));

        // TODO mara channel
        // bot_ptr->global_command_create(dpp::slashcommand("mara", "Начать марафон", bot_ptr->me.id)
        //                                    .add_option(dpp::command_option(dpp::co_string, "name", "Название")));
    }

    std::vector<std::string> roles = {LVL85_ROLE,  CLVL1_ROLE,  CLVL10_ROLE, CLVL20_ROLE, CLVL30_ROLE, CLVL40_ROLE,
                                      CLVL50_ROLE, CLVL60_ROLE, CLVL70_ROLE, CLVL80_ROLE, CLVL90_ROLE};

    for (const auto& guild_id : event.guilds) {
        std::vector<uint64_t> role_ids;
        for (const auto& role_name : roles) {
            bool found = false;
            auto guild_roles = dpp::get_role_cache();
            // for (const auto& role_id : guild_roles) {
            //     const auto role_found = dpp::find_role(role_id);
            //     if (role_found->name == role_name) {
            //         found = true;
            //         role_ids.push_back(static_cast<uint64_t>(role_found->id));
            //         break;
            //     }
            // }
            if (!found) {
                Log(dpp::ll_error, "Role " + role_name + " not found");
            }
        }
        config.set_value(guild_id, ROLES_CONF, role_ids);
    }
}

void Bot::on_slashcommand(const dpp::slashcommand_t& event) {
    if (event.command.get_command_name() == PING_COMMAND) {
        event.reply("Я тут");
    }
    if (event.command.get_command_name() == CONF_COMMAND) {
        const auto key = std::get<std::string>(event.get_parameter("key"));
        const auto value = std::get<std::string>(event.get_parameter("value"));
        if (event.command.get_issuing_user() == event.command.get_guild().owner_id) {
            config.set_value(event.command.guild_id, key, value);
            event.reply(dpp::message(std::format("OK")).set_flags(dpp::m_ephemeral));
        } else {
            event.reply(dpp::message(std::format("Эту команду может использовать только владелец сервера"))
                            .set_flags(dpp::m_ephemeral));
        }
    }
    if (event.command.get_command_name() == TIME_COMMAND) {
        const auto event_name = std::get<std::string>(event.get_parameter("event"));
        if (event_name == IC_EVENT) {
            event.reply(config.get_next_ic());
        } else if (event_name == AIC_EVENT) {
            event.reply(config.get_next_aic());
        } else if (event_name == LOD_EVENT) {
            event.reply(config.get_next_lod());
        } else if (event_name == LOL_EVENT) {
            event.reply(config.get_next_lol());
        } else {
            event.reply(
                dpp::message(std::format("Недопустимый параметр [{}]", event_name)).set_flags(dpp::m_ephemeral));
        }
    }
    // TODO mara channel
    // if (event.command.get_command_name() == "mara") {
    //     dpp::message msg(event.command.channel_id, "this text has a button");
    //     msg.add_component(dpp::component().add_component(dpp::component()
    //                                                          .set_label("Click me!")
    //                                                          .set_type(dpp::cot_button)
    //                                                          .set_style(dpp::cos_primary)
    //                                                          .set_id("myid")));
    //
    //     event.reply(msg);
    // }
}

void Bot::on_autocomplete(const dpp::autocomplete_t& event) {
    for (auto& opt : event.options) {
        if (opt.focused) {
            if (event.name == TIME_COMMAND) {
                const auto user_value = std::get<std::string>(opt.value);
                bot_ptr->interaction_response_create(
                    event.command.id, event.command.token,
                    dpp::interaction_response(dpp::ir_autocomplete_reply)
                        .add_autocomplete_choice(dpp::command_option_choice("instant combat", std::string(IC_EVENT)))
                        .add_autocomplete_choice(
                            dpp::command_option_choice("asgobas instant combat", std::string(AIC_EVENT)))
                        .add_autocomplete_choice(dpp::command_option_choice("land of death", std::string(LOD_EVENT)))
                        .add_autocomplete_choice(dpp::command_option_choice("land of life", std::string(LOL_EVENT))));
                break;
            }
        }
    }
}

void Bot::on_guild_member_update(const dpp::guild_member_update_t& update) {
    const auto name = update.updated.get_nickname();

    const std::regex level_string_pattern(R"(\[[Cc0-9\/\]]+)");
    std::string level_string;
    std::smatch match;
    if (std::regex_search(name, match, level_string_pattern)) {
        level_string = match.str(0);
    } else {
        Log(dpp::ll_error, "Can't parse name " + name);
        return;
    }

    level_string = level_string.substr(1, level_string.size() - 2);
    Log(dpp::ll_trace, "level string = " + level_string);

    const std::regex level_pattern(R"([Cc]?[0-9]+)");

    std::smatch match_levels;
    while (std::regex_search(level_string, match_levels, level_pattern)) {
        std::string level = match_levels.str(0);
        if (level.front() == 'C' || level.front() == 'c') {
            Log(dpp::ll_trace, "clevel string = " + level);
        } else {
            Log(dpp::ll_trace, " level string = " + level);
        }
        level_string = match_levels.suffix().str();
    }

    // const auto start = name.find_first_of('[') + 1;
    // const auto end = name.find_last_of(']');
    // if (start == std::string::npos || end == std::string::npos || end <= start) {
    //     Log(dpp::ll_error, std::format("Can not parse levels in name [{}]", name));
    //     return;
    // }
    // auto levels_string = name.substr(start, end - start);
    // for (auto i = levels_string.find_first_of('/'); i != std::string::npos; i = levels_string.find_first_of('/')) {
    //     auto level_string = levels_string.substr(0, i);
    //     if (levels_string.front() == 'C' || levels_string.front() == 'c') {
    //         level_string = level_string.substr(1, levels_string.size());
    //         Log(dpp::ll_debug, "got clevel = " + level_string);
    //     } else {
    //         Log(dpp::ll_debug, "got level = " + level_string);
    //     }
    //     levels_string = levels_string.substr(i + 1, levels_string.size());
    // }
    // Log(dpp::ll_debug, "got number = " + levels_string);
}
