#include "Engine.h"
#include <iostream>
#include <Utils/IniParser.h>
#include <imgui-SFML.h>



namespace Flow
{
	Engine::Engine() : bIsEngineRunning(false) {}
	
	Engine::~Engine() {}


    void Engine::InitializeEngine()
    {
        std::cout << "Flow [Engine] Core Subsystems Initializing \n";

        LoadConfiguration();
        BootWindow();

        // INITIALIZE IMGUI-SFML BACKEND VIEWPORTS---------------------
        // Cast to void satisfies the nodiscard safety attribute under strict compilation levels
        (void)ImGui::SFML::Init(Window);

        


       
        /////-------------------------------------------------------
        // ---------------------------------------------------------
        // Activate the Default Input Context
        // ---------------------------------------------------------
        auto& LoadedContexts = ConfigSys.GetInputContexts();

        // Ensure the "Gameplay" context was actually found in the INI file
        if (LoadedContexts.find("Gameplay") != LoadedContexts.end())
        {
            // Pass the memory address of the Gameplay context to the top of the stack
            InputSys.PushContext(&LoadedContexts.at("Gameplay"));
        }
        else
        {
            std::cerr << "Flow [Engine] WARNING: 'Gameplay' Input Context missing from Engine.ini!\n";
        }
        // ---------------------------------------------------------

               bIsEngineRunning = true;
    }

    void Engine::LoadConfiguration()
    {
        // 1. Tell the manager to load and automatically serialize the file
        ConfigSys.Load("Configs/Engine.ini");

        std::cout << "Flow [Engine] Configuration Applied Successfully.\n";
        std::cout << "  -> Resolution : " << GetConfig().WindowWidth << "x" << GetConfig().WindowHeight << "\n";
        std::cout << "  -> Window Name: " << GetConfig().WindowName << "\n";
    }

    void Engine::BootWindow()
    {
        // 3. Boot the window
        sf::State WindowState = GetConfig().bIsFullscreen ? sf::State::Fullscreen : sf::State::Windowed;

        Window.create(
            sf::VideoMode({ GetConfig().WindowWidth, GetConfig().WindowHeight }),
            GetConfig().WindowName,
            WindowState);
    }

	void Engine::RunEngine()
	{
		// Reserved for internal engine-only ticking if necessary.
		// Separate from the Application's gameplay loop.
	}

	void Engine::ShutdownEngine()
	{
		std::cout << "Flow [Engine] Core Subsystems Shutting Down....\n";

        // CLEANUP IMGUI CORE ALLOCATIONS BEFORE DEVICE DISPOSAL
        ImGui::SFML::Shutdown();

		bIsEngineRunning = false;
	}
}