project(Flow)

# 1. Gather all Engine source files
set(FLOW_SOURCES
    src/Application.cpp
    src/Engine.cpp
    src/Utils/IniParser.cpp
    src/Utils/ConfigManager.cpp
    src/Input/InputManager.cpp
)

# 2. Build the Flow Engine as a STATIC LIBRARY (.lib), not an executable
add_library(Flow STATIC ${FLOW_SOURCES} "src/ConfigManager.h" "src/Utils/ConfigManager.cpp" "src/Utils/Log.h" "src/Input/StringHash.h" "src/Input/InputTypes.h" "src/Input/InputManager.h" "src/Input/InputManager.cpp")

# 3. Expose the Flow/src folder so the Sandbox can see our header files
target_include_directories(Flow PUBLIC src)

# 4. Link SFML and ImGui to the Flow Engine
# We use PUBLIC so that the Sandbox automatically inherits these tools too!
target_link_libraries(Flow PUBLIC 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    ImGui-SFML::ImGui-SFML
)