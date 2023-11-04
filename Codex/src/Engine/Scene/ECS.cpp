#include "ECS.h"
#include "Components.h"

namespace codex {
    void to_json(nlohmann::ordered_json& j, const Entity& entity)
    {
        // We are not modifying anything anyways.
        auto& e = (Entity&)entity;

        nlohmann::ordered_json jentity;
        jentity["Components"] = nlohmann::ordered_json::array();

        if (e.HasComponent<TagComponent>())
        {
            auto&                  c = e.GetComponent<TagComponent>();
            nlohmann::ordered_json jc;
            jc["TagComponent"]["tag"] = c.tag;
            jentity["Components"].push_back(jc);
        }
        if (e.HasComponent<TransformComponent>())
        {
            auto&                  c = e.GetComponent<TransformComponent>();
            nlohmann::ordered_json jc;
            jc["TransformComponent"] = { { "position", c.position }, { "rotation", c.rotation }, { "scale", c.scale } };
            jentity["Components"].push_back(jc);
        }
        if (e.HasComponent<SpriteRendererComponent>())
        {
            auto&                  c = e.GetComponent<SpriteRendererComponent>();
            nlohmann::ordered_json jc;
            jc["SpriteRendererComponent"]["m_Sprite"] = c.GetSprite();
            jentity["Components"].push_back(jc);
        }
        j = jentity;
    }

    void from_json(const nlohmann::ordered_json& j, Entity& entity)
    {
        auto is_component = [&](auto& j, const char* component) -> nlohmann::ordered_json
        {
            if (j.is_object() && j.count(component))
                return j.at(component);
            return nlohmann::ordered_json();
        };

        const auto& components = j.at("Components");
        for (const auto& c : components)
        {
            if (auto cj = is_component(c, "TagComponent"); !cj.empty())
            {
                auto& nc = entity.GetComponent<TagComponent>();
                cj.at("tag").get_to(nc.tag);
            }
            else if (auto cj = is_component(c, "TransformComponent"); !cj.empty())
            {
                auto& nc = entity.GetComponent<TransformComponent>();
                cj.at("position").get_to(nc.position);
                cj.at("rotation").get_to(nc.rotation);
                cj.at("scale").get_to(nc.scale);
            }
            else if (auto cj = is_component(c, "SpriteRendererComponent"); !cj.empty())
            {
                Sprite s;
                cj.at("m_Sprite").get_to(s);
                entity.AddComponent<SpriteRendererComponent>(s);
            }
        }
    }

} // namespace codex