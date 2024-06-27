#ifndef CODEX_CORE_GEOMETRY_DEFS_H
#define CODEX_CORE_GEOMETRY_DEFS_H

#include <sdafx.h>

namespace codex::math {
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
} // namespace codex::math

using namespace codex::math;

namespace nlohmann {
    template <>
    struct adl_serializer<codex::math::Vector2f>
    {
        static void to_json(ordered_json& j, const codex::math::Vector2f& vec)
        {
            j = ordered_json{ { "x", vec.x }, { "y", vec.y } };
        }
        static void from_json(const ordered_json& j, codex::math::Vector2f& vec)
        {
            j.at("x").get_to(vec.x);
            j.at("y").get_to(vec.y);
        }
    };

    template <>
    struct adl_serializer<codex::math::Vector2>
    {
        static void to_json(ordered_json& j, const codex::math::Vector2& vec)
        {
            j = ordered_json{ { "x", vec.x }, { "y", vec.y } };
        }
        static void from_json(const ordered_json& j, codex::math::Vector2& vec)
        {
            j.at("x").get_to(vec.x);
            j.at("y").get_to(vec.y);
        }
    };

    template <>
    struct adl_serializer<codex::math::Vector3f>
    {
        static void to_json(ordered_json& j, const codex::math::Vector3f& vec)
        {
            j = ordered_json{ { "x", vec.x }, { "y", vec.y }, { "z", vec.z } };
        }
        static void from_json(const ordered_json& j, codex::math::Vector3f& vec)
        {
            j.at("x").get_to(vec.x);
            j.at("y").get_to(vec.y);
            j.at("z").get_to(vec.z);
        }
    };

    template <>
    struct adl_serializer<codex::math::Vector3>
    {
        static void to_json(ordered_json& j, const codex::math::Vector3& vec)
        {
            j = ordered_json{ { "x", vec.x }, { "y", vec.y }, { "z", vec.z } };
        }
        static void from_json(const ordered_json& j, codex::math::Vector3& vec)
        {
            j.at("x").get_to(vec.x);
            j.at("y").get_to(vec.y);
            j.at("z").get_to(vec.z);
        }
    };

    template <>
    struct adl_serializer<codex::math::Vector4f>
    {
        static void to_json(ordered_json& j, const codex::math::Vector4f& vec)
        {
            j = ordered_json{ { "x", vec.x }, { "y", vec.y }, { "z", vec.z }, { "w", vec.w } };
        }
        static void from_json(const ordered_json& j, codex::math::Vector4f& vec)
        {
            j.at("x").get_to(vec.x);
            j.at("y").get_to(vec.y);
            j.at("z").get_to(vec.z);
            j.at("w").get_to(vec.w);
        }
    };

    template <>
    struct adl_serializer<codex::math::Vector4>
    {
        static void to_json(ordered_json& j, const codex::math::Vector4& vec)
        {
            j = ordered_json{ { "x", vec.x }, { "y", vec.y }, { "z", vec.z }, { "w", vec.w } };
        }
        static void from_json(const ordered_json& j, codex::math::Vector4& vec)
        {
            j.at("x").get_to(vec.x);
            j.at("y").get_to(vec.y);
            j.at("z").get_to(vec.z);
            j.at("w").get_to(vec.w);
        }
    };

    template <>
    struct adl_serializer<codex::math::Rectf>
    {
        static void to_json(ordered_json& j, const codex::math::Rectf& rect)
        {
            j = ordered_json{ { "x", rect.x }, { "y", rect.y }, { "w", rect.w }, { "h", rect.h } };
        }
        static void from_json(const ordered_json& j, codex::math::Rectf& rect)
        {
            j.at("x").get_to(rect.x);
            j.at("y").get_to(rect.y);
            j.at("w").get_to(rect.w);
            j.at("h").get_to(rect.h);
        }
    };

    template <>
    struct adl_serializer<codex::math::Rect>
    {
        static void to_json(ordered_json& j, const codex::math::Rect& rect)
        {
            j = ordered_json{ { "x", rect.x }, { "y", rect.y }, { "w", rect.w }, { "h", rect.h } };
        }
        static void from_json(const ordered_json& j, codex::math::Rect& rect)
        {
            j.at("x").get_to(rect.x);
            j.at("y").get_to(rect.y);
            j.at("w").get_to(rect.w);
            j.at("h").get_to(rect.h);
        }
    };
} // namespace nlohmann

namespace std {
    template <>
    struct hash<codex::math::Vector2f>
    {
        [[nodiscard]] std::size_t operator()(const codex::math::Vector2f& vec) const noexcept
        {
            return hash<codex::f32>()(vec.x) ^ hash<codex::f32>()(vec.y);
        }
    };

    template <>
    struct hash<codex::math::Vector3f>
    {
        [[nodiscard]] std::size_t operator()(const codex::math::Vector3f& vec) const noexcept
        {
            return hash<codex::f32>()(vec.x) ^ hash<codex::f32>()(vec.y) ^ hash<codex::f32>()(vec.z);
        }
    };

    template <>
    struct hash<codex::math::Vector4f>
    {
        [[nodiscard]] std::size_t operator()(const codex::math::Vector4f& vec) const noexcept
        {
            return hash<codex::f32>()(vec.x) ^ hash<codex::f32>()(vec.y) ^ hash<codex::f32>()(vec.z) ^
                   hash<codex::f32>()(vec.w);
        }
    };

    template <>
    struct hash<codex::math::Rectf>
    {
        [[nodiscard]] std::size_t operator()(const codex::math::Rectf& rect) const noexcept
        {
            return hash<codex::f32>()(rect.x) ^ hash<codex::f32>()(rect.y) ^ hash<codex::f32>()(rect.w) ^
                   hash<codex::f32>()(rect.h);
        }
    };
} // namespace std

namespace fmt {
    template <>
    struct formatter<codex::math::Vector2f> : formatter<std::string_view>
    {
        auto format(const codex::math::Vector2f& vec, format_context& ctx) const
        {
            return format_to(ctx.out(), "({}, {})", vec.x, vec.y);
        }
    };
    template <>
    struct formatter<codex::math::Vector2> : formatter<std::string_view>
    {
        auto format(const codex::math::Vector2& vec, format_context& ctx) const
        {
            return format_to(ctx.out(), "({}, {})", vec.x, vec.y);
        }
    };
    template <>
    struct formatter<codex::math::Vector3f> : formatter<std::string_view>
    {
        auto format(const codex::math::Vector3f& vec, format_context& ctx) const
        {
            return format_to(ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z);
        }
    };
    template <>
    struct formatter<codex::math::Vector3> : formatter<std::string_view>
    {
        auto format(const codex::math::Vector3& vec, format_context& ctx) const
        {
            return format_to(ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z);
        }
    };
    template <>
    struct formatter<codex::math::Vector4f> : formatter<std::string_view>
    {
        auto format(const codex::math::Vector4f& vec, format_context& ctx) const
        {
            return format_to(ctx.out(), "({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
        }
    };
    template <>
    struct formatter<codex::math::Vector4> : formatter<std::string_view>
    {
        auto format(const codex::math::Vector4& vec, format_context& ctx) const
        {
            return format_to(ctx.out(), "({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
        }
    };
    template <>
    struct formatter<codex::math::Rectf> : formatter<std::string_view>
    {
        auto format(const codex::math::Rectf& rect, format_context& ctx) const
        {
            return format_to(ctx.out(), "({}, {}, {}, {})", rect.x, rect.y, rect.w, rect.h);
        }
    };
    template <>
    struct formatter<codex::math::Rect> : formatter<std::string_view>
    {
        auto format(const codex::math::Rect& rect, format_context& ctx) const
        {
            return format_to(ctx.out(), "({}, {}, {}, {})", rect.x, rect.y, rect.w, rect.h);
        }
    };
} // namespace fmt

#endif // CODEX_CORE_GEOMETRY_DEFS_H
