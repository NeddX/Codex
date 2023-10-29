#include "Scene.h"
#include "../Renderer/Renderer.h"
#include "Components.h"
#include "ECS.h"

namespace codex {
    Scene::Scene(const i32 width, const i32 height) : m_Width(width), m_Height(height)
    {
        m_Running = false;
        m_Camera  = std::make_unique<Camera>(width, height);
        // m_ActiveEntity = nullptr;
    }

    Entity Scene::CreateEntity(const std::string_view defaultTag)
    {
        Entity entity(m_Registry.create(), this);
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<TagComponent>(defaultTag);
        return entity;
    }

    void Scene::RemoveEntity(const Entity entity)
    {
        m_Registry.destroy(entity.m_Handle);
    }

    void Scene::RemoveEntity(const u32 entity)
    {
        m_Registry.destroy((entt::entity)entity);
    }

    // NOTE: Potentially non-standard!
    template <typename T>
    std::vector<Entity> Scene::GetAllEntitiesWithComponent()
    {
        auto                view = m_Registry.view<T>();
        std::vector<Entity> entities;
        entities.reserve(view.size_hint());
        for (auto& e : view)
            entities.emplace_back(e, this);
        return entities;
    }

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
        entities.reserve(view.size_hint());
        for (auto& e : view)
        {
            Entity entity = { e, this };
            if (entity.GetComponent<TagComponent>().tag == tag)
                entities.push_back(entity);
        }
        return entities;
    }

    std::vector<Entity> Scene::GetAllEntities()
    {
        std::vector<Entity> entities;
        entities.reserve(m_Registry.size());
        // m_Registry.each([&entities](auto& entity) { entities.push_back(entity); });
        return entities;
    }

    void Scene::Start()
    {
        /*for (const auto& ent : m_Entities)
            ent->Start();*/
        m_Running = true;
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
            BatchRenderer2D::RenderSprite(renderer_component.GetSprite(),
                                          { transform_component.position.x, transform_component.position.y,
                                            transform_component.scale.x, transform_component.scale.y },
                                          -1);
        }

        for (auto& entity : GetAllEntitiesWithComponent<NativeBehaviourComponent>())
        {
            // TODO: This should happen OnScenePlay.
            auto& behaviour_component = entity.GetComponent<NativeBehaviourComponent>();
            if (!behaviour_component.instance)
            {
                behaviour_component.instance          = behaviour_component.Instantiate();
                behaviour_component.instance->m_Owner = entity;
                behaviour_component.instance->Init();
            }
            behaviour_component.instance->Update(deltaTime);
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
} // namespace codex
