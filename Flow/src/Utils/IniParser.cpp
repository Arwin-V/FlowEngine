#include "Utils/IniParser.h"
#include <fstream>
#include <iostream>

namespace Flow
{
	std::unordered_map<std::string, std::string> IniParser::Load(const std::string& FilePath)
	{
		std::unordered_map<std::string, std::string> ParsedData;

		std::cout << "Flow [IniParser] Reading file from the disk: " << FilePath << "\n";
		std::ifstream File(FilePath);

		// If the file is missing, return the empty map so the engine can safely fallback to defaults
		if (!File.is_open())
		{
			std::cerr << "Flow [IniParser] WARNING: Could not open " << FilePath << ".\n";
			return ParsedData;
		}

		std::string Line;
		while (std::getline(File, Line))
		{
			// Skip empty lines and [Category] headers of comments
			if (Line.empty() || Line[0] == '[' || Line[0] == ';')
			{
				continue;
			}

			// Find the delimiter
			size_t DelimiterPos = Line.find('=');
			if (DelimiterPos != std::string::npos)
			{
				// Extract key and value
				std::string Key = Line.substr(0, DelimiterPos);
				std::string Value = Line.substr(DelimiterPos + 1);

				// Store in the map
				ParsedData[Key] = Value;
			}
		}

		File.close();
		return ParsedData;
	}

}