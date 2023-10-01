#ifndef CODEX_CORE_COMMON_DEFINITIONS_H
#define CODEX_CORE_COMMON_DEFINITIONS_H

#include <cstdint>

#ifdef __GNUC__
#define CX_DEBUG_TRAP() __builtin_trap()
#elif _MSC_VER
#define CX_DEBUG_TRAP() __debugbreak()
#elif __clang__
#define CX_DEBUG_TRAP() __builtin_debugtrap()
#endif

//#define PI 3.14159265358979323f // The ratio between circle's diameter and the circumference. This is used to convert from degrees to radians.
//#define CX_TO_RADF(d) (f32)((f32)(d) * (PI / 180.0f)) // Convert degrees to radians.

namespace Codex {
    using usize = std::size_t;
    using i32ptr = std::intptr_t;
    using ui32ptr = std::uintptr_t;
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    using f32 = float;
    using f64 = double;
    using f128 = long double;

    namespace math {
        constexpr f32 PI = 3.14159265358979323f;

        inline constexpr f32 ToRadf(const f32 degree)
        {
            return degree * (PI / 180.0f);
        }
    } // namespace math
} // namespace Codex

    /*
	constexpr f32 PI = 3.14159265358979323f;

	constexpr f32 ToRadf(f32 degree) {
		return (f32)(degree * (PI / 180.0f));
	}*/

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

#endif // CODEX_CORE_COMMON_DEFINITIONS_H
