project(Flow)

# 1. Gather all Engine source files
set(FLOW_SOURCES
    src/Application.cpp
    src/Engine.cpp
    src/Utils/IniParser.cpp
    src/Utils/ConfigManager.cpp
    src/Input/InputManager.cpp
    src/ECS/Components.h
)

# 2. Build the Flow Engine as a STATIC LIBRARY
# Make sure the file list here matches the FLOW_SOURCES or explicitly includes the new header
add_library(Flow STATIC ${FLOW_SOURCES} 
    "src/ConfigManager.h" 
    "src/Utils/ConfigManager.cpp" 
    "src/Utils/Log.h" 
    "src/Input/StringHash.h" 
    "src/Input/InputTypes.h" 
    "src/Input/InputManager.h" 
    "src/Input/InputManager.cpp" 
    "src/Input/KeyMapper.h" 
    "src/Input/KeyMapper.cpp" 
    "src/Input/InputContext.h" 
    "src/Utils/DebugOverlay.h"
    "src/ECS/Components.h"  
    "src/MathLib/CollisionMath.h" 
    "src/ECS/Systems/CollisionSystem.h")

# 3. Expose the Flow/src folder so the Sandbox can see our header files
target_include_directories(Flow PUBLIC src)

# 4. Link SFML and ImGui to the Flow Engine
# We use PUBLIC so that the Sandbox automatically inherits these tools too!
target_link_libraries(Flow PUBLIC 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    ImGui-SFML::ImGui-SFML
    EnTT::EnTT
)