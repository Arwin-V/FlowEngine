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
        // ---------------------------------------------------------
        // 1. THE FLOOR (Static Geometry)
        // ---------------------------------------------------------
        auto floor = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(floor, 0.0f, 600.0f); // Placed near the bottom of the screen
        auto& floorBox = GetRegistry().emplace<Flow::BoxCollider>(floor, 1280.0f, 50.0f); // Massive width
        // Notice: We don't add a RigidBody or Velocity. The engine treats it as a Static immovable object.

        // ---------------------------------------------------------
        // 2. THE PLAYER (Dynamic - Standard Gravity)
        // ---------------------------------------------------------
        auto player = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(player, 100.0f, 100.0f);
        GetRegistry().emplace<Flow::Velocity>(player, 0.0f, 0.0f);
        GetRegistry().emplace<Flow::PlayerController>(player);
        auto& pBox = GetRegistry().emplace<Flow::BoxCollider>(player, 40.0f, 40.0f);
        pBox.objectType = Flow::CollisionChannel::Player;

        auto& pRigid = GetRegistry().emplace<Flow::RigidBody>(player);
        pRigid.mass = 1.0f;
        pRigid.gravityScale = 1.0f;
        pRigid.linearDrag = 15.0f; // VERY high drag for snappy stopping

        // ---------------------------------------------------------
        // 3. THE HEAVY CRATE (Dynamic - Falls from the sky)
        // ---------------------------------------------------------
        auto crate = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(crate, 400.0f, -100.0f); // Starts way up high
        GetRegistry().emplace<Flow::Velocity>(crate, 0.0f, 0.0f);
        GetRegistry().emplace<Flow::BoxCollider>(crate, 80.0f, 80.0f); // Big box

        auto& cRigid = GetRegistry().emplace<Flow::RigidBody>(crate);
        cRigid.type = Flow::BodyType::Dynamic;
        cRigid.mass = 50.0f; // Very heavy
        cRigid.gravityScale = 1.5f; // Falls 50% faster than the player

        // ---------------------------------------------------------
        // 4. THE MAGIC ORB (Dynamic - Anti-Gravity)
        // ---------------------------------------------------------
        auto orb = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(orb, 600.0f, 500.0f); // Starts near the floor
        GetRegistry().emplace<Flow::Velocity>(orb, 0.0f, 0.0f);
        GetRegistry().emplace<Flow::BoxCollider>(orb, 30.0f, 30.0f);

        auto& oRigid = GetRegistry().emplace<Flow::RigidBody>(orb);
        oRigid.type = Flow::BodyType::Dynamic;
        oRigid.mass = 0.5f; // Light weight
        oRigid.gravityScale = -0.2f; // Negative gravity! It will slowly float upwards off the screen.

        FLOW_LOG_INFO("Sandbox: Physics Testbed Initialized!");
    }

    virtual void OnUpdate(float deltaTime) override // Runs in fixed time loop
    {
        // Orchestrate the Physics System
        auto& actions = GetInputManager().GetFrameActions();
       
        // 1. Move Player (Player might step inside a wall here)
        Flow::PhysicsSystem::Update(GetRegistry(), actions, deltaTime);

        // 2. Collision (If inside a wall, push them immediately back out before Render)
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