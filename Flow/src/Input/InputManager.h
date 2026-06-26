#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include <vector>
#include "Input/InputTypes.h"
#include "Input/InputContext.h" // Added Context Header




namespace Flow
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		// -------------------------------------------------------------------
		// 1. Stack Management (The Context Plates)
		// -------------------------------------------------------------------

		// Adds a context to the TOP of the stack
		void PushContext(InputContext* Context);

		// Removes the top context (e.g., closing the pause menu)
		void PopContext();

		// -------------------------------------------------------------------
		// 2. The Core Engine Loop Hook
		// -------------------------------------------------------------------
		void ProcessEvent(const sf::Event& event);
		void UpdateContinousStates();
		void ClearFrame();

		const std::vector<GameAction>& GetFrameActions() const { return ActionBuffer; }

	private:
		std::vector<GameAction> ActionBuffer;

		// THE STACK: Index 0 is the bottom (Gameplay), Index N is the top (UI Menu)
		std::vector<InputContext*> ContextStack;

		// Tracks physical hardware state to prevent OS spamming (This remains global)
		std::unordered_map<sf::Keyboard::Key, bool> KeyStates;
	};
}