cmake_minimum_required(VERSION 3.30)
project(one LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include the FetchContent module
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

# Fetch raylib library
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib
        GIT_TAG 4.5.0
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(raylib)

# اشاره به فایل main.c در مسیر فعلی
add_executable(${PROJECT_NAME} main.c)

# Link raylib to your project
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Link additional libraries for Windows
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm ws2_32)
endif()
