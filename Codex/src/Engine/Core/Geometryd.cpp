#include "Geomtryd.h"

namespace nlohmann {
    void to_json(nlohmann::json& j, const codex::Vector2f& vec)
    {
        j = nlohmann::json{ { "x", vec.x }, { "y", vec.y } };
    }

    void to_json(nlohmann::json& j, const codex::Vector3f& vec)
    {
        j = nlohmann::json{ { "x", vec.x }, { "y", vec.y }, { "z", vec.z } };
    }

    void to_json(nlohmann::json& j, const codex::Vector4f& vec)
    {
        j = nlohmann::json{ { "x", vec.x }, { "y", vec.y }, { "z", vec.z }, { "w", vec.w } };
    }

    void to_json(nlohmann::json& j, const codex::Rect& rect)
    {
        j = nlohmann::json{ { "x", rect.x }, { "y", rect.y }, { "w", rect.h }, { "h", rect.h } };
    }

    void to_json(nlohmann::json& j, const codex::Rectf& rect)
    {
        j = nlohmann::json{ { "x", rect.x }, { "y", rect.y }, { "w", rect.h }, { "h", rect.h } };
    }

    void from_json(const nlohmann::json& j, codex::Vector2f& vec)
    {
        j.at("x").get_to(vec.x);
        j.at("y").get_to(vec.y);
    }

    void from_json(const nlohmann::json& j, codex::Vector3f& vec)
    {
        j.at("x").get_to(vec.x);
        j.at("y").get_to(vec.y);
        j.at("z").get_to(vec.z);
    }

    void from_json(const nlohmann::json& j, codex::Vector4f& vec)
    {
        j.at("x").get_to(vec.x);
        j.at("y").get_to(vec.y);
        j.at("z").get_to(vec.z);
        j.at("w").get_to(vec.w);
    }

    void from_json(const nlohmann::json& j, codex::Rect& rect)
    {
        j.at("x").get_to(rect.x);
        j.at("y").get_to(rect.y);
        j.at("w").get_to(rect.h);
        j.at("h").get_to(rect.w);
    }

    void from_json(const nlohmann::json& j, codex::Rectf& rect)
    {
        j.at("x").get_to(rect.x);
        j.at("y").get_to(rect.y);
        j.at("w").get_to(rect.h);
        j.at("h").get_to(rect.w);
    }
} // namespace nlohmann
