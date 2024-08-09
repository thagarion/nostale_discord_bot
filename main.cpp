#include <iostream>
#include <dpp/dpp.h>

int main() {

    std::string TOKEN_ID;
    uint64_t GUILD_ID;

    std::cin >> TOKEN_ID >> GUILD_ID;

    dpp::cluster bot(TOKEN_ID);

    bot.on_log(dpp::utility::cout_logger());

    bot.log(dpp::loglevel::ll_debug, TOKEN_ID);

    bot.on_ready([&bot](const dpp::ready_t &event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("change_name", "Изменить имя на сервере", bot.me.id).add_option(
                    dpp::command_option(dpp::co_string, "name", "Новое имя")));
        }
    });

    bot.on_slashcommand([](const dpp::slashcommand_t &event) {
        dpp::permission perms = event.command.get_resolved_permission(event.command.usr.id);
        if (!perms.can(dpp::p_change_nickname)) {
            event.reply("You don't have the required permissions to change nickname!");
            return;
        }
        if (event.command.get_command_name() == "change_name") {
            std::string new_name = std::get<std::string>(event.get_parameter("name"));
            dpp::snowflake user_id = event.command.get_issuing_user().id;
            std::string nickname = event.command.get_guild().members.at(user_id).get_nickname();
            dpp::guild guild = event.command.get_guild();
            guild.members.find(user_id)->second.set_nickname(new_name);
            event.reply(nickname + " теперь известен как " + guild.members.find(user_id)->second.get_mention());
        }
    });

    bot.log(dpp::loglevel::ll_debug, "test");

    bot.start(dpp::st_wait);
}