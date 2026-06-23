#pragma once
#include "Engine.h"

namespace Flow
{
	class Application 
	{
	public:
		Application();
		virtual ~Application();

		// Gameplay Hooks (Inversion of Control)
		virtual void OnStart() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void Draw() = 0;

		void Run();

		float GetGameDeltaTime() const { return GameDeltaTime; }
		float GetRealDeltaTime() const { return RealDeltaTime; }
		float GetAccumulator() const { return Accumulator; }
		float GetFixedTimeStep() const { return FixedTimeStep; }

		double GetRealElapsedTime() const { return RealElapsedTime; }
		double GetGameElapsedTime() const { return GameElapsedTime; }

	private:
		Engine EngineInstance;
		bool bIsApplicationRunning = true;

		float GameDeltaTime = 0.0f;
		float RealDeltaTime = 0.0f;
		float Accumulator = 0.0f;

		double RealElapsedTime = 0;
		double GameElapsedTime = 0;

		const float FixedTimeStep = 1.0f / 60.0f; // Target: ~60 FPS (16.66ms)
	};

	// Client definition: Must be implemented by the game side
	Application* CreateApplication();
}