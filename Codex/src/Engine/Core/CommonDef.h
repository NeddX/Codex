#ifndef CODEX_CORE_COMMON_DEFINITIONS_H
#define CODEX_CORE_COMMON_DEFINITIONS_H

#include <cstdint>

#if defined(CX_COMPILER_MSVC)
#ifdef CX_BUILD_LIB
#define CODEX_API __declspec(dllexport)
#else
#define CODEX_API __declspec(dllimport)
#endif
#elif defined(__clang__) || defined(__GNUC__)
#define CODEX_EXPORT __attribute__((visibility("default")))
#endif

#ifdef CX_COMPILER_GNUC
#define CX_DEBUG_TRAP()    __builtin_trap()
#define CX_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif CX_COMPILER_MSVC
#define CX_DEBUG_TRAP()    __debugbreak()
#define CX_PRETTY_FUNCTION __FUNCSIG__
#elif CX_COMPILER_CLANG
#define CX_DEBUG_TRAP()    __builtin_debugtrap()
#define CX_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

namespace codex {
    using usize   = std::size_t;
    using i32ptr  = std::intptr_t;
    using ui32ptr = std::uintptr_t;
    using i8      = std::int8_t;
    using i16     = std::int16_t;
    using i32     = std::int32_t;
    using i64     = std::int64_t;
    using u8      = std::uint8_t;
    using u16     = std::uint16_t;
    using u32     = std::uint32_t;
    using u64     = std::uint64_t;
    using f32     = float;
    using f64     = double;
    using f128    = long double;

    namespace math {
        constexpr f32 PI = 3.14159265358979323f;

        inline constexpr f32 ToRadf(const f32 degree)
        {
            return degree * (PI / 180.0f);
        }
    } // namespace math

    template <typename T>
    inline constexpr T BitFlag(const T b)
    {
        return b << 1;
    }

    template <typename T, const usize n>
    inline constexpr T ArrayCount(const T (&arr)[n])
    {
        return sizeof(arr) / sizeof(arr[0]);
    }
} // namespace codex

#ifdef CX_CONFIG_DEBUG
#define MGL_DEBUG
#define CX_ASSERT(x, msg)                                                                                              \
    if (!(x))                                                                                                          \
    {                                                                                                                  \
        std::cerr << "[CODEX-DEBUG] :: Assertion failed: " << msg << "\n\tStack trace:"                                \
                  << "\n\t\tFunction: " << __FUNCTION__ << "\n\t\tFile: " << __FILE__ << "\n\t\tLine: " << __LINE__    \
                  << std::endl;                                                                                        \
        CX_DEBUG_TRAP();                                                                                               \
    }
#else
#define CX_ASSERT(x, msg) ;
#endif

#endif // CODEX_CORE_COMMON_DEFINITIONS_H
