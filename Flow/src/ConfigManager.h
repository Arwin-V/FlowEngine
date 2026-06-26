#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <type_traits> // Required for compile-time type checking
#include "Input/InputContext.h"

namespace Flow
{
	// The strong-typed data container
	struct EngineConfig
	{
		// fallbacks to these defaults if error reading ini

		unsigned int WindowWidth = 800;
		unsigned int WindowHeight = 600;
		std::string WindowName = "FlowEngine Default";
		bool bIsFullscreen = false;
		unsigned int NetworkPort = 7777;
		unsigned int MaxPlayers = 1;
	};

	class ConfigManager
	{
	public:
		ConfigManager();

		// Reads the file and executes lambdas
		void Load(const std::string& FilePath);

		// Allows the Engine to read the final variables safely
		const EngineConfig& GetConfig() const { return Config; }

		// 2. Engine to grab the finished profiles
		std::unordered_map<std::string, InputContext>& GetInputContexts() { return InputContexts;  }

	private:

		EngineConfig Config;

		/*
		The Property Registry maps an INI key to a type - erased lambda.
		The lambda captures the specific memory address of a struct member by reference,
		allowing dynamic text mapping to statically-typed variables.
		*/

		std::unordered_map<std::string, std::function<void(const std::string&)>> PropertyRegistry;

		/*
		A C++17 template function using 'if constexpr'.
		This evaluates the type of 'T' at compile-time. The compiler will completely delete
		the 'if' branches that don't match, resulting in blazingly fast, zero-overhead type conversion.
		*/

		// Storage for automatically generated profiles
		std::unordered_map<std::string, InputContext> InputContexts;

		template <typename T>
		void RegisterProperty(const std::string& KeyName, T& VariableToBind)
		{
			// Lambda is created and stored in dictionary
			PropertyRegistry[KeyName] = [&VariableToBind](const std::string& RawValue)
				{
					// Compile-time checks to convert the sting to correct c++ type
					if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>)
					{
						VariableToBind = std::stoi(RawValue);
					}
					else if constexpr (std::is_same_v<T, bool>)
					{
						// Treat any non-zero number as true
						VariableToBind = (std::stoi(RawValue) != 0);
					}
					else if constexpr (std::is_same_v<T, std::string>)
					{
						VariableToBind = RawValue;
					}
				};
		}

	};


}