// Sandbox/src/SandboxApp.cpp
#include "Application.h"
#include "EntryPoint.h"
#include "ECS/Components.h"
#include "Utils/Log.h"
#include "Input/StringHash.h"
#include "ECS/Systems/PhysicsSystem.h" 
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/CollisionSystem.h"

class Sandbox : public Flow::Application 

{
public:
    virtual void OnStart() override 
    
    {
        auto player = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(player, 100.0f, 100.0f);
        GetRegistry().emplace<Flow::Velocity>(player, 0.0f, 0.0f);
        GetRegistry().emplace<Flow::BoxCollider>(player, 40.0f, 40.0f); // 40x40 Box

        auto wall = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(wall, 300.0f, 100.0f);
        GetRegistry().emplace<Flow::BoxCollider>(wall, 40.0f, 100.0f); // 40x100 Box (A vertical wall)

        FLOW_LOG_INFO("Sandbox: Entity spawned with Transform!");
    }

    virtual void OnUpdate(float deltaTime) override 
    {
        // Orchestrate the Physics System
        auto& actions = GetInputManager().GetFrameActions();
        Flow::PhysicsSystem::Update(GetRegistry(), actions, deltaTime);

        Flow::CollisionSystem::Update(GetRegistry());
    }

   virtual void Draw() override 
   {
        // Orchestrate the Render System
        Flow::RenderSystem::Draw(GetRegistry(), GetWindow());
   }
};

// It tells the EntryPoint.h (in the Flow library) that this 
// Sandbox class exists and how to create it.
Flow::Application* Flow::CreateApplication()
{
    return new Sandbox();
}