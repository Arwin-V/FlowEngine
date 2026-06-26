#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>
#include <string>
#include <optional>
#include "Input/InputTypes.h"

namespace Flow
{
	// Using a struct for the map value keeps the memory tightly packed
	struct BindingData
	{
		uint32_t ActionID;
		int PlayerIndex;
	};

	class InputContext
	{
	public:
		InputContext(const std::string& Name) : ContextName(Name) {}

		// Registers a physical key to an action for this specific profile
		void BindKey(sf::Keyboard::Key PhysicalKey, uint32_t ActionID, int PlayerIndex = 0)
		{
			KeyBindings[PhysicalKey] = { ActionID, PlayerIndex };
		}

		// Allows the Engine to ask: "Does this specific profile care about the 'W' key?"
		std::optional<BindingData> GetBinding(sf::Keyboard::Key PhysicalKey) const
		{
			auto it = KeyBindings.find(PhysicalKey);
			if (it != KeyBindings.end())
			{
				return it->second;
			}
			return std::nullopt; // C++17 safe way to return "nothing found"
		}

		const std::string& GetName() const { return ContextName; }

		// Allows the InputManager to sweep all keys in this specific context
		const std::unordered_map<sf::Keyboard::Key, BindingData>& GetBindings() const { return KeyBindings; }

	private:
		std::string ContextName;

		// This context's personal dictionary of hardware keys
		std::unordered_map<sf::Keyboard::Key, BindingData> KeyBindings;
	};
}