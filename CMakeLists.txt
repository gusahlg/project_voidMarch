cmake_minimum_required(VERSION 3.20)
project(void_march VERSION 0.1.0 LANGUAGES C CXX)

# ---- C++ standard ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Options ----
# Single-config generators (Ninja/Unix Makefiles) use CMAKE_BUILD_TYPE; Multi-config (MSVC) use configs.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---- Raylib ----
# Prefer package config provided by MSYS2/distro installs
#   MSYS2: pacman -S mingw-w64-x86_64-raylib
#   Ubuntu: apt install libraylib-dev (package name may vary)
find_package(raylib CONFIG REQUIRED)

# ---- Sources ----
# Keep it maintainable: auto-collect .cpp under game_index
file(GLOB_RECURSE VM_SOURCES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/game_index/*.cpp")

add_executable(void_march ${VM_SOURCES})

target_include_directories(void_march
  PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/game_index"
    "${PROJECT_SOURCE_DIR}/game_index/include"
)

target_link_libraries(void_march PRIVATE raylib)

# ---- Rust (math_core) integration ----
# Build with host toolchain (no --target). Cross-compiling can be added later.

# Where Cargo will output the static lib for a --release build (host target)
set(RUST_LIB_DIR  "${PROJECT_SOURCE_DIR}/math_core/target/x86_64-pc-windows-gnu/release")
set(RUST_LIB_PATH "${RUST_LIB_DIR}/libmath_core.a")

# Tell Ninja EXACTLY which file is produced and HOW to build it
add_custom_command(
  OUTPUT "${RUST_LIB_PATH}"
  COMMAND ${CMAKE_COMMAND} -E env CARGO_TERM_COLOR=always cargo build --release
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/math_core"
  COMMENT "Cargo: building math_core (host) -> ${RUST_LIB_PATH}"
  VERBATIM
)

# A target that depends on that output file
add_custom_target(math_core_build ALL
  DEPENDS "${RUST_LIB_PATH}"
)

# Import the produced archive as an imported static library
add_library(math_core STATIC IMPORTED GLOBAL)
set_target_properties(math_core PROPERTIES
  IMPORTED_LOCATION "${RUST_LIB_PATH}"
)

# Ensure build order: build Rust before linking the exe
add_dependencies(void_march math_core_build)

# Link Rust lib into the C++ exe
target_link_libraries(void_march PRIVATE math_core)
