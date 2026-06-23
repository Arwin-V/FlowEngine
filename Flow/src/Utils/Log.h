#pragma once
#include <iostream>

namespace Flow
{
    // ANSI Color coded to make the terminal visually readable
    constexpr const char* COLOR_RESET = "\033[0m";
    constexpr const char* COLOR_GREEN = "\033[32m";
    constexpr const char* COLOR_YELLOW = "\033[33m";
    constexpr const char* COLOR_RED = "\033[31m";
}

// ========================================================================
// CORE LOGGING MACROS
// ========================================================================
// The #ifndef NDEBUG means "If Not Debug". 
// This automatically disables logs when you build the final game.

#ifndef NDEBUG
    // Uses standard output. Automatically appends colors and newlines.
#define FLOW_LOG_INFO(Message)  std::cout << Flow::COLOR_GREEN  << "Flow [INFO]  " << Message << Flow::COLOR_RESET << "\n"
#define FLOW_LOG_WARN(Message)  std::cout << Flow::COLOR_YELLOW << "Flow [WARN]  " << Message << Flow::COLOR_RESET << "\n"

// Uses standard error output (cerr) which skips buffers for immediate crash reporting
#define FLOW_LOG_ERROR(Message) std::cerr << Flow::COLOR_RED    << "Flow [ERROR] " << Message << Flow::COLOR_RESET << "\n"
#else
    // In Release mode, these macros turn into literal empty space. Zero performance cost.
#define FLOW_LOG_INFO(Message)
#define FLOW_LOG_WARN(Message)
#define FLOW_LOG_ERROR(Message)
#endif