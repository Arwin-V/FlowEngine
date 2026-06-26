#include "ConfigManager.h"
#include "Utils/IniParser.h"
#include <iostream>
#include "Input/KeyMapper.h"   
#include "Input/StringHash.h"



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

			// ---------------------------------------------------------
			// Intercept Input Bindings (Look for the '.')
			// ---------------------------------------------------------
			size_t DotPos = Key.find('.');
			if (DotPos != std::string::npos)
			{
				std::string ContextName = Key.substr(0, DotPos);
				std::string ActionName = Key.substr(DotPos + 1);

				// If this context doesn't exist yet, create it
				if (InputContexts.find(ContextName) == InputContexts.end())
				{
					InputContexts.emplace(ContextName, InputContext(ContextName));
				}

				// 1. Translate string to hardware key
				sf::Keyboard::Key PhysKey = KeyMapper::GetKeyFromString(Value);

				// 2. Hash the action name (Runtime hashing!)
				uint32_t ActionID = HashString(ActionName.c_str());

				// 3. Extract Player Index automatically based on our "P1_", "P2_" prefix
				int PlayerIndex = 0;
				if (ActionName.substr(0, 3) == "P2_") PlayerIndex = 1;
				else if (ActionName.substr(0, 3) == "P3_") PlayerIndex = 2;
				else if (ActionName.substr(0, 3) == "P4_") PlayerIndex = 3;

				// 4. Bind it to the specific profile
				InputContexts.at(ContextName).BindKey(PhysKey, ActionID, PlayerIndex);
				std::cout << "Flow [ConfigManager] Mapped Input: [" << ContextName << "] " << ActionName << " -> " << Value << "\n";

				continue; // Skip the rest of the loop for this key

				// ---------------------------------------------------------------
			}


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


