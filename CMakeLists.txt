cmake_minimum_required(VERSION 3.20)

# Name your entire workspace
project(FlowEngine VERSION 1.0)

# Enforce strict warning-as-error policy across all platforms
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Force Modern C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force Static Linking globally
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Put the final .exe in a clean bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# 1. Download official SFML 3.0.0
message(STATUS "Fetching SFML 3.0.0...")
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

# 2. Download Core Dear ImGui
message(STATUS "Fetching Core ImGui...")
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.5
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "Core ImGui Directory" FORCE)

# 3. Download ImGui-SFML bridge
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
message(STATUS "Fetching ImGui-SFML...")
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG master 
)
FetchContent_MakeAvailable(ImGui-SFML)

# ---------------------------------------------------------
# 4. Download EnTT (Entity Component System)
# ---------------------------------------------------------
message(STATUS "Fetching EnTT 3.13.0...")
FetchContent_Declare(entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.13.0
)
FetchContent_MakeAvailable(entt)
# ---------------------------------------------------------

# 5. Tell CMake to process our two sub-folders
add_subdirectory(Flow)
add_subdirectory(Sandbox)

