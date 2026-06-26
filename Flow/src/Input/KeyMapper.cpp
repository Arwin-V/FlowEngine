#include "Input/KeyMapper.h"
#include <unordered_map>
#include "Utils/Log.h"

namespace Flow
{
	sf::Keyboard::Key KeyMapper::GetKeyFromString(const std::string& KeyName)
	{
		// A static map ensures this dictionary is only created in memory once, not every time we call the function.
		static const std::unordered_map<std::string, sf::Keyboard::Key> KeyMap = {
			{"W", sf::Keyboard::Key::W},
			{"A", sf::Keyboard::Key::A},
			{"S", sf::Keyboard::Key::S},
			{"D", sf::Keyboard::Key::D},
			{"Up", sf::Keyboard::Key::Up},
			{"Down", sf::Keyboard::Key::Down},
			{"Left", sf::Keyboard::Key::Left},
			{"Right", sf::Keyboard::Key::Right},
			{"Space", sf::Keyboard::Key::Space},
			{"Enter", sf::Keyboard::Key::Enter},
			{"Escape", sf::Keyboard::Key::Escape}
		};

		auto it = KeyMap.find(KeyName);
		if (it != KeyMap.end())
		{
			return it->second;
		}

		// If the user types "Shoot=Banana" in the INI, this catches it safely.
		FLOW_LOG_WARN("KeyMapper: Unknown key string '" + KeyName + "'. Defaulting to Unknown.");
		return sf::Keyboard::Key::Unknown;
	}
}