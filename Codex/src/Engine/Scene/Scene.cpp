#include "Scene.h"
#include "../Renderer/Renderer.h"
#include "Components.h"
#include "ECS.h"

namespace codex {
    using namespace codex::graphics;

    Entity Scene::CreateEntity(const std::string_view defaultTag)
    {
        Entity entity(m_Registry.create(), this);
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<TagComponent>(defaultTag);
        return entity;
    }

    void Scene::RemoveEntity(const Entity entity)
    {
        CX_ASSERT(m_Registry.valid(entity.m_Handle), "Entity does not exists in registry.");
        m_Registry.destroy(entity.m_Handle);
    }

    void Scene::RemoveEntity(const u32 entity)
    {
        m_Registry.destroy((entt::entity)entity);
    }

    // NOTE: Okay so if T is copyable then view<T> will provide the
    // size() method, otherwise the size_hint() method.
    template <typename T>
        requires(std::is_copy_constructible_v<T>)
    static std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene)
    {
        auto                view = scene.m_Registry.view<T>();
        std::vector<Entity> entities;
        entities.reserve(view.size());
        for (auto& e : view)
            entities.emplace_back(e, &scene);
        return entities;
    }

    template <typename T>
        requires(!std::is_copy_constructible_v<T>)
    static std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene)
    {
        auto                view = scene.m_Registry.view<T>();
        std::vector<Entity> entities;
        entities.reserve(view.size_hint());
        for (auto& e : view)
            entities.emplace_back(e, &scene);
        return entities;
    }

    template <typename T>
    std::vector<Entity> Scene::GetAllEntitiesWithComponent()
    {
        return SceneGetAllEntitiesWithComponent<T>(*this);
    }

    // NOTE: Don't forget to update this when new components get added, LOL.
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<TagComponent>();
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<TransformComponent>();
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<SpriteRendererComponent>();
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<GridRendererComponent>();
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<NativeBehaviourComponent>();
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<TilemapComponent>();

    std::vector<Entity> Scene::GetAllEntitesWithTag(const std::string_view tag)
    {
        auto                view = m_Registry.view<TagComponent>();
        std::vector<Entity> entities;
        entities.reserve(view.size());
        for (auto& e : view)
        {
            Entity entity{ e, this };
            if (entity.GetComponent<TagComponent>().tag == tag)
                entities.push_back(entity);
        }
        return entities;
    }

    std::vector<Entity> Scene::GetAllEntities()
    {
        std::vector<Entity> entities;
        entities.reserve(GetEntityCount());
        auto entities_view = m_Registry.view<entt::entity>();
        for (const auto& e : entities_view)
        {
            const auto entity = Entity(e, this);
            if (!entity)
                break;
            entities.push_back(entity);
        }
        return entities;
    }

    void Scene::Start()
    {
        /*for (const auto& ent : m_Entities)
            ent->Start();*/
    }

    void Scene::Update(const f32 deltaTime)
    {
        // Render
        for (auto& entity : GetAllEntitiesWithComponent<GridRendererComponent>())
        {
            auto& grid_renderer = entity.GetComponent<GridRendererComponent>();
            grid_renderer.Render();
        }
        for (auto& entity : GetAllEntitiesWithComponent<SpriteRendererComponent>())
        {
            auto& transform_component = entity.GetComponent<TransformComponent>();
            auto& renderer_component  = entity.GetComponent<SpriteRendererComponent>();
            if (auto& s = renderer_component.GetSprite(); s)
            {
                auto transform = transform_component.GetTransform();
                auto size      = s.GetSize();
                transform      = transform * glm::scale(glm::identity<Matrix4f>(), { size.x, size.y, 1.0f });
                BatchRenderer2D::RenderSprite(renderer_component.GetSprite(), transform, entity.GetId());
            }
        }

        for (auto& entity : GetAllEntitiesWithComponent<NativeBehaviourComponent>())
        {
             // TODO: This should happen OnScenePlay.
             auto& behaviour_component = entity.GetComponent<NativeBehaviourComponent>();
             for (auto& [k, v] : behaviour_component.behaviours)
                 v->Update(deltaTime);
        }

        /*
        for (auto& entity : GetAllEntitiesWithComponent<TilemapComponent>())
        {
            // auto& transform_component = entity.GetComponent<TransformComponent>();
            auto& tilemap_component = entity.GetComponent<TilemapComponent>();
            auto& grid_component    = entity.GetComponent<GridRendererComponent>();
            if (entity.GetId() == m_SelectedEntityId)
                grid_component.Render();
            for (const auto& [key, value] : tilemap_component.GetAllTiles())
            {
                for (const auto& t : value)
                {
                    auto src_rect  = Rect{ (i32)t.second.x, (i32)t.second.y, (i32)tilemap_component.GetTileSize().x,
                                          (i32)tilemap_component.GetTileSize().y };
                    auto dest_rect = Rect{ (i32)t.first.x, (i32)t.first.y, (i32)tilemap_component.GetGridSize().x,
                                           (i32)tilemap_component.GetGridSize().y };

                    m_SpriteBatch->RenderRect(tilemap_component.GetTexture().get(), src_rect, dest_rect,
                                              Vector4f{ 1.0f, 1.0f, 1.0f, 1.0f }, key, (i32)entity.GetId());
                }
            }
        }
        */
    }

    void to_json(nlohmann::ordered_json& j, const Scene& scene)
    {
        std::vector<Entity> entities;
        const auto entities_view = scene.m_Registry.view<entt::entity>();
        entities.reserve(entities_view.size_hint());
        for (const auto& e : entities)
            entities.emplace_back(e, (Scene*)&scene); 

        j["Name"]     = scene.m_Name;
        j["Entities"] = entities;
    }
} // namespace codex
