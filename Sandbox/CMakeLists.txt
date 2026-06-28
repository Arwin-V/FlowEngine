project(Sandbox)

# 1. Gather the Sandbox source code
set(SANDBOX_SOURCES
    src/SandboxApp.cpp
)

# 2. Build the Sandbox as the actual EXECUTABLE (.exe)
add_executable(Sandbox ${SANDBOX_SOURCES})

# 3. Link the Flow Engine directly into the Sandbox
target_link_libraries(Sandbox PRIVATE Flow)

# 4. Automate Asset Deployment
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Configs
    $<TARGET_FILE_DIR:Sandbox>/Configs
    COMMENT "Copying Configs folder to the exact binary directory..."

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/Assets 
            $<TARGET_FILE_DIR:Sandbox>/Assets
    COMMENT "Syncing entire Assets tree to binary directory..."
)
)