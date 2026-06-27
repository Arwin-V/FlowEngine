// Sandbox/src/SandboxApp.cpp
#include "Application.h"
#include "EntryPoint.h"
#include "ECS/Components.h"
#include "Utils/Log.h"
#include "Input/StringHash.h"

class Sandbox : public Flow::Application {
public:
    virtual void OnStart() override {
        // Create an entity and give it a Transform
        auto entity = GetRegistry().create();
        GetRegistry().emplace<Flow::Transform>(entity, 100.0f, 100.0f);
        GetRegistry().emplace<Flow::Velocity>(entity, 0.0f, 0.0f);

        FLOW_LOG_INFO("Sandbox: Entity spawned with Transform!");
    }

    virtual void OnUpdate(float deltaTime) override {
        // 1. Get input actions from the Engine
        auto& actions = GetInputManager().GetFrameActions(); // Adjust based on your Engine structure
        // If EngineInstance is accessible via GetRegistry(), add a GetInputManager() to Application too!

        // DEBUG: Print exactly how many actions the Sandbox sees
        if (!actions.empty()) {
            FLOW_LOG_INFO("Sandbox saw " << (int)actions.size() << " actions this frame");
        }

        auto view = GetRegistry().view<Flow::Transform, Flow::Velocity>();

        for (auto entity : view) {
            auto& transform = view.get<Flow::Transform>(entity);
            auto& velocity = view.get<Flow::Velocity>(entity);

            // 1. Pre-process input: Determine if the button is currently "Active"
            bool bIsThrustPressed = false;

            for (const auto& action : actions) {
                if (action.ActionID == Flow::HashString("P1_THRUST")) {
                    // If it's pressed OR held, we want to move
                    if (action.State == Flow::ActionState::Pressed || action.State == Flow::ActionState::Held) {
                        bIsThrustPressed = true;
                    }
                }
            }

            // 2. Apply movement based on the state flag, not the event
            if (bIsThrustPressed) {
                velocity.vx = 200.0f;
            }
            else {
                velocity.vx = 0.0f; // Stopped
            }

            // 3. Apply Velocity to Transform
            transform.x += velocity.vx * deltaTime;
        }
    }

    virtual void Draw() override 
    {
        // 1. Get the window from the engine
        sf::RenderWindow& window = GetWindow();

        // 2. Iterate through all entities that have a Transform
        auto view = GetRegistry().view<Flow::Transform>();

        for (auto entity : view) {
            auto& transform = view.get<Flow::Transform>(entity);

            // 3. Create a simple shape for debugging
            sf::CircleShape circle(20.0f);
            circle.setFillColor(sf::Color::Green);
            circle.setPosition({ transform.x, transform.y });

            // 4. Draw it to the window
            window.draw(circle);
        }

    }
};

// It tells the EntryPoint.h (in the Flow library) that this 
// Sandbox class exists and how to create it.
Flow::Application* Flow::CreateApplication()
{
    return new Sandbox();
}