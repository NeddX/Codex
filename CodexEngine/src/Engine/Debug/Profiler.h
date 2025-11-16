#ifndef CODEX_DEBUG_PROFILER_H
#define CODEX_DEBUG_PROFILER_H

#define CX_DEBUG_PROFILE_SCOPE(scope_name)                                                                             \
    const auto __profile_scope = codex::dbg::TimeScope(codex::dbg::ProfileInfo{                                        \
        .name = scope_name,                                                                                            \
        .file = __FILE__,                                                                                              \
        .func = CX_PRETTY_FUNCTION,                                                                                    \
        .line = __LINE__,                                                                                              \
    });

#include <sdafx.h>

namespace codex::dbg {
    // Forward declarations.
    class TimeScope;

    struct ProfileInfo
    {
        std::string      name;
        std::string_view file;
        std::string_view func;
        u32              line;
    };

    class CODEX_API Profiler
    {
    private:
        static Profiler*                                  s_Instance;
        static std::unordered_map<std::string, TimeScope> s_ScopedProfilers;

    public:
        [[nodiscard]] static TimeScope&& GetProfile(const std::string& name) noexcept;
        [[nodiscard]] static const std::unordered_map<std::string, TimeScope>& GetProfilers() noexcept;
        static void AddProfile(TimeScope&& timeScope) noexcept;
    };
} // namespace codex::dbg

#endif // CODEX_DEBUG_PROFILER_H
