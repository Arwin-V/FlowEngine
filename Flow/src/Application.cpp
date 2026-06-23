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

		// Get a murable ref to the actual window
		sf::RenderWindow& Window = EngineInstance.GetWindow();


		// 3. Setup the Stopwatch (SFML Clock)
		sf::Clock clock;
		Accumulator = 0.0f;

		// 4. The Infinite Master Loop
		while (bIsApplicationRunning)
		{
			// SFML 3.0 c++17 Event polling . Keeps the screen active without freezing
			// pollEvent() returns and optional event, creating it in the loop condition
			while (const std::optional event = Window.pollEvent())
			{
				// Strict template checking is used (->ist<T>) instead of checking an integer type
				if (event->is<sf::Event::Closed>())
				{
					bIsApplicationRunning = false;
				}
			}



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

		}

		// 5. Clean up once the loop breaks
		EngineInstance.ShutdownEngine();
		std::cout << "Flow [Application] Master Loop Terminated \n";
	}
}