#pragma once
#include <string>
#include <unordered_map>

namespace Flow
{
	class IniParser
	{
	public:
		// Static method: Can be called anywhere without creating an object
		static std::unordered_map<std::string, std::string> Load(const std::string& FilePath);
	};
}