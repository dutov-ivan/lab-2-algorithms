cmake_minimum_required(VERSION 3.20)
project(lab-2-algorithms VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release when no build type is specified (useful for single-config generators)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif ()

# Export compilation database for tools (clangd, clang-tidy, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Put built executables into a bin/ folder inside the build dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Collect sources from src/ so adding/removing files doesn't require editing CMakeLists
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES}
        src/common.cpp)

# Public include directory
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Compiler warnings and sensible defaults
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    # Treat some warning classes as errors in CI/local development (optional)
    # target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
endif ()

# Install rules (optional): install the executable and headers
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

# === GoogleTest ===
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723
)
FetchContent_MakeAvailable(googletest)

# === Test executable ===
add_executable(tests test/BoardTests.cpp
        src/common.cpp)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests PRIVATE gtest_main gmock_main)

enable_testing()
add_test(NAME ExternalSortTests COMMAND tests)