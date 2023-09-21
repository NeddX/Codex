#ifndef CODEX_CORE_CONSTANTS_H
#define CODEX_CORE_CONSTANTS_H	

#ifdef __GNUC__
#define CX_DEBUG_TRAP() __builtin_trap()
#elif _MSC_VER
#define CX_DEBUG_TRAP() __debugbreak()
#elif __clang__
#define CX_DEBUG_TRAP() __builtin_debugtrap()
#endif

#define PI 3.14159265358979323f							// The ratio between circle's diameter and the circumference. This is used to convert from degrees to radians.
#define CX_TO_RADF(d) (float)((float)(d) * (PI / 180.0f))	// Convert degrees to radians.

namespace Codex
{
    /*
	constexpr float PI = 3.14159265358979323f;

	constexpr float ToRadf(float degree) {
		return (float)(degree * (PI / 180.0f));
	}*/
}

#ifdef CODEX_CONF_DEBUG
    #define CX_ARRAY_COUNT(x) sizeof(x)/sizeof(x[0])
    #define MGL_DEBUG
    #define CX_ASSERT(x, msg) \
        if (!(x)) { \
            std::cerr << "[CODEX_DEBUG] :: Assertion failed: " << msg << "\n\tStack trace:"\
                      << "\n\t\tFunction: " << __FUNCTION__ \
                      << "\n\t\tFile: " << __FILE__ \
                      << "\n\t\tLine: " << __LINE__ << std::endl; \
            CX_DEBUG_TRAP(); \
        }
#else
    #define CX_ASSERT(x, msg) ;
#endif


#endif // CODEX_CORE_CONSTANTS_H