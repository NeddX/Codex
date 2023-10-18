#include "Scene.h"
#include "ECS.h"
#include "Components.h"

namespace codex {
    Scene::Scene(Renderer* renderer, const i32 width, const i32 height) :
        m_Width(width), m_Height(height), m_Renderer(renderer)
    {
        m_Running = false;
        m_Camera = std::make_unique<Camera>(width, height);
        //m_ActiveEntity = nullptr;
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

    std::vector<Entity> Scene::GetAllEntitesWithTag(const std::string_view tag)
    {
        auto view = m_Registry.view<TagComponent>();
        std::vector<Entity> entities;
        entities.reserve(view.size_hint());
        for (auto& e : view)
        {
            Entity entity = {e, this};
            if (entity.GetComponent<TagComponent>().tag == tag)
                entities.push_back(entity);
        }
        return entities;
    }

    /*
    void Scene::AddEntity(Entity* entity)
    {
        m_Entities.push_back(entity);
        if (m_Running) entity->Start();
    }
    */

    void Scene::Init()
    {
        this->Start();
        /*for (const auto& ent : m_Entities)
            ent->Start();*/
        m_Running = true;
    }
}
