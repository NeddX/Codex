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

    class Scene
    {
        friend class Window;
        friend class Entity;

    protected:
        i32                     m_Width;
        i32                     m_Height;
        std::unique_ptr<Camera> m_Camera;
        entt::registry          m_Registry;

    private:
        bool m_Running;

    public:
        Scene(const i32 width, const i32 height);
        ~Scene() = default;

    public:
        inline Camera*  GetCamera() const { return m_Camera.get(); }
        inline Vector2f GetMousePositionInWorld()
        {
            return Vector2f((f32)Input::GetMouseX() + m_Camera->position.x,
                            (f32)Input::GetMouseY() + m_Camera->position.y);
            return Vector2f();
        }

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
        void OnWindowResize_Event(const i32 newWidth, const i32 newHeight)
        {
            m_Camera->SetWidth(newWidth);
            m_Camera->SetHeight(newHeight);
        }

    public:
        void Start();
        void Update(const f32 deltaTime);
    };
} // namespace codex

#endif // CODEX_CORE_SCENE_H
