#include "ConfigManager.h"
#include "Utils/IniParser.h"
#include <iostream>



namespace Flow
{
	ConfigManager::ConfigManager()
	{
		// 1. Properties are registered once the manager is created
		// If  want to add a new variable, add it to the struct, and type one line here.
		RegisterProperty("WindowWidth", Config.WindowWidth);
		RegisterProperty("WindowHeight", Config.WindowHeight);
		RegisterProperty("WindowName", Config.WindowName);
		RegisterProperty("FullScreen", Config.bIsFullscreen);
		RegisterProperty("DefaultPort", Config.NetworkPort);
		RegisterProperty("MaxPlayers", Config.MaxPlayers);
	}

	void ConfigManager::Load(const std::string& FilePath)
	{
		// Ask the IniParser to grab the raw text
		auto RawData = IniParser::Load(FilePath);

		// Loop through every line found in the file
		for (const auto& [Key, Value] : RawData)
		{
			// Check if we build lambda for this key
			auto Iterator = PropertyRegistry.find(Key);

			if (Iterator != PropertyRegistry.end())
			{
				try
				{
					// Trigger the lambda function. Iterator->second is the std::function)
					Iterator->second(Value);
					std::cout << "Flow [ConfigManager] Succesfully mapped: " << Key << "\n";
				}
				catch (const std::exception& e)
				{
					// If t typed "WindowWidth=Banana", the stoi() conversion will fail safely here.
					std::cerr << "Flow [ConfigManager] ERROR: Corrupted data for " << Key << "Using default. \n";
				}
			}
			else
			{
				// Warning Msg if error
				// The key exists in file, but is not registerd here. Either Key name wrong or should yet register
				std::cerr << "Flow [ConfigManger] WARNING: " << Key << " in " << FilePath << "Not available in Engine Registry. \n";
			}
		}
			
	}
}


