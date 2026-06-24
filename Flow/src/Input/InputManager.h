#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include <vector>
#include "Input/InputTypes.h"
#include "Input/StringHash.h"



namespace Flow
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();


		// 1. Setup & Configuration (Called during game boot).............................
	
		// Maps a physical keyboard button to an abstract hashed string ID
		void BindKey(sf::Keyboard::Key PhysicalKey, uint32_t ActionID, int PlayerIndex = 0);

		// 2. The Core Engine Loop Hook (Called every frame).............................

		// Catches discrete events from the OS (Just Pressed, Just Released)
		void ProcessEvent(const sf::Event& event);

		// Sweeps the hardware for continuous data (held buttons, Analog Sticks)
		void UpdateContinousStates();

		// Clears the buffer at the exact end of a frame so actions don't repeat
		void ClearFrame();


		// 3. The API for Layer 2 (Application) and Layer 3 (Sandbox).....................

		// Returns the raw list of everything that happened this exact frame
		const std::vector<GameAction>& GetFrameActions() const { return ActionBuffer; }

	private:
		// The Data Pipeline : A contigious array of actions generated this frame
		std::vector <GameAction> ActionBuffer;

		// The Dictionary: Maps [Physical Key] -> [ActionID, PlayerIndex]
		// Using a struct for the map value keeps the memory tightly packed
		struct BindingData
		{
			uint32_t ActionID;
			int PlayerIndex;
		};
		
		std::unordered_map<sf::Keyboard::Key, BindingData> KeyBindings;

		// Tracks which actions are currently held down (to prevent spamming "Just Pressed" events)
		std::unordered_map<sf::Keyboard::Key, bool> KeyStates;
		
		
	}; 
}