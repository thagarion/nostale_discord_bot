cmake_minimum_required(VERSION 3.22)
project(nostale_discord_bot VERSION 1.0 DESCRIPTION "A discord bot")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (BOT_TOKEN)
    message(STATUS "BOT_TOKEN found: " ${BOT_TOKEN})
    add_definitions(-DBOT_TOKEN=${BOT_TOKEN})
else ()
    message(FATAL_ERROR "BOT_TOKEN not presented")
endif ()

if (GEMINI_TOKEN)
    message(STATUS "GEMINI_TOKEN found: " ${GEMINI_TOKEN})
else ()
    message(STATUS "GEMINI_TOKEN not presented")
endif ()
add_definitions(-DGEMINI_TOKEN=${GEMINI_TOKEN})

include(FetchContent)
FetchContent_Declare(
        dpp
        GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
        GIT_TAG v10.0.30
)
FetchContent_MakeAvailable(dpp)
FetchContent_Declare(
        html2md
        GIT_REPOSITORY https://github.com/tim-gromeyer/html2md.git
        GIT_TAG v1.6.0
)
FetchContent_MakeAvailable(html2md)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

file(GLOB_RECURSE sources "sources/**.cpp")
file(GLOB_RECURSE headers "headers/**.hpp")

include_directories(${project_name} PUBLIC headers ${DPP_INCLUDE_DIR} ${HTML2MD_INCLUDE_DIR} ${JSON_INCLUDE_DIR})

add_executable(${project_name} PUBLIC main.cpp ${sources} ${headers})

target_link_libraries(${project_name} PUBLIC pthread dpp html2md yaml-cpp curl tinyxml2 nlohmann_json)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources} ${headers})