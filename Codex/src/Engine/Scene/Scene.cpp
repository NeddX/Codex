#include "Scene.h"
#include "../Graphics/Renderer.h"
#include "../NativeBehaviour/NativeBehaviour.h"
#include "../Reflection/Reflector.h"
#include "Components.h"
#include "ECS.h"

namespace codex {
    static std::unordered_map<Entity::HandleType, std::string> g_PossibleAttachedScripts;

    Scene::~Scene() noexcept
    {
        // Okay so attached scripts need to be detached and delete-ed before NBMan is unloaded.
        m_Registry.~basic_registry();
        m_ScriptModule.Reset();
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
        CX_ASSERT(m_Registry.valid(entity.m_Handle), "Entity does not exists in registry.");
        m_Registry.destroy(entity.m_Handle);
    }

    void Scene::RemoveEntity(const u32 entity)
    {
        m_Registry.destroy((entt::entity)entity);
    }

    // NOTE: Okay so if T is movable then view<T> will provide the
    // size() method, otherwise the size_hint() method.
    template <typename T>
        requires(std::is_move_constructible_v<T>)
    std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene)
    {
        auto                view = scene.m_Registry.view<T>();
        std::vector<Entity> entities;
        entities.reserve(view.size());
        for (auto& e : view)
            entities.emplace_back(e, &scene);
        return entities;
    }

    template <typename T>
        requires(!std::is_move_constructible_v<T>)
    std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene)
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
        for (auto entities_view = m_Registry.view<entt::entity>(); const auto& e : entities_view)
        {
            const auto entity = Entity(e, this);
            if (!entity)
                break;
            entities.push_back(entity);
        }
        return entities;
    }

    void Scene::LoadScriptModule(std::filesystem::path modulePath)
    {
        if (m_ScriptModule)
            UnloadScriptModule();
        m_ScriptModule = mem::Box<sys::DLib>::New(std::move(modulePath));

        if (!g_PossibleAttachedScripts.empty())
        {
            for (auto& [handle, name] : g_PossibleAttachedScripts)
            {
                Entity entity{ handle, this };
                if (entity.HasComponent<NativeBehaviourComponent>())
                {
                    auto& nbc = entity.GetComponent<NativeBehaviourComponent>();
                    if (BehaviourExists(name.c_str()))
                        nbc.Attach(CreateBehaviour(name.c_str()));
                }
            }
            g_PossibleAttachedScripts.clear();
        }

        fmt::println("Script module loaded."); // TODO: Remove!
    }

    void Scene::UnloadScriptModule()
    {
        // When unloading we need to check for possible attached scripts, if any script is attached then record its name
        // so that we can attached them back (that is if they still exist) when the module is loaded back.
        auto nbc_view = m_Registry.view<NativeBehaviourComponent>();
        for (auto& entt : nbc_view)
        {
            Entity entity{ entt, this };
            auto&  nbc        = entity.GetComponent<NativeBehaviourComponent>();
            auto&  behaviours = nbc.GetBehaviours();
            auto   it         = behaviours.begin();
            std::list<std::string_view> to_be_detached;

            for (auto& [name, _] : behaviours)
            {
                g_PossibleAttachedScripts[(Entity::HandleType)entity] = name;
                to_be_detached.emplace_back(name);
            }

            for (const auto& e : to_be_detached)
                nbc.Detach(e);
        }

        m_ScriptModule.Reset();
        fmt::println("Script module unloaded."); // TODO: Remove!
    }

    NativeBehaviour* Scene::CreateBehaviour(const char* className) const noexcept
    {
        return RF_INSTANCE_CREATE(m_ScriptModule, className);
    }

    bool Scene::BehaviourExists(const char* className) const noexcept
    {
        return RF_INSTANCE_CHECK(m_ScriptModule, className);
    }

    void Scene::OnEditorInit(scene::EditorCamera& camera)
    {
    }

    void Scene::OnRuntimeInit()
    {
    }

    void Scene::OnEditorUpdate(const f32 deltaTime)
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
                gfx::BatchRenderer2D::RenderSprite(renderer_component.GetSprite(), transform, entity.GetId());
            }
        }

        for (auto& entity : GetAllEntitiesWithComponent<NativeBehaviourComponent>())
        {
            // TODO: This should happen OnScenePlay.
            auto& behaviour_component = entity.GetComponent<NativeBehaviourComponent>();
            for (auto& [k, v] : behaviour_component.GetBehaviours())
                v->OnUpdate(deltaTime);
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

    void Scene::OnRuntimeUpdate(const f32 deltaTime)
    {
    }

    void to_json(nlohmann::ordered_json& j, const Scene& scene)
    {
        const auto entities = ((Scene&)scene).GetAllEntities();
        j["Name"]           = scene.m_Name;
        j["Entities"]       = entities;
    }
} // namespace codex
