#include "Application.h"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp> // for clock functions to work
#include <iostream>
#include <optional>


namespace Flow
{
	Application::Application() {}
	Application::~Application() {}

	void Application::Run()
	{
		std::cout << "Flow [Application] Master Loop Started. \n";

		// 1. Boot internal engine systems
		EngineInstance.InitializeEngine();

		// 2. Call the Sandbox's initialization hook
		OnStart();

		// Get a mutable ref to the actual window
		sf::RenderWindow& Window = EngineInstance.GetWindow();


		// 3. Setup the Stopwatch (SFML Clock)
		sf::Clock clock;
		Accumulator = 0.0f;

		// 4. The Infinite Master Loop
		while (bIsApplicationRunning)
		{
			// -- Input Phase 1 --.....................................................

			// SFML 3.0 c++17 Event polling . Keeps the screen active without freezing
			// pollEvent() returns and optional event, creating it in the loop condition
			while (const std::optional event = Window.pollEvent())
			{
				// Strict template checking is used (->ist<T>) instead of checking an integer type
				if (event->is<sf::Event::Closed>())
				{
					bIsApplicationRunning = false;
				}

				// Dereference the optional event (*event) and pass it to the Input Manager
				EngineInstance.GetInputManager().ProcessEvent(*event);
			}

			// -- Input Phase 2 --
			// Continous Hardware Sweep

			// Must happen exaclty once per frame, immediately after polling OS events
			EngineInstance.GetInputManager().UpdateContinousStates();

			// Delat Time and Physics Accumulator logic................................

			// Get how much time passed since the exact end of the last loop
			RealDeltaTime = clock.restart().asSeconds();

			// Time since application started without accounting to lag or game pauses
			RealElapsedTime += RealDeltaTime;  

			// Delta time that adjusts itself to pause and freezes
			GameDeltaTime = RealDeltaTime;

			// Safety Clamp: If the game freezes, don't let the Accumulator explode
			if (GameDeltaTime > 0.25f)
			{
				GameDeltaTime = 0.25f;
			}

			// Time since application started with accounting to lag and game pauses
			GameElapsedTime += GameDeltaTime;

			// Pour the time into the bucket
			Accumulator += GameDeltaTime;

			// The Physicist wakes up
			while (Accumulator >= FixedTimeStep)
			{
				OnUpdate(FixedTimeStep); // Always passes exactly 16.66ms
				Accumulator -= FixedTimeStep;
			}

			//..................RENDER PIPELINE............................

			// 1. Wipe the old frame clean( Black bg)
			Window.clear(sf::Color::Black);

			// 2. The sandbox draws whatever state the Universe currently is in
			Draw();

			// 3. Swap the graphics buffer to physical monitor
			Window.display();

			// ---------------------------------------------------------
			// DIAGNOSTIC TEST: Read the buffer before it clears
			// ---------------------------------------------------------
			for (const auto& action : EngineInstance.GetInputManager().GetFrameActions())
			{
				std::string StateString;
				if (action.State == Flow::ActionState::Pressed) StateString = "PRESSED";
				else if (action.State == Flow::ActionState::Held) StateString = "HELD";
				else if (action.State == Flow::ActionState::Released) StateString = "RELEASED";

				std::cout << "[INPUT] Player: " << action.PlayerIndex
					<< " | ActionID: " << action.ActionID
					<< " | State: " << StateString << "\n";
			}
			// ---------------------------------------------------------

			// --- INPUT PHASE 3 ---
			// Flush the buffer
			// Must happen at the exact end of the frame, after everything is drawn.
			// If you forget this, the Sandbox will process the same "Shoot" command forever.
			EngineInstance.GetInputManager().ClearFrame();

		}

		// 5. Clean up once the loop breaks
		EngineInstance.ShutdownEngine();
		std::cout << "Flow [Application] Master Loop Terminated \n";
	}
}