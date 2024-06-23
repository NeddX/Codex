#ifndef CODEX_SCENE_H
#define CODEX_SCENE_H

#include "../Core/Input.h"
#include "../Core/ResourceHandler.h"
#include "../Graphics/Renderer.h"
#include "../Memory/Memory.h"
#include "../Scene/Camera.h"
#include "../Scene/EditorCamera.h"
#include "../Scene/SpriteSheet.h"
#include "../System/DynamicLibrary.h"

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

    private:
        entt::registry      m_Registry;
        std::string         m_Name         = "Default scene";
        mem::Box<sys::DLib> m_ScriptModule = nullptr;

    public:
        Scene() noexcept             = default;
        Scene(const Scene&) noexcept = delete;
        Scene(Scene&&) noexcept      = default;
        ~Scene() noexcept;

    public:
        Scene& operator=(const Scene&) noexcept = delete;
        Scene& operator=(Scene&&) noexcept      = default;

    public:
        // TODO: Have a IDisplay trait which allows for
        // displaying the names of the objects just like in UE.
        inline std::string&       GetName() noexcept { return m_Name; }
        inline const std::string& GetName() const noexcept { return m_Name; }
        inline usize GetEntityCount() const noexcept { return m_Registry.view<entt::entity>().size_hint(); }

    public:
        template <typename T>
        std::vector<Entity> GetAllEntitiesWithComponent();

    public:
        Entity              CreateEntity(const std::string_view tag = "default tag");
        void                RemoveEntity(const Entity entity);
        void                RemoveEntity(const u32 entity);
        std::vector<Entity> GetAllEntitesWithTag(const std::string_view tag);
        std::vector<Entity> GetAllEntities();

    public:
        void             LoadScriptModule(std::filesystem::path modulePath);
        void             UnloadScriptModule();
        NativeBehaviour* CreateBehaviour(const char* className) const noexcept;
        bool             BehaviourExists(const char* className) const noexcept;

    public:
        void OnEditorInit(scene::EditorCamera& camera);
        void OnRuntimeInit();
        void OnEditorUpdate(const f32 deltaTime);
        void OnRuntimeUpdate(const f32 deltaTime);

    public:
        friend void to_json(nlohmann::ordered_json& j, const Scene& scene);
        template <typename T>
            requires(std::is_move_constructible_v<T>)
        friend std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene);

        template <typename T>
            requires(!std::is_move_constructible_v<T>)
        friend std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene);
    };
} // namespace codex

#endif // CODEX_SCENE_H
