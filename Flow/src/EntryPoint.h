#pragma once
#include "Application.h"
#include <iostream>

extern Flow::Application* Flow::CreateApplication();

int main(int argc, char** argv)
{
	std::cout << "Flow Engine | Booting System Architecture........\n";

	// The engine instantiates the client game application dynamically via the link hook
	auto app = Flow::CreateApplication();

	// The engine takes complete control over execution (Inversion of Control)
	app->Run();

	// Clean up when the engine terminates
	delete app;

	std::cout << "Flow Engine Shutdown Complete.\n";
}