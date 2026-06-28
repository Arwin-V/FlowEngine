// Sandbox/src/SandboxApp.cpp
#include "Application.h"
#include "EntryPoint.h"
#include "ECS/Components.h"
#include "Utils/Log.h"
#include "Input/StringHash.h"
#include "ECS/Systems/PhysicsSystem.h" 
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/CollisionSystem.h"
#include "ECS/Systems/PlatformerMovementSystem.h"
#include "ECS/Systems/PlatformCollisionSystem.h"
#include "Utils/AssetManager.h"
#include "ECS/Systems/AnimationSystem.h"

class Sandbox : public Flow::Application 

{
public:
    virtual void OnStart() override
    {
        std::cout << "Current working directory is: " << std::filesystem::current_path() << std::endl;

        // 1. THE FLOOR (Static World Bounds)
        // ---------------------------------------------------------
        auto floor = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(floor, 0.0f, 650.0f);
        auto& floorBox = GetRegistry().emplace<Flow::BoxCollider>(floor, 1280.0f, 50.0f);
        floorBox.objectType = Flow::CollisionChannel::WorldStatic;

        // ---------------------------------------------------------
        // 2. JUMPING PLATFORMS (Static)
        // ---------------------------------------------------------
        auto plat1 = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(plat1, 300.0f, 500.0f);
        GetRegistry().emplace<Flow::BoxCollider>(plat1, 200.0f, 20.0f);

        auto plat2 = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(plat2, 700.0f, 400.0f);
        GetRegistry().emplace<Flow::BoxCollider>(plat2, 200.0f, 20.0f);

        auto plat3 = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(plat3, 400.0f, 250.0f);
        GetRegistry().emplace<Flow::BoxCollider>(plat3, 150.0f, 20.0f);

        // ---------------------------------------------------------
        // 3. THE KINEMATIC PLAYER (Red Square)
        // ---------------------------------------------------------
        auto player = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(player, 100.0f, 500.0f); // Start on the left, above floor
        GetRegistry().emplace<Flow::Velocity>(player, 0.0f, 0.0f);

        // Use the NEW PlatformerController instead of RigidBody!
        auto& pCtrl = GetRegistry().emplace<Flow::PlatformerController>(player);
        pCtrl.moveSpeed = 400.0f;
        pCtrl.jumpForce = 700.0f; // Tweak this for Mario feel

        auto& pBox = GetRegistry().emplace<Flow::BoxCollider>(player, 40.0f, 40.0f);
        pBox.objectType = Flow::CollisionChannel::Player; 

        // The Designer-Friendly Loading System:
        auto& pAnim = GetRegistry().emplace<Flow::AnimatorComponent>(player);
        Flow::AssetManager::Get().BuildAnimatorFromJson("Assets/Hero/Hero.json", pAnim);

        

        
        FLOW_LOG_INFO("Sandbox: Platformer Testbed Initialized!");
    }

    virtual void OnUpdate(float DeltaTime) override
    {
        auto& Actions = GetInputManager().GetFrameActions();

        Flow::PlatformMovementSystem::Update(GetRegistry(), Actions, DeltaTime);
        Flow::PlatformCollisionSystem::Update(GetRegistry(), DeltaTime);
        Flow::AnimationSystem::Update(GetRegistry(), DeltaTime);

        // F3 Debug Toggle (Unreal Naming & Allman Style)
        static bool bF3WasPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3))
        {
            if (!bF3WasPressed)
            {
                Flow::RenderSystem::bDrawDebugColliders = !Flow::RenderSystem::bDrawDebugColliders;
                bF3WasPressed = true;
            }
        }
        else
        {
            bF3WasPressed = false;
        }
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

/* Test for platformer code
 // ---------------------------------------------------------
        // 1. THE FLOOR (Static World Bounds)
        // ---------------------------------------------------------
        auto floor = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(floor, 0.0f, 650.0f);
        auto& floorBox = GetRegistry().emplace<Flow::BoxCollider>(floor, 1280.0f, 50.0f);
        floorBox.objectType = Flow::CollisionChannel::WorldStatic;

        // ---------------------------------------------------------
        // 2. JUMPING PLATFORMS (Static)
        // ---------------------------------------------------------
        auto plat1 = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(plat1, 300.0f, 500.0f);
        GetRegistry().emplace<Flow::BoxCollider>(plat1, 200.0f, 20.0f);

        auto plat2 = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(plat2, 700.0f, 400.0f);
        GetRegistry().emplace<Flow::BoxCollider>(plat2, 200.0f, 20.0f);

        auto plat3 = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(plat3, 400.0f, 250.0f);
        GetRegistry().emplace<Flow::BoxCollider>(plat3, 150.0f, 20.0f);

        // ---------------------------------------------------------
        // 3. THE KINEMATIC PLAYER (Red Square)
        // ---------------------------------------------------------
        auto player = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(player, 100.0f, 500.0f); // Start on the left, above floor
        GetRegistry().emplace<Flow::Velocity>(player, 0.0f, 0.0f);

        // Use the NEW PlatformerController instead of RigidBody!
        auto& pCtrl = GetRegistry().emplace<Flow::PlatformerController>(player);
        pCtrl.moveSpeed = 400.0f;
        pCtrl.jumpForce = 700.0f; // Tweak this for Mario feel

        auto& pBox = GetRegistry().emplace<Flow::BoxCollider>(player, 40.0f, 40.0f);
        pBox.objectType = Flow::CollisionChannel::Player;*/