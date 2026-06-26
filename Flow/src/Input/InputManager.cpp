#include "Input/InputManager.h"
#include "Utils/Log.h"

namespace Flow
{
	InputManager::InputManager() {}
	InputManager::~InputManager() {}

	void InputManager::PushContext(InputContext* Context)
	{
		if (Context)
		{
			ContextStack.push_back(Context);
			FLOW_LOG_INFO("InputManager: Pushed Context '" + Context->GetName() + "' to stack.");
		}
	}

	void InputManager::PopContext()
	{
		if (!ContextStack.empty())
		{
			FLOW_LOG_INFO("InputManager: Popped Context '" + ContextStack.back()->GetName() + "' from stack.");
			ContextStack.pop_back();
		}
	}

	void InputManager::ProcessEvent(const sf::Event& event)
	{
		// 1. Check for a Key Press
		if (const auto* KeyPressed = event.getIf<sf::Event::KeyPressed>())
		{
			// Read the stack backwards (Top to Bottom)
			for (auto it = ContextStack.rbegin(); it != ContextStack.rend(); ++it)
			{
				// Does THIS context care about the key?
				if (auto Binding = (*it)->GetBinding(KeyPressed->code))
				{
					if (!KeyStates[KeyPressed->code])
					{
						KeyStates[KeyPressed->code] = true;
						ActionBuffer.push_back({ Binding->ActionID, Binding->PlayerIndex, ActionState::Pressed, 1.0f });
					}
					break; // CONSUMED! Stop looking at lower contexts.
				}
			}
		}
		// 2. Check for a Key Release
		else if (const auto* KeyReleased = event.getIf<sf::Event::KeyReleased>())
		{
			for (auto it = ContextStack.rbegin(); it != ContextStack.rend(); ++it)
			{
				if (auto Binding = (*it)->GetBinding(KeyReleased->code))
				{
					KeyStates[KeyReleased->code] = false;
					ActionBuffer.push_back({ Binding->ActionID, Binding->PlayerIndex, ActionState::Released, 0.0f });
					break; // CONSUMED!
				}
			}
		}
	}

	void InputManager::UpdateContinousStates()
	{
		// Temporary map to ensure a key isn't processed twice in the same frame by different contexts
		std::unordered_map<sf::Keyboard::Key, bool> ProcessedKeys;

		for (auto it = ContextStack.rbegin(); it != ContextStack.rend(); ++it)
		{
			for (const auto& [PhysicalKey, Binding] : (*it)->GetBindings())
			{
				// If a higher context already ate this key, skip it
				if (ProcessedKeys[PhysicalKey]) continue;

				if (sf::Keyboard::isKeyPressed(PhysicalKey))
				{
					if (KeyStates[PhysicalKey])
					{
						ActionBuffer.push_back({ Binding.ActionID, Binding.PlayerIndex, ActionState::Held, 1.0f });
					}
					ProcessedKeys[PhysicalKey] = true; // Mark as eaten
				}
			}
		}
	}

	void InputManager::ClearFrame()
	{
		ActionBuffer.clear();
	}
}