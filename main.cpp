#include <dpp/dpp.h>

#include "config.hpp"

int main(int argc, char* argv[]) {
    const Config config(argv[1]);

    dpp::cluster bot(config.get_bot_token());

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Проверка", bot.me.id));
            bot.global_command_create(dpp::slashcommand("ic", "Узнать время ББ", bot.me.id));
            bot.global_command_create(dpp::slashcommand("asgo", "Узнать время Асго ББ", bot.me.id));
            bot.global_command_create(dpp::slashcommand("lod", "Узнать время ЛоДа", bot.me.id));
            bot.global_command_create(dpp::slashcommand("lol", "Узнать время ЛоЛа", bot.me.id));
            bot.global_command_create(dpp::slashcommand("change_name", "Изменить имя на сервере", bot.me.id)
                                          .add_option(dpp::command_option(dpp::co_string, "name", "Новое имя")));
            bot.global_command_create(dpp::slashcommand("mara", "Начать марафон", bot.me.id)
                                          .add_option(dpp::command_option(dpp::co_string, "name", "Название")));
        }
    });

    bot.on_slashcommand([config](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Я тут");
        }
        if (event.command.get_command_name() == "ic") {
            event.reply(config.get_next_ic());
        }
        if (event.command.get_command_name() == "asgo") {
            event.reply(config.get_next_aic());
        }
        if (event.command.get_command_name() == "lod") {
            event.reply(config.get_next_lod());
        }
        if (event.command.get_command_name() == "lol") {
            event.reply(config.get_next_lol());
        }
        if (event.command.get_command_name() == "change_name") {
            if (const dpp::permission permission = event.command.get_resolved_permission(event.command.usr.id);
                !permission.can(dpp::p_change_nickname)) {
                event.reply("You don't have the required permissions to change nickname!");
                return;
            }
            const std::string new_name = std::get<std::string>(event.get_parameter("name"));
            const dpp::snowflake user_id = event.command.get_issuing_user().id;
            const std::string nickname = event.command.get_guild().members.at(user_id).get_nickname();
            dpp::guild guild = event.command.get_guild();
            guild.members.find(user_id)->second.set_nickname(new_name);
            event.reply(nickname + " теперь известен как " + guild.members.find(user_id)->second.get_mention());
        }
        if (event.command.get_command_name() == "mara") {
            if (event.command.get_channel().id == dpp::snowflake(config.get_mara_channel_id())) {
                event.reply("Это правильный канал");
            } else {
                event.reply("Эту команду можно использовать только в " +
                            dpp::find_channel(dpp::snowflake(config.get_mara_channel_id()))->get_mention());
            }
        }
    });

    bot.start(dpp::st_wait);
}
