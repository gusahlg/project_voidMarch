cmake_minimum_required(VERSION 3.10.0)
project(void_march VERSION 0.1.0 LANGUAGES C CXX)

# Gather all source files

set(SOURCES
    game_index/main.cpp

    game_index/screen_logic/home_screen.cpp
    game_index/screen_logic/level_select.cpp
    game_index/screen_logic/select.cpp

    game_index/game_logic/enemy_logic.cpp
    game_index/game_logic/game_inputs.cpp
    game_index/game_logic/game.cpp
    game_index/game_logic/on_screen.cpp
    game_index/game_logic/player_event_handler.cpp

    game_index/game_logic/inventory/melee_bindings.cpp

    game_index/game_logic/items/items.cpp
)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/game_index/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party/raylib/include
)

# Add executable
add_executable(void_march ${SOURCES})

# Link raylib (static library)
target_link_libraries(void_march
    PRIVATE
    ${PROJECT_SOURCE_DIR}/third_party/raylib/lib/libraylib.a
    m  # math
    pthread
    dl
    rt
    X11
)

# Link Rust static library
target_link_libraries(void_march PRIVATE ${PROJECT_SOURCE_DIR}/lib/libmath_core.a)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
