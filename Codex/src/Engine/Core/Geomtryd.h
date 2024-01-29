#ifndef CODEX_CORE_GEOMETRY_DEFS_H
#define CODEX_CORE_GEOMETRY_DEFS_H

#include <sdafx.h>

namespace codex {
    using Matrix4f = glm::mat4;
    using Matrix2f = glm::mat2;
    using Vector2f = glm::vec2;
    using Vector3f = glm::vec3;
    using Vector4f = glm::vec4;
    using Vector2  = glm::ivec2;
    using Vector3  = glm::ivec3;
    using Vector4  = glm::ivec4;
    using Rect     = mgl::Rect;
    using Rectf    = mgl::Rectf;
} // namespace codex

namespace nlohmann {
    void to_json(nlohmann::ordered_json& j, const codex::Vector2f& vec);
    void to_json(nlohmann::ordered_json& j, const codex::Vector3f& vec);
    void to_json(nlohmann::ordered_json& j, const codex::Vector4f& vec);
    void to_json(nlohmann::ordered_json& j, const codex::Rect& rect);
    void to_json(nlohmann::ordered_json& j, const codex::Rectf& rect);

    void from_json(const nlohmann::ordered_json& j, codex::Vector2f& vec);
    void from_json(const nlohmann::ordered_json& j, codex::Vector3f& vec);
    void from_json(const nlohmann::ordered_json& j, codex::Vector4f& vec);
    void from_json(const nlohmann::ordered_json& j, codex::Rect& rect);
    void from_json(const nlohmann::ordered_json& j, codex::Rectf& rect);
} // namespace nlohmann

namespace std {
    template <>
    struct hash<codex::Vector2f>
    {
        codex::usize operator()(const codex::Vector2f& vec) const
        {
            return hash<codex::f32>()(vec.x) ^ hash<codex::f32>()(vec.y);
        }
    };

    template <>
    struct hash<codex::Vector3f>
    {
        codex::usize operator()(const codex::Vector3f& vec) const
        {
            return hash<codex::f32>()(vec.x) ^ hash<codex::f32>()(vec.y) ^ hash<codex::f32>()(vec.z);
        }
    };

    template <>
    struct hash<codex::Vector4f>
    {
        codex::usize operator()(const codex::Vector4f& vec) const
        {
            return hash<codex::f32>()(vec.x) ^ hash<codex::f32>()(vec.y) ^ hash<codex::f32>()(vec.z) ^
                   hash<codex::f32>()(vec.w);
        }
    };

    template <>
    struct hash<codex::Rectf>
    {
        codex::usize operator()(const codex::Rectf& rect) const
        {
            return hash<codex::f32>()(rect.x) ^ hash<codex::f32>()(rect.y) ^ hash<codex::f32>()(rect.w) ^
                   hash<codex::f32>()(rect.h);
        }
    };
} // namespace std

namespace fmt {
    template <>
    struct formatter<codex::Vector2f>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const codex::Vector2f& vec, FormatContext& ctx)
        {
            return fmt::format_to(ctx.out(), "({}, {})", vec.x, vec.y);
        }
    };

    template <>
    struct formatter<codex::Vector2>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const codex::Vector2& vec, FormatContext& ctx)
        {
            return fmt::format_to(ctx.out(), "({}, {})", vec.x, vec.y);
        }
    };

    template <>
    struct formatter<codex::Vector3f>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const codex::Vector3f& vec, FormatContext& ctx)
        {
            return fmt::format_to(ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z);
        }
    };

    template <>
    struct formatter<codex::Vector3>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const codex::Vector3& vec, FormatContext& ctx)
        {
            return fmt::format_to(ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z);
        }
    };

    template <>
    struct formatter<codex::Vector4f>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const codex::Vector4f& vec, FormatContext& ctx)
        {
            return fmt::format_to(ctx.out(), "({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
        }
    };

    template <>
    struct formatter<codex::Vector4>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const codex::Vector4& vec, FormatContext& ctx)
        {
            return fmt::format_to(ctx.out(), "({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
        }
    };

    template <>
    struct formatter<codex::Rectf>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const codex::Rectf& vec, FormatContext& ctx)
        {
            return fmt::format_to(ctx.out(), "({}, {}, {}, {})", vec.x, vec.y, vec.w, vec.h);
        }
    };
} // namespace fmt

#endif // CODEX_CORE_GEOMETRY_DEFS_H
