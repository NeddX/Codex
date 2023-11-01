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

    class Scene
    {
        friend class Serializer;
        friend class Window;
        friend class Entity;

    private:
        entt::registry m_Registry;
        std::string    m_Name = "Default scene";

    public:
        Scene()  = default;
        ~Scene() = default;

    public:
        inline std::string&       GetName() noexcept { return m_Name; }
        inline const std::string& GetName() const noexcept { return m_Name; }
        inline usize              GetEntityCount() const noexcept { return m_Registry.size(); }

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
        friend void to_json(nlohmann::json& j, const Scene& scene);
    };
} // namespace codex

#endif // CODEX_CORE_SCENE_H
