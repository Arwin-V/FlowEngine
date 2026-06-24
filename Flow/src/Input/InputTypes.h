#pragma once
#include <cstdint>

namespace Flow
{
	// The exact lifecycle phase of the button this frame
	enum class ActionState
	{
		Pressed, // The exact millisecond it was pushed down
		Held,    // It is actively being held
		Released // The exact millisecond it was let go
	};
		
	// The data packet that travels across our 3 layers
	struct GameAction
	{
		uint32_t ActionID; // The hashed text
		int PlayerIndex;   // Player Number
		ActionState State; // Pressed? Released? Held?
		float AxisValue;   // -1.0f to +1.0f ( for analog stick and mouse)
	};
}