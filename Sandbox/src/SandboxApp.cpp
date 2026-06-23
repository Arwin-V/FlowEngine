#include "Application.h"
#include "EntryPoint.h"

class Sandbox : public Flow::Application
{
public:
	Sandbox() {}
	~Sandbox() {}

	virtual void OnStart() override
	{
		// Initialize your game objects here
	}

	virtual void OnUpdate(float DeltTime) override
	{
		// Your gameplay logic that gets updated every frame
	}

	virtual void Draw()
	{
		// Your gameplay logic that gets updated every frame
	}
};

// Define the allocation implementation that EntryPoint.h Calls into
Flow::Application* Flow::CreateApplication()
{
	return new Sandbox();
}