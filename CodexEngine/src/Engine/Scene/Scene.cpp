#include "Scene.h"

#include <Debug/Profiler.h>
#include <Debug/TimeScope.h>
#include <Engine/Graphics/Renderer.h>
#include <Engine/NativeBehaviour/NativeBehaviour.h>
#include <Engine/Reflection/Reflector.h>
#include <Engine/Utils/Box2DUtils.h>
#include <Engine/Utils/Math.h>

#include "Components.h"
#include "ECS.h"

namespace codex {
    // TODO: NBMan is shared across all scenes, Scene being the owner of NBMan does not seem correct.
    mem::Box<sys::DLib>                     Scene::s_ScriptModule = nullptr;
    std::unordered_map<Entity, std::string> Scene::s_PossibleAttachedScripts;

    using EntityMap = std::unordered_map<UUID, entt::entity>;

    template <typename... Components>
    static void CopyComponents(const entt::registry& from, entt::registry& to, const EntityMap& map) noexcept
    {
        (
            [&]()
            {
                const auto view = from.view<Components>();
                for (const auto& e : view)
                {
                    const auto  uuid      = from.get<IDComponent>(e).uuid;
                    const auto& component = from.get<Components>(e);
                    const auto  entity    = map.at(uuid);
                    to.emplace_or_replace<Components>(entity, component);
                }
            }(),
            ...);
    }

    template <typename... Components>
    static void CopyComponents(ComponentGroup<Components...>, const entt::registry& from, entt::registry& to,
                               const EntityMap& map) noexcept
    {
        CopyComponents<Components...>(from, to, map);
    }

    Scene::Scene(Scene&& other) noexcept
    {
        m_Registry     = std::move(other.m_Registry);
        m_Name         = std::move(other.m_Name);
        s_ScriptModule = std::move(other.s_ScriptModule);
    }

    Scene& Scene::operator=(Scene&& other) noexcept
    {
        Scene{ std::move(other) }.Swap(*this);
        return *this;
    }

    Scene::~Scene() noexcept
    {
        // Attached scripts need to be detached (and freed) first before NBMan is unloaded.
        // The reason why I'm not directly invoking the destructor of entt::basic_registry<> is
        // because it causes a crash on OSX.
        auto view = m_Registry->view<NativeBehaviourComponent>();
        for (auto& e : view)
            view.get<NativeBehaviourComponent>(e).DisposeBehaviours();

        m_State.store(State::Edit);
        if (m_FixedUpdateThread.joinable())
            m_FixedUpdateThread.join();
    }

    void Scene::CopyTo(Scene& other) const noexcept
    {
        {
            EntityMap entity_map;

            const auto src_reg = m_Registry.Lock();
            const auto id_view = src_reg->view<IDComponent, TagComponent>();

            for (const auto& e : id_view)
            {
                const auto& tc        = id_view.get<TagComponent>(e);
                const auto& idc       = id_view.get<IDComponent>(e);
                auto        cx_entity = other.CreateEntity(tc.tag, idc.uuid);
                entity_map[idc.uuid]  = entt::entity{ static_cast<Entity::HandleType>(cx_entity) };
            }

            auto dst_reg = other.m_Registry.Lock();
            CopyComponents(AllComponents{}, *src_reg, *dst_reg, entity_map);
        }

        other.m_Name              = m_Name;
        other.m_PhysicsProperties = m_PhysicsProperties;
    }

    Entity Scene::CreateEntity(const std::string_view defaultTag, UUID uuid) noexcept
    {
        auto entity = m_Registry->create();
        m_Registry->emplace<IDComponent>(entity, uuid);
        m_Registry->emplace<TransformComponent>(entity);
        m_Registry->emplace<TagComponent>(entity, defaultTag);
        return { entity, this };
    }

    void Scene::RemoveEntity(const Entity entity)
    {
        CX_ASSERT(m_Registry->valid(entity.m_Handle), "Entity does not exists in registry.");
        m_Registry->destroy(entity.m_Handle);
    }

    void Scene::RemoveEntity(const u32 entity)
    {
        m_Registry->destroy(static_cast<entt::entity>(entity));
    }

    // NOTE: Okay so if T is movable then view<T> will provide the
    // size() method, otherwise the size_hint() method.
    template <typename T>
        requires(std::is_move_constructible_v<T>)
    std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene) noexcept
    {
        auto                view = scene.m_Registry->view<T>();
        std::vector<Entity> entities;
        entities.reserve(view.size());
        for (auto& e : view)
            entities.emplace_back(e, &scene);
        return entities;
    }

    template <typename T>
        requires(!std::is_move_constructible_v<T>)
    std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene) noexcept
    {
        auto                view = scene.m_Registry->view<T>();
        std::vector<Entity> entities;
        entities.reserve(view.size_hint());
        for (auto& e : view)
            entities.emplace_back(e, &scene);
        return entities;
    }

    template <typename T>
    std::vector<Entity> Scene::GetAllEntitiesWithComponent() noexcept
    {
        return SceneGetAllEntitiesWithComponent<T>(*this);
    }

    // NOTE: Don't forget to update this when new components get added, lol.
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<TagComponent>() noexcept;
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<TransformComponent>() noexcept;
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<SpriteRendererComponent>() noexcept;
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<NativeBehaviourComponent>() noexcept;
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<CameraComponent>() noexcept;
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<RigidBody2DComponent>() noexcept;
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<BoxCollider2DComponent>() noexcept;
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<CircleCollider2DComponent>() noexcept;
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<GridRendererComponent>() noexcept;
    template std::vector<Entity> Scene::GetAllEntitiesWithComponent<TilemapComponent>() noexcept;

    [[nodiscard]] std::vector<Entity> Scene::GetAllEntitesWithTag(const std::string_view tag)
    {
        auto                view = m_Registry->view<TagComponent>();
        std::vector<Entity> entities;
        entities.reserve(view.size());
        for (auto& e : view)
        {
            if (view.get<TagComponent>(e).tag == tag)
                entities.emplace_back(e, this);
        }
        return entities;
    }

    [[nodiscard]] std::vector<Entity> Scene::GetAllEntities()
    {
        std::vector<Entity> entities;
        entities.reserve(GetEntityCount());
        for (auto entities_view = m_Registry->view<entt::entity>(); const auto& e : entities_view)
        {
            const auto entity = Entity(e, this);
            if (!entity)
                break;
            entities.push_back(entity);
        }
        return entities;
    }

    [[nodiscard]] Entity Scene::GetPrimaryCameraEntity() noexcept
    {
        return (m_PrimaryCameraEntity) ? *m_PrimaryCameraEntity : Entity::None();
    }

    void Scene::RenderSprites()
    {
        // Sprites
        {
            CX_DEBUG_PROFILE_SCOPE("SpriteRender")

            const auto registry = m_Registry.Lock();
            const auto view     = registry->view<TransformComponent, SpriteRendererComponent>();
            for (auto& e : view)
            {
                const auto& transform_component = view.get<TransformComponent>(e);
                const auto& renderer_component  = view.get<SpriteRendererComponent>(e);
                if (const auto& s = renderer_component.GetSprite(); s)
                {
                    const auto size = s.GetSize();
                    // The scaling we do here is the Sprite's size.

                    // TODO: Get rid of this and optimize this?
                    const auto transform = transform_component.ToMatrix() *
                                           glm::scale(glm::identity<Matrix4f>(), { size.x, size.y, 1.0f });
                    gfx::BatchRenderer2D::RenderSprite(renderer_component.GetSprite(), transform, static_cast<i32>(e));
                }
            }
        }

        // Tiles
        {
            CX_DEBUG_PROFILE_SCOPE("TileRender")

            const auto registry = m_Registry.Lock();
            const auto view     = registry->view<TilemapComponent, TransformComponent>();
            for (const auto& e : view)
            {
                auto& tilemap_component = view.get<TilemapComponent>(e);
                for (const auto& tile : tilemap_component.tiles)
                {
                    auto sprite = tilemap_component.sprite;
                    sprite.SetSize(tilemap_component.gridSize);
                    sprite.SetTextureCoords(
                        utils::ToRectf(tile.atlas, tilemap_component.tileSize.x, tilemap_component.tileSize.y));
                    sprite.SetZIndex(tile.layer);

                    auto transform = Matrix4f{ 1.0f };
                    transform      = glm::translate(transform, tile.pos);
                    transform      = glm::scale(transform, Vector3f{ sprite.GetSize().x, sprite.GetSize().y, 1.0f });
                    gfx::BatchRenderer2D::RenderSprite(sprite, transform, static_cast<i32>(e));
                }
            }
        }
    }

    void Scene::ConstructPhysicsBodies()
    {
        // Rigid body 2d construction.
        {
            auto registry = m_Registry.Lock();

            const auto rb2d_view = registry->view<TransformComponent, RigidBody2DComponent>();
            for (const auto& e : rb2d_view)
            {
                const auto& trans = rb2d_view.get<TransformComponent>(e);
                auto&       rb2d  = rb2d_view.get<RigidBody2DComponent>(e);

                b2BodyDef body_def;
                body_def.position.Set(trans.position.x * m_PhysicsProperties.scalingFactor,
                                      trans.position.y * m_PhysicsProperties.scalingFactor);
                body_def.type           = utils::ToB2Type(rb2d.bodyType);
                body_def.angle          = trans.rotation.z;
                body_def.angularDamping = rb2d.angularDamping;
                body_def.linearDamping  = rb2d.linearDamping;
                body_def.bullet         = rb2d.highVelocity;
                body_def.fixedRotation  = rb2d.fixedRotation;
                body_def.gravityScale   = rb2d.gravityScale;
                body_def.enabled        = rb2d.enabled;
                body_def.angle          = math::ToRadf(trans.rotation.z);
                auto* b2_body           = m_PhysicsWorld->CreateBody(&body_def);

                rb2d.runtimeBody = b2_body;
            }
        }

        // Box collider 2d construction.
        {
            auto registry = m_Registry.Lock();

            const auto box_collider_view =
                registry->view<TransformComponent, RigidBody2DComponent, BoxCollider2DComponent>();
            for (const auto& e : box_collider_view)
            {
                const auto& body     = box_collider_view.get<RigidBody2DComponent>(e);
                const auto& collider = box_collider_view.get<BoxCollider2DComponent>(e);
                const auto& trans    = box_collider_view.get<TransformComponent>(e);

                b2PolygonShape shape;
                shape.SetAsBox(collider.size.x * trans.scale.x * m_PhysicsProperties.scalingFactor,
                               collider.size.y * trans.scale.y * m_PhysicsProperties.scalingFactor,
                               utils::ToB2Vec2(collider.offset * m_PhysicsProperties.scalingFactor), 0.0f);

                b2FixtureDef fixture_def;
                fixture_def.shape                = &shape;
                fixture_def.density              = collider.physicsMaterial.density;
                fixture_def.friction             = collider.physicsMaterial.friction;
                fixture_def.restitution          = collider.physicsMaterial.restitution;
                fixture_def.restitutionThreshold = collider.physicsMaterial.restitutionThreshold;

                reinterpret_cast<b2Body*>(body.runtimeBody)->CreateFixture(&fixture_def);
            }
        }

        // Cirlce collider 2d.
        {
            auto registry = m_Registry.Lock();

            const auto circle_collider_view =
                registry->view<TransformComponent, RigidBody2DComponent, CircleCollider2DComponent>();
            for (const auto& e : circle_collider_view)
            {
                const auto& body     = circle_collider_view.get<RigidBody2DComponent>(e);
                const auto& collider = circle_collider_view.get<CircleCollider2DComponent>(e);

                b2CircleShape shape;
                shape.m_p.Set(collider.offset.x * m_PhysicsProperties.scalingFactor,
                              collider.offset.y * m_PhysicsProperties.scalingFactor);
                shape.m_radius = collider.radius * m_PhysicsProperties.scalingFactor;

                b2FixtureDef fixture_def;
                fixture_def.shape                = &shape;
                fixture_def.density              = collider.physicsMaterial.density;
                fixture_def.friction             = collider.physicsMaterial.friction;
                fixture_def.restitution          = collider.physicsMaterial.restitution;
                fixture_def.restitutionThreshold = collider.physicsMaterial.restitutionThreshold;

                reinterpret_cast<b2Body*>(body.runtimeBody)->CreateFixture(&fixture_def);
            }
        }
    }

    void Scene::OnEditorInit([[maybe_unused]] scene::EditorCamera& camera)
    {
    }

    void Scene::OnRuntimeStart()
    {
        m_PhysicsWorld = mem::Box<b2World>::New(utils::ToB2Vec2(m_PhysicsProperties.gravity));
        m_PhysicsWorld->SetAllowSleeping(true);

        m_State.store(State::Play);

        // Grab primary camera.
        {
            auto registry = m_Registry.Lock();

            // Grab the primary camera entity.
            {
                auto view = registry->view<TransformComponent, CameraComponent>();
                for (auto& e : view)
                {
                    auto& c = view.get<CameraComponent>(e);
                    if (c.primary)
                    {
                        m_PrimaryCameraEntity.Reset(new Entity(e, this));
                        break;
                    }
                }
            }
        }

        ConstructPhysicsBodies();

        // Native behaviour instantiation.
        {
            // Lock the registry only for one statement because user scripts can also possibly lock the registry
            // for interactions (such as calls to GetComponent<T>, HasComponent<T> etc...) instead of
            // locking for the entire scope.
            auto nbc_view = m_Registry->view<NativeBehaviourComponent>();
            for (auto& e : nbc_view)
            {
                auto& nbc = nbc_view.get<NativeBehaviourComponent>(e);
                nbc.SetParent(Entity{ e, this });

                try
                {
                    nbc.OnInit();
                }
                catch (const NativeBehaviourException& ex)
                {
                    // If NBC fails to initialise all components then most likely one of the Behaviours threw an error
                    // occured; revert state back to State::Edit and halt physics simulation.
                    OnRuntimeStop();

                    // TODO: We should stop calling lgx::Get for every log.
                    // TODO: Review this inner exception thing for later.
                    auto& exi = ex.InnerException();
                    lgx::Get("engine").Error("A Behaviour threw an Error: {}", exi.to_string());
                    return;
                }
            }
        }

        m_FixedUpdateThread = std::thread(Scene::OnFixedUpdate, std::ref(*this));
    }

    void Scene::OnSimulationStart()
    {
        m_PhysicsWorld = mem::Box<b2World>::New(utils::ToB2Vec2(m_PhysicsProperties.gravity));
        m_PhysicsWorld->SetAllowSleeping(true);

        m_State.store(State::Simulate);

        ConstructPhysicsBodies();

        // Native behaviour instantiation.
        {
            // Lock the registry only for one statement because user scripts can also possibly lock the registry
            // for interactions (such as calls to GetComponent<T>, HasComponent<T> etc...) instead of
            // locking for the entire scope.
            auto nbc_view = m_Registry->view<NativeBehaviourComponent>();
            for (auto& e : nbc_view)
            {
                auto& nbc = nbc_view.get<NativeBehaviourComponent>(e);
                nbc.SetParent(Entity{ e, this });

                try
                {
                    nbc.OnInit();
                }
                catch (const NativeBehaviourException& ex)
                {
                    // If NBC fails to initialise all components then most likely one of the Behaviours threw an error
                    // occured; revert state back to State::Edit and halt physics simulation.
                    OnRuntimeStop();

                    // TODO: We should stop calling lgx::Get for every log.
                    // TODO: Review this inner exception thing for later.
                    auto& exi = ex.InnerException();
                    lgx::Get("engine").Error("A Behaviour threw an Error: {}", exi.to_string());
                    return;
                }
            }
        }

        m_FixedUpdateThread = std::thread(Scene::OnFixedUpdate, std::ref(*this));
    }

    void Scene::OnSimulationStop()
    {
        m_State.store(State::Edit);

        if (m_FixedUpdateThread.joinable())
            m_FixedUpdateThread.join();

        m_PhysicsWorld.Reset();
    }

    void Scene::OnRuntimeStop()
    {
        m_State.store(State::Edit);

        if (m_FixedUpdateThread.joinable())
            m_FixedUpdateThread.join();

        m_PhysicsWorld.Reset();
    }

    void Scene::OnEditorUpdate([[maybe_unused]] const f32 deltaTime, scene::EditorCamera& camera)
    {
        // Render
        {
            gfx::BatchRenderer2D::Begin(camera);
            RenderSprites();
            gfx::BatchRenderer2D::End();
        }
    }

    void Scene::OnSimulationUpdate([[maybe_unused]] const f32 deltaTime, scene::EditorCamera& camera)
    {
        // Render
        {
            gfx::BatchRenderer2D::Begin(camera);
            RenderSprites();
            gfx::BatchRenderer2D::End();
        }
    }

    void Scene::OnRuntimeUpdate(const f32 deltaTime)
    {
        // Render.
        {
            // Grab primary camera.
            {
                auto registry = m_Registry.Lock();

                // Grab the primary camera entity.
                {
                    auto view = registry->view<TransformComponent, CameraComponent>();
                    for (auto& e : view)
                    {
                        auto& c = view.get<CameraComponent>(e);
                        if (c.primary)
                        {
                            *m_PrimaryCameraEntity = Entity{ e, this };
                            break;
                        }
                    }
                }
            }

            // Render our sprites if there's a camera.
            if (m_PrimaryCameraEntity)
            {
                const auto& cc = m_PrimaryCameraEntity->GetComponent<CameraComponent>();
                const auto& tc = m_PrimaryCameraEntity->GetComponent<TransformComponent>();

                gfx::BatchRenderer2D::Begin(cc.camera, tc);
                RenderSprites();
                gfx::BatchRenderer2D::End();
            }
        }

        // Native scripts.
        {
            // Lock the registry only for one statement because user scripts can also possibly lock the registry
            // for interactions (such as calls to GetComponent<T>, HasComponent<T> etc...) instead of
            // locking for the entire scope.
            auto view = m_Registry->view<NativeBehaviourComponent>();
            for (auto& e : view)
            {
                auto& nbc = view.get<NativeBehaviourComponent>(e);
                try
                {
                    nbc.OnUpdate(deltaTime);
                }
                catch (const NativeBehaviourException& ex)
                {
                    // If NBC fails to update all components then most likely one of the Behaviours threw an error
                    // occured; revert state back to State::Edit and halt physics simulation.
                    OnRuntimeStop();

                    // TODO: We should stop calling lgx::Get for every log.
                    // TODO: Review this inner exception thing for later.
                    auto& exi = ex.InnerException();
                    lgx::Get("engine").Error("A Behaviour threw an Error: {}", exi.to_string());
                    return;
                }
            }
        }
    }

    NativeBehaviour* Scene::CreateBehaviour(const char* className, Entity parent)
    {
        auto* ptr = RF_INSTANCE_CREATE(s_ScriptModule, className, parent);
        if (ptr)
            s_PossibleAttachedScripts[parent] = className;
        return ptr;
    }

    bool Scene::BehaviourExists(const char* className)
    {
        return RF_INSTANCE_CHECK(s_ScriptModule, className);
    }

    void Scene::LoadScriptModule(std::filesystem::path modulePath)
    {
        if (s_ScriptModule)
            UnloadScriptModule();
        s_ScriptModule = mem::Box<sys::DLib>::New(std::move(modulePath));

        if (!s_PossibleAttachedScripts.empty())
        {
            for (auto& [entity, name] : s_PossibleAttachedScripts)
            {
                if (entity.HasComponent<NativeBehaviourComponent>())
                {
                    // Why the fuck is entity const here? Whatever, fuck it, I will just cast the constness away.
                    // I gave up with this dogshit language long time ago.
                    auto& nbc = const_cast<NativeBehaviourComponent&>(entity.GetComponent<NativeBehaviourComponent>());
                    if (BehaviourExists(name.c_str()))
                        nbc.Attach(RF_INSTANCE_CREATE(s_ScriptModule, name.c_str(), entity));
                }
            }
            s_PossibleAttachedScripts.clear();
        }

        lgx::Get("engine").Log(lgx::Info, "Script module loaded.");
    }

    void Scene::UnloadScriptModule()
    {
        // When unloading we need to check for possible attached scripts, if any script is attached then record its
        // name so that we can attached them back (that is if they still exist) when the module is loaded back.
        for (auto& [entity, name] : s_PossibleAttachedScripts)
        {
            auto& nbc = const_cast<NativeBehaviourComponent&>(entity.GetComponent<NativeBehaviourComponent>());
            if (nbc.m_Behaviours.contains(name))
                nbc.Detach(name);
        }
        /*
        for (auto& e : view)
        {
            auto&                  nbc        = view.get<NativeBehaviourComponent>(e);
            auto&                  behaviours = nbc.GetBehaviours();
            auto                   it         = behaviours.begin();
            std::list<std::string> to_be_detached;

            for (auto& [name, _] : behaviours)
            {
                s_PossibleAttachedScripts[static_cast<Entity::HandleType>(e)] = name;
                to_be_detached.emplace_back(name);
            }

            for (const auto& e : to_be_detached)
                nbc.Detach(e);
        }
        */

        s_ScriptModule.Reset();
        lgx::Get("engine").Log(lgx::Info, "Script module unloaded.");
    }

    void Scene::OnFixedUpdate(Scene& self) noexcept
    {
        using clock = std::chrono::high_resolution_clock;

        const auto frame_interval     = 1.0f / self.m_PhysicsProperties.tickRate;
        const auto max_frame_interval = 1.0f / Application::GetFps();

        auto lag         = 0.0f;
        auto frame_start = clock::now();
        while (self.m_State.load() != State::Edit)
        {
            const auto frame_now  = clock::now();
            const auto frame_time = std::chrono::duration<f32>(clock::now() - frame_start).count();
            frame_start           = frame_now;

            lag += frame_time;

            while (lag >= frame_interval)
            {
                // Native behaviours.
                if (self.m_State.load() == State::Play)
                {
                    // Lock the registry only for one statement because user scripts can also possibly lock the registry
                    // for interactions (such as calls to GetComponent<T>, HasComponent<T> etc...) instead of
                    // locking for the entire scope.
                    auto view = self.m_Registry->view<NativeBehaviourComponent>();
                    for (auto& e : view)
                    {
                        auto& nbc = view.get<NativeBehaviourComponent>(e);
                        nbc.OnFixedUpdate(frame_interval);
                    }
                }

                // Physics.
                {
                    auto registry = self.m_Registry.Lock();
                    auto view     = registry->view<TransformComponent, RigidBody2DComponent>();
                    self.m_PhysicsWorld->Step(frame_interval, self.m_PhysicsProperties.velocityIterations,
                                              self.m_PhysicsProperties.positionIterations);
                    for (auto& e : view)
                    {
                        auto& trans = view.get<TransformComponent>(e);
                        auto& rb2d  = view.get<RigidBody2DComponent>(e);

                        auto        b2_body = reinterpret_cast<b2Body*>(rb2d.runtimeBody);
                        const auto& b2_pos  = b2_body->GetPosition();

                        trans.position.x = b2_pos.x / self.m_PhysicsProperties.scalingFactor + 1.0f;
                        trans.position.y = b2_pos.y / self.m_PhysicsProperties.scalingFactor + 1.0f;
                        trans.rotation.z = math::ToDegf(b2_body->GetAngle());
                    }
                }

                lag -= frame_interval;
            }

            // Upper bound for the outer loop.
            if (frame_time < max_frame_interval)
                std::this_thread::sleep_for(std::chrono::duration<f32>(max_frame_interval - frame_time));
        }
    }

    // TODO: Use ADL Serializer.
    void to_json(nlohmann::ordered_json& j, const Scene& scene)
    {
        const auto entities = ((Scene&)scene).GetAllEntities();
        j["Name"]           = scene.m_Name;
        j["Entities"]       = entities;
    }
} // namespace codex
