#include "Input/InputManager.h"

namespace Flow
{
	InputManager::InputManager()
	{

	}

	InputManager::~InputManager()
	{

	}

	void InputManager::BindKey(sf::Keyboard::Key PhysicalKey, uint32_t ActionID, int PlayerIndex)
	{
		// Store the binding in our dictionary
		KeyBindings[PhysicalKey] = { ActionID, PlayerIndex };

		// Initialize its tracking state to false
		KeyStates[PhysicalKey] = false;

	}

	void InputManager::ProcessEvent(const sf::Event& event)
	{
		// SFML 3.0 STANDARD: Type-safe event extraction using getIf<T>()

		// 1. Check for a Key Press
		if (const auto* KeyPressed = event.getIf<sf::Event::KeyPressed>())
		{
			// Does this key exist in our dictioneary?
			auto it = KeyBindings.find(KeyPressed->code);
			if (it != KeyBindings.end())
			{
			    // Prevent the OS from spamming "Pressed" if the user holds the key down
				if (!KeyStates[KeyPressed->code])
				{
					KeyStates[KeyPressed->code] = true; // Mark as currently down

					// Push a pure data packed to the Sandbox
					ActionBuffer.push_back({
						it->second.ActionID,
						it->second.PlayerIndex,
						ActionState::Pressed,
						1.0f
						});
				}

			}
		}

		// 2. Check for a Key Release
		else if (const auto* KeyReleased = event.getIf<sf::Event::KeyReleased>())
		{
			auto it = KeyBindings.find(KeyReleased->code);
			if (it != KeyBindings.end())
			{
				KeyStates[KeyReleased->code] = false; // Mark as released

				ActionBuffer.push_back({
					it->second.ActionID,
					it->second.PlayerIndex,
					ActionState::Released,
					0.0f
					});
			}
		}

	}

	void InputManager::UpdateContinousStates()
	{
		// Loop through every keys that we care about in the dictionary
		for (const auto& [PhysicalKey, Binding] : KeyBindings)
		{
			// SFML 3.0 Direct Hardware Polling
			if (sf::Keyboard::isKeyPressed(PhysicalKey))
			{
				// We only generate a "Held" action if the state is recorded as true
				// (Meaning it succesfully passed through the Pressed Phase first)
				if (KeyStates[PhysicalKey])
				{
					ActionBuffer.push_back({
						Binding.ActionID,
						Binding.PlayerIndex,
						ActionState::Held,
						1.0f
						});
				}
			}
		}
	}

	void InputManager::ClearFrame()
	{
		// Wipe the array so the sandbox doesn't process the same actions twice
		ActionBuffer.clear();
	}
}