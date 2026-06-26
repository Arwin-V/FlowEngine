#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <string>

namespace Flow
{
	class KeyMapper
	{
	public:
		/// Converts a string like "Space" to sf::Keyboard::Key::Space
		static sf::Keyboard::Key GetKeyFromString(const std::string& KeyName);
	};
}