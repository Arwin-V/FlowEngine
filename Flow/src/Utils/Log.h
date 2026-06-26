#pragma once
#include <iostream>
#include <cstring>

namespace Flow
{
    // ANSI Colors
    constexpr const char* COLOR_RESET = "\033[0m";
    constexpr const char* COLOR_GREEN = "\033[32m";
    constexpr const char* COLOR_YELLOW = "\033[33m";
    constexpr const char* COLOR_RED = "\033[31m";

    // Helper to extract just the filename from the full path
    inline const char* GetShortFilename(const char* fullPath) {
        const char* slash = strrchr(fullPath, '\\');
        if (!slash) slash = strrchr(fullPath, '/');
        return slash ? slash + 1 : fullPath;
    }
}

// ========================================================================
// UPDATED CORE LOGGING MACROS
// ========================================================================

#ifndef NDEBUG
    // We use ##__VA_ARGS__ to allow the optional variables to be passed in
#define FLOW_LOG_INFO(Message, ...) \
        std::cout << Flow::COLOR_GREEN << "Flow [INFO]  [" << Flow::GetShortFilename(__FILE__) << ":" << __LINE__ << "] " \
        << Message << Flow::COLOR_RESET << "\n", ##__VA_ARGS__

#define FLOW_LOG_WARN(Message, ...) \
        std::cout << Flow::COLOR_YELLOW << "Flow [WARN]  [" << Flow::GetShortFilename(__FILE__) << ":" << __LINE__ << "] " \
        << Message << Flow::COLOR_RESET << "\n", ##__VA_ARGS__

#define FLOW_LOG_ERROR(Message, ...) \
        std::cerr << Flow::COLOR_RED << "Flow [ERROR] [" << Flow::GetShortFilename(__FILE__) << ":" << __LINE__ << "] " \
        << Message << Flow::COLOR_RESET << "\n", ##__VA_ARGS__
#else
#define FLOW_LOG_INFO(Message, ...)
#define FLOW_LOG_WARN(Message, ...)
#define FLOW_LOG_ERROR(Message, ...)
#endif