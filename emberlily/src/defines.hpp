#pragma once

// Detect platform
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define EM_PLATFORM_WINDOWS
    #ifndef _WIN64
        #error "64-bit required on Windows!"
    #endif
#elif defined(__linux__) || defined(__gnu_linux__)
    #define EM_PLATFORM_LINUX
#else
    #error "Platform not supported :("
#endif

#ifdef EM_EXPORT
    // Export lib api
    #ifdef _MSC_VER
        #define EM_API __declspec(dllexport)
    #else
        #define EM_API __attribute__((visibility("default")))
    #endif
#else
    // Import lib api
    #ifdef _MSC_API
        #define EM_API __declspec(dllimport)
    #else
        #define EM_API
    #endif
#endif
