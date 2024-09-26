#include <dpp/dpp.h>

#include "config.hpp"
#include "enums.hpp"

int main(int argc, char* argv[]) {
    const Config config(argv[1]);

    dpp::cluster bot(config.get_bot_token(), dpp::i_default_intents | dpp::i_message_content | dpp::i_all_intents);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand(PING_COMMAND, "Проверка", bot.me.id));
            bot.global_command_create(
                dpp::slashcommand(TIME_COMMAND, "Узнать время следующего ивента", bot.me.id)
                    .add_option(
                        dpp::command_option(dpp::co_string, "event", "Название ивента").set_auto_complete(true)));
            bot.global_command_create(dpp::slashcommand("mara", "Начать марафон", bot.me.id)
                                          .add_option(dpp::command_option(dpp::co_string, "name", "Название")));
        }
    });

    bot.on_slashcommand([config](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Я тут");
        }
        if (event.command.get_command_name() == "time") {
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
        if (event.command.get_command_name() == "mara") {
            if (event.command.get_channel().id != dpp::snowflake(config.get_mara_channel_id())) {
                event.reply("Эту команду можно использовать только в " +
                            find_channel(dpp::snowflake(config.get_mara_channel_id()))->get_mention());
                return;
            }
            dpp::message msg(event.command.channel_id, "this text has a button");
            msg.add_component(dpp::component().add_component(dpp::component()
                                                                 .set_label("Click me!")
                                                                 .set_type(dpp::cot_button)
                                                                 .set_style(dpp::cos_primary)
                                                                 .set_id("myid")));

            event.reply(msg);
        }
    });

    bot.on_autocomplete([&bot](const dpp::autocomplete_t& event) {
        for (auto& opt : event.options) {
            if (opt.focused) {
                if (event.name == TIME_COMMAND) {
                    const auto user_value = std::get<std::string>(opt.value);
                    bot.interaction_response_create(event.command.id, event.command.token,
                                                    dpp::interaction_response(dpp::ir_autocomplete_reply)
                                                        .add_autocomplete_choice(dpp::command_option_choice(
                                                            "instant combat", std::string(IC_EVENT)))
                                                        .add_autocomplete_choice(dpp::command_option_choice(
                                                            "asgobas instant combat", std::string(AIC_EVENT)))
                                                        .add_autocomplete_choice(dpp::command_option_choice(
                                                            "land of death", std::string(LOD_EVENT)))
                                                        .add_autocomplete_choice(dpp::command_option_choice(
                                                            "land of life", std::string(LOL_EVENT))));
                    break;
                }
            }
        }
    });

    bot.on_button_click([](const dpp::button_click_t& event) {
        event.reply(dpp::ir_update_message, "You clicked: " + event.custom_id);
    });

    bot.on_guild_member_update([&bot](const dpp::guild_member_update_t& update) {
    });

    bot.start(dpp::st_wait);
}
