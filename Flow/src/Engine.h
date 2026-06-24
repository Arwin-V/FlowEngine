#pragma once
#include <SFML/Graphics.hpp>
#include "ConfigManager.h"
#include "Input/InputManager.h"


namespace Flow 
{
	
	class Engine
	{
	public:
		Engine();
		~Engine();

		void InitializeEngine();
		void RunEngine();
		void ShutdownEngine();

		// Returns a mutable reference to the Engine's window.
		// The Application may call drawing functions, but the Engine retains sole memory ownership.
		sf::RenderWindow& GetWindow() { return Window; }

		InputManager& GetInputManager() { return InputSys; }

		bool IsRunning() const { return bIsEngineRunning; }
		void Stop() { bIsEngineRunning = false; }

	private:

		// 3. Engine permanently owns input Manager in RAM
		InputManager InputSys;

		// Engine Status
		bool bIsEngineRunning = false;
		
		// Canvas
		sf::RenderWindow Window;
	
		// Engine Congigurations...................................
		ConfigManager ConfigSys;

		// 2. Grab a read-only copy of the final variables
		inline const EngineConfig& GetConfig() const { return ConfigSys.GetConfig(); }

		// Boot functions
		void LoadConfiguration();
		void BootWindow();

	};
}