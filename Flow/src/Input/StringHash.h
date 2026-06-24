#pragma once
#include <cstdint>

namespace Flow
{
	// FNV-1a 32-bit hash algorithm
	// The 'constexpr' keyword forced the C++ compiler to do this math during compilation
	// This costs 0 seconds of performance when game is running

	constexpr uint32_t HashString(const char* StringToHash, uint32_t Value = 2166136261u)
	{
		return *StringToHash ? HashString(StringToHash + 1, (Value ^ static_cast<uint32_t>(*StringToHash)) * 16777619u) : Value;
	}
}