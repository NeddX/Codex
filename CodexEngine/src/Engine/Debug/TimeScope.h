#ifndef CODEX_DEBUG_TIMESCOPE_H
#define CODEX_DEBUG_TIMESCOPE_H

#include "Profiler.h"

namespace codex::dbg {
    template <typename T>
    concept IsChronoPeriod = requires { typename T::period; };

    template <typename T>
    concept IsChronoClock = requires {
        { T::now() } -> std::convertible_to<std::chrono::time_point<T>>;
    };

    class TimeScope
    {
        friend class Profiler;

    public:
        using Ratio       = std::nano;
        using Rep         = f64;
        using ChronoClock = std::chrono::steady_clock;

    private:
        ProfileInfo                       m_Info;
        bool                              m_Initiated = false;
        ChronoClock::time_point           m_InitialTp;
        std::chrono::duration<Rep, Ratio> m_Duration;

    public:
        inline TimeScope() noexcept = default;
        inline TimeScope(ProfileInfo info) noexcept
            : m_Info(std::move(info))
            , m_Initiated(true)
            , m_InitialTp(ChronoClock::now())
        {
        }
        inline TimeScope(TimeScope&& other) noexcept            = default;
        inline TimeScope& operator=(TimeScope&& other) noexcept = default;
        inline ~TimeScope() noexcept
        {
            if (m_Initiated)
            {
                m_Initiated = false;
                m_Duration  = std::chrono::duration<Rep, Ratio>(ChronoClock::now() - m_InitialTp);
                Profiler::AddProfile(std::move(*this));
            }
        }

    public:
        [[nodiscard]] ProfileInfo GetInfo() const noexcept { return m_Info; }
        template <typename ToRatio, typename ToRep>
        [[nodiscard]] auto ElapsedAs() const noexcept
        {
            return std::chrono::duration_cast<std::chrono::duration<ToRep, ToRatio>>(m_Duration);
        }
        [[nodiscard]] auto Elapsed() const noexcept { return m_Duration; }
    };
} // namespace codex::dbg

#endif // CODEX_DEBUG_TIMESCOPE_H
