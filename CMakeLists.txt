cmake_minimum_required(VERSION 3.22)
project(nostale_discord_bot VERSION 1.0 DESCRIPTION "A discord bot")

file(GLOB_RECURSE sources "sources/**.cpp")
file(GLOB_RECURSE headers "headers/**.hpp")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        dpp
        GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
        GIT_TAG v10.0.30
)
FetchContent_MakeAvailable(dpp)
FetchContent_Declare(
        date
        GIT_REPOSITORY https://github.com/HowardHinnant/date.git
        GIT_TAG master
)
set(BUILD_TZ_LIB ON CACHE INTERNAL "Build/Install of TZ library")
set(USE_SYSTEM_TZ_DB ON CACHE INTERNAL "Use the operating system's timezone database")
FetchContent_MakeAvailable(date)

include_directories(${project_name} PUBLIC headers ${DPP_INCLUDE_DIR})

add_executable(${project_name} PUBLIC main.cpp ${sources} ${headers})

target_link_libraries(${project_name} PUBLIC dpp date::date date-tz yaml-cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources} ${headers})