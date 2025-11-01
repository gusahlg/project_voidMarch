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
    "${PROJECT_SOURCE_DIR}/game_index/game_logic"
    "${PROJECT_SOURCE_DIR}/game_index/game_logic/items"
    "${PROJECT_SOURCE_DIR}/game_index/game_logic/systems/combat"
    "${PROJECT_SOURCE_DIR}/game_index/include"
)

target_link_libraries(void_march PRIVATE raylib)

# ---- Rust (math_core) integration: stable & cross-platform ----

# 0) Locate cargo/rustc from MSYS2 on Windows (avoid Windows rustup MSVC)
if(WIN32)
  # Prefer the MSYS2 MinGW toolchain explicitly
  find_program(CARGO_EXECUTABLE cargo HINTS /mingw64/bin)
  find_program(RUSTC_EXECUTABLE rustc HINTS /mingw64/bin)
else()
  find_program(CARGO_EXECUTABLE cargo)
  find_program(RUSTC_EXECUTABLE rustc)
endif()

if(NOT CARGO_EXECUTABLE OR NOT RUSTC_EXECUTABLE)
  message(FATAL_ERROR "cargo and/or rustc not found. Install Rust (MSYS2 on Windows).")
endif()

# 1) Decide the Rust target triple to build for
#    - If we are on Windows+GCC (MinGW), force 'x86_64-pc-windows-gnu'
#    - Otherwise, detect host triple via 'rustc -vV'
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(RUST_TARGET "x86_64-pc-windows-gnu")
else()
  execute_process(
    COMMAND ${RUSTC_EXECUTABLE} -vV
    OUTPUT_VARIABLE RUSTC_VV
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  string(REGEX MATCH "host: ([^\n\r]+)" _match "${RUSTC_VV}")
  if(NOT CMAKE_MATCH_1)
    message(FATAL_ERROR "Failed to detect rustc host triple from:\n${RUSTC_VV}")
  endif()
  set(RUST_TARGET "${CMAKE_MATCH_1}")
endif()

# 2) Define where the static lib should appear for that target/config
set(RUST_LIB_DIR  "${PROJECT_SOURCE_DIR}/math_core/target/${RUST_TARGET}/release")
set(RUST_LIB_PATH "${RUST_LIB_DIR}/libmath_core.a")

# 3) Explicit build rule that produces the archive at that exact path
add_custom_command(
  OUTPUT "${RUST_LIB_PATH}"
  COMMAND ${CMAKE_COMMAND} -E env CARGO_TERM_COLOR=always
          ${CARGO_EXECUTABLE} build --release --target ${RUST_TARGET}
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/math_core"
  COMMENT "Cargo: building math_core for ${RUST_TARGET} -> ${RUST_LIB_PATH}"
  VERBATIM
)

# 4) A phony target that depends on the output file
add_custom_target(math_core_build ALL
  DEPENDS "${RUST_LIB_PATH}"
)

# 5) Import the produced archive and make the C++ exe wait for it
add_library(math_core STATIC IMPORTED GLOBAL)
set_target_properties(math_core PROPERTIES IMPORTED_LOCATION "${RUST_LIB_PATH}")

add_dependencies(void_march math_core_build)
target_link_libraries(void_march PRIVATE math_core)
