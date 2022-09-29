#pragma once

#include "Logger.hpp"

#ifdef EM_ENABLE_ASSERTS
    #ifdef _MSC_VER
        #include <intrin.h>
        #define debugBreak() __debugbreak()
    #else
        #define debugBreak() __builtin_trap()
    #endif

    #define EM_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); debugBreak(); } }
    #define EM_CORE_ASSERT(x, ...) { if(!(x)) { EM_LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); debugBreak(); } }
#else 
    #define ASSERT(x, ...)
    #define EM_ASSERT(x, ...)
#endif