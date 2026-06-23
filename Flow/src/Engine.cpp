#include "Engine.h"
#include <iostream>
#include <Utils/IniParser.h>



namespace Flow
{
	Engine::Engine() : bIsEngineRunning(false) {}
	
	Engine::~Engine() {}


    void Engine::InitializeEngine()
    {
        std::cout << "Flow [Engine] Core Subsystems Initializing \n";

        LoadConfiguration();
        BootWindow();

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
		bIsEngineRunning = false;
	}
}