cmake_minimum_required(VERSION 3.22)
project(telegram_bot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(TgBot REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
  TgBot::TgBot
  SQLite::SQLite3
  nlohmann_json::nlohmann_json
)
