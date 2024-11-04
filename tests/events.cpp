#include <ctime>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bot.hpp"

#define std_time_mocked time
extern "C" {
std::time_t std_time_mocked(std::time_t* t);
}

#include "configs/config.hpp"

std::time_t mock_time_value = 0;
extern "C" std::time_t std_time_mocked(std::time_t* t) {
    if (t) {
        *t = mock_time_value;
    }
    return mock_time_value;
}

TEST(GetTime, LandOfLifeEvent) {
    {
        mock_time_value = 1717388150;  // Monday, 3 June 2024 г., 4:15:50
        const auto config = Config("configs");
        const auto result = config.get_next_lol();
        EXPECT_EQ(result,
                  "Лол начался в 07:00 (0 часов 15 минут 50 секунд назад)\n"
                  "Асго в 08:00 (Осталось 0 часов 44 минуты 10 секунд)\n"
                  "Каналы: 1, 2, 3, 4, 5, 6, 7");
    }
    {
        mock_time_value = 1730693750;  // Monday, 4 November 2024 г., 4:15:50
        const auto config = Config("configs");
        const auto result = config.get_next_lol();
        EXPECT_EQ(result,
                  "Следующий ЛоЛ будет в 08:00 (Осталось 0 часов 44 минуты 10 секунд)\n"
                  "Асго в 09:00 (Осталось 1 час 44 минуты 10 секунд)\n"
                  "Каналы: 1, 2, 3, 4, 5, 6, 7");
    }
    {
        mock_time_value = 1733004950;  // Saturday, 30 November 2024 г., 22:15:50
        const auto config = Config("configs");
        const auto result = config.get_next_lol();
        EXPECT_EQ(result,
                  "Следующий ЛоЛ будет в 02:00 (Осталось 0 часов 44 минуты 10 секунд)\n"
                  "Асго в 03:00 (Осталось 1 час 44 минуты 10 секунд)\n"
                  "Каналы: 1, 2, 3, 4, 5, 6, 7");
    }
}

TEST(GetTime, LandOfDeathEvent) {
    {
        mock_time_value = 1717388150;  // Monday, 3 June 2024 г., 4:15:50
        const auto config = Config("configs");
        const auto result = config.get_next_lod();
        EXPECT_EQ(result,
                  "ЛоД начался в 07:00 (0 часов 15 минут 50 секунд назад)\n"
                  "ДХ в 08:00 (Осталось 0 часов 44 минуты 10 секунд)\n"
                  "Каналы: 1");
    }
    {
        mock_time_value = 1730693750;  // Monday, 4 November 2024 г., 4:15:50
        const auto config = Config("configs");
        const auto result = config.get_next_lod();
        EXPECT_EQ(result,
                  "Следующий ЛоД будет в 08:00 (Осталось 0 часов 44 минуты 10 секунд)\n"
                  "ДХ в 09:00 (Осталось 1 час 44 минуты 10 секунд)\n"
                  "Каналы: 1");
    }
    {
        mock_time_value = 1733004950;  // Saturday, 30 November 2024 г., 22:15:50
        const auto config = Config("configs");
        const auto result = config.get_next_lod();
        EXPECT_EQ(result,
                  "Следующий ЛоД будет в 02:00 (Осталось 0 часов 44 минуты 10 секунд)\n"
                  "ДХ в 03:00 (Осталось 1 час 44 минуты 10 секунд)\n"
                  "Каналы: 7");
    }
}

TEST(GetTime, InstantCombatEvent) {
    {
        mock_time_value = 1717388150;  // Monday, 3 June 2024 г., 4:15:50
        const auto config = Config("configs");
        const auto result = config.get_next_ic();
        EXPECT_EQ(result,
                  "Следующая ББ будет в 09:00 (Осталось 1 час 44 минуты 10 секунд)\n"
                  "Каналы: 1, 2, 3");
    }
    {
        mock_time_value = 1730693750;  // Monday, 4 November 2024 г., 4:15:50
        const auto config = Config("configs");
        const auto result = config.get_next_ic();
        EXPECT_EQ(result,
                  "Следующая ББ будет в 08:00 (Осталось 0 часов 44 минуты 10 секунд)\n"
                  "Каналы: 1, 2, 3");
    }
    {
        mock_time_value = 1733004950;  // Saturday, 30 November 2024 г., 22:15:50
        const auto config = Config("configs");
        const auto result = config.get_next_ic();
        EXPECT_EQ(result,
                  "Следующая ББ будет в 02:00 (Осталось 0 часов 44 минуты 10 секунд)\n"
                  "Каналы: 1, 2, 3");
    }
}