cmake_minimum_required(VERSION 3.16)
project(flipper_emergency_lock_core C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(lock_state src/lock_state.c)
target_include_directories(lock_state PUBLIC include)

add_executable(lock_state_tests tests/test_lock_state.c)
target_link_libraries(lock_state_tests PRIVATE lock_state)
enable_testing()
add_test(NAME lock_state_tests COMMAND lock_state_tests)
