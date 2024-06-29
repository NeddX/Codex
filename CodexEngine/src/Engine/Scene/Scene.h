#ifndef CODEX_SCENE_H
#define CODEX_SCENE_H

#include <Engine/Concurrency/Mutex.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/ResourceHandler.h>
#include <Engine/Core/UUID.h>
#include <Engine/Graphics/Renderer.h>
#include <Engine/Memory/Memory.h>
#include <Engine/Scene/Camera.h>
#include <Engine/Scene/EditorCamera.h>
#include <Engine/Scene/SpriteSheet.h>
#include <Engine/System/DynamicLibrary.h>

namespace codex {
    // Forward declarations
    class Window;
    class Entity;
    class Serializer;
    class NativeBehaviour;

    class CODEX_API Scene
    {
        friend class Window;
        friend class Entity;
        friend class Serializer;

    public:
        struct PhysicsProperties
        {
            i32      tickRate           = 60;
            u32      velocityIterations = 6;
            u32      positionIterations = 2;
            f32      scalingFactor      = 1.0f / 64.0f;
            Vector2f gravity            = { 0.0f, -9.8f };
        };
        enum class State
        {
            Play,
            Edit,
            Simulate
        };

    private:
        cc::Mutex<entt::registry> m_Registry;
        std::string               m_Name         = "Default scene";
        mem::Box<b2World>         m_PhysicsWorld = nullptr;
        std::atomic<State>        m_State        = State::Edit;
        std::thread               m_FixedUpdateThread;
        PhysicsProperties         m_PhysicsProperties;
        mem::Box<Entity>          m_PrimaryCameraEntity = nullptr;

    private:
        static mem::Box<sys::DLib>                     s_ScriptModule;
        static std::unordered_map<Entity, std::string> s_PossibleAttachedScripts;

    public:
        Scene() noexcept                        = default;
        Scene(const Scene&) noexcept            = delete;
        Scene& operator=(const Scene&) noexcept = delete;
        Scene(Scene&& other) noexcept;
        Scene& operator=(Scene&& other) noexcept;
        ~Scene() noexcept;

    public:
        [[nodiscard]] inline State GetState() const noexcept { return m_State.load(); }
        inline void                SetState(const State newState) noexcept { return m_State.store(newState); }
        [[nodiscard]] inline PhysicsProperties&       GetPhysicsProperties() noexcept { return m_PhysicsProperties; }
        [[nodiscard]] inline const PhysicsProperties& GetPhysicsProperties() const noexcept
        {
            return const_cast<Scene*>(this)->GetPhysicsProperties();
        }
        // TODO: Have a IDisplay trait which allows for
        // displaying the names of the objects just like in UE.
        [[nodiscard]] inline std::string_view GetName() const noexcept { return m_Name; }
        [[nodiscard]] inline usize            GetEntityCount() const noexcept
        {
            return m_Registry->view<entt::entity>().size_hint();
        }
        inline void Swap(Scene& other) noexcept
        {
            m_Registry.Swap(other.m_Registry);
            std::swap(m_Name, other.m_Name);
            std::swap(s_ScriptModule, other.s_ScriptModule);
            std::swap(m_FixedUpdateThread, other.m_FixedUpdateThread);
            std::swap(m_PhysicsWorld, other.m_PhysicsWorld);
        }

    public:
        template <typename T>
        std::vector<Entity> GetAllEntitiesWithComponent() noexcept;

    public:
        void   CopyTo(Scene& other) const noexcept;
        Entity CreateEntity(const std::string_view tag = "default tag", UUID uuid = UUID{}) noexcept;
        void   RemoveEntity(const Entity entity);
        void   RemoveEntity(const u32 entity);
        [[nodiscard]] std::vector<Entity> GetAllEntitesWithTag(const std::string_view tag);
        [[nodiscard]] std::vector<Entity> GetAllEntities();
        [[nodiscard]] Entity              GetPrimaryCameraEntity() noexcept;

    private:
        void RenderSprites();
        void ConstructPhysicsBodies();

    public:
        void OnEditorInit(scene::EditorCamera& camera);
        void OnRuntimeStart();
        void OnRuntimeStop();
        void OnSimulationStart();
        void OnSimulationStop();
        void OnEditorUpdate(const f32 deltaTime, scene::EditorCamera& camera);
        void OnRuntimeUpdate(const f32 deltaTime);
        void OnSimulationUpdate(const f32 deltaTime, scene::EditorCamera& camera);

    public:
        static NativeBehaviour* CreateBehaviour(const char* className, Entity parent);
        static bool             BehaviourExists(const char* className);
        static void             LoadScriptModule(std::filesystem::path modulePath);
        static void             UnloadScriptModule();

    private:
        static void OnFixedUpdate(Scene& self) noexcept;

    public:
        friend void to_json(nlohmann::ordered_json& j, const Scene& scene);
        template <typename T>
            requires(std::is_move_constructible_v<T>)
        friend std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene) noexcept;

        template <typename T>
            requires(!std::is_move_constructible_v<T>)
        friend std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene) noexcept;
    };
} // namespace codex

#endif // CODEX_SCENE_H
