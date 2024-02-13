#ifndef CODEX_CORE_SCENE_H
#define CODEX_CORE_SCENE_H

#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Core/ResourceHandler.h"
#include "../Renderer/Renderer.h"
#include "../Scene/SpriteSheet.h"

namespace codex {
    // Forward declarations
    class Window;
    class Entity;
    class Serializer;

    class CODEX_API Scene
    {
        friend class Serializer;
        friend class Window;
        friend class Entity;

    private:
        entt::registry m_Registry;
        std::string    m_Name = "Default scene";

    public:
        // TODO: Have a IDisplay trait which allows for
        // displaying the names of the objects just like in UE.
        inline std::string&       GetName() noexcept { return m_Name; }
        inline const std::string& GetName() const noexcept { return m_Name; }
        inline usize              GetEntityCount() const noexcept { return m_Registry.view<entt::entity>().size_hint(); }

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
        void Start();
        void Update(const f32 deltaTime);

    public:
        friend void to_json(nlohmann::ordered_json& j, const Scene& scene);
        template <typename T>
            requires(std::is_copy_constructible_v<T>)
        friend std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene);

        template <typename T>
            requires(!std::is_copy_constructible_v<T>)
        friend std::vector<Entity> SceneGetAllEntitiesWithComponent(Scene& scene);
    };
} // namespace codex

#endif // CODEX_CORE_SCENE_H
