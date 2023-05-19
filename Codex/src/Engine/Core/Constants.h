#ifndef CODEX_CORE_CONSTANTS_H
#define CODEX_CORE_CONSTANTS_H

#ifndef CFX_EXPORTS
#define CFX_EXPORTS
#ifdef _WIN32
#define CFX_EXPORT __declspec(dllexport)
#endif
#endif		

#ifdef __GNUC__
#define CODEX_DEBUG_TRAP() __builtin_trap()
#elif _MSC_VER
#define CODEX_DEBUG_TRAP() __debugbreak()
#elif __clang__
#define CODEX_DEBUG_TRAP() __builtin_debugtrap()
#endif

#ifdef CODEX_CONF_DEBUG
    #define CODEX_ARRAY_COUNT(x) sizeof(x)/sizeof(x[0])
    #define MGL_DEBUG
    #define CODEX_ASSERT(x, msg) \
        if (!(x)) { \
            std::cerr << "[CODEX_DEBUG] :: Assertion failed: " << msg << "\n\tStack trace:"\
                      << "\n\t\tFunction: " << __FUNCTION__ \
                      << "\n\t\tFile: " << __FILE__ \
                      << "\n\t\tLine: " << __LINE__ << std::endl; \
            CODEX_DEBUG_TRAP(); \
        }
#else
    #define CODEX_ASSERT(x, msg) x
#endif


#endif // CODEX_CORE_CONSTANTS_H