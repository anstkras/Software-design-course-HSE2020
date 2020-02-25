cmake_minimum_required(VERSION 3.10) # Проверка версии CMake
project(CLI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Werror -pedantic -pthread")

set(SOURCES 
    src/application.cpp
    src/environment.cpp
    src/executor.cpp
    src/parser/parser.cpp
    src/parser/tokenizer.cpp
    src/parser/variables_substitutor.cpp
    src/parser/commands_parser.cpp
    src/commands/command_factory.cpp

    src/commands/wc_command.cpp
    src/commands/echo_command.cpp
    src/commands/exit_command.cpp
    src/commands/pwd_command.cpp
    src/commands/cat_command.cpp
    src/commands/assignment_command.cpp
    src/commands/external_command.cpp
    src/commands/ls_command.cpp
    src/commands/cd_command.cpp
)

include_directories(inc/)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
include_directories(${Boost_INCLUDE_DIRS})
find_package(Boost COMPONENTS filesystem REQUIRED)

add_library(lcli STATIC ${SOURCES})
target_link_libraries(lcli stdc++fs)
target_link_libraries(lcli ${Boost_LIBRARIES})
add_executable(cli src/main.cpp)
target_link_libraries(cli lcli)

#GTest
enable_testing()
include(GoogleTest)
add_subdirectory(google_test)

set(TEST_SOURCES
    test/main.cpp
    test/environment_test.cpp
    test/executor_test.cpp
    test/command_factory_test.cpp
    test/parser_test.cpp
    test/commands_test.cpp
)

add_executable(cli_test ${TEST_SOURCES})
target_link_libraries(cli_test lcli gtest gtest_main)
add_test(NAME cli_test COMMAND cli_test)
