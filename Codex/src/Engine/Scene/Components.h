#ifndef CODEX_SCENE_COMPONENTS_H
#define CODEX_SCENE_COMPONENTS_H

#include <sdafx.h>

#include "../Core/Geomtryd.h"
#include "../Memory/Memory.h"
#include "../NativeBehaviour/NativeBehaviour.h"
#include "Camera.h"
#include "Sprite.h"

namespace codex {
    // Forward decelerations
    class Entity;
    class Serializer;

    struct IComponent
    {
        CX_COMPONENT

    protected:
        Entity m_Parent;

    protected:
        virtual void OnInit() {}
        virtual ~IComponent() = default;
    };

    struct CODEX_API TagComponent : public IComponent
    {
        CX_COMPONENT

    public:
        std::string tag;

    public:
        TagComponent();
        TagComponent(const std::string_view tag);
    };

    struct CODEX_API TransformComponent : public IComponent
    {
        CX_COMPONENT

    public:
        Vector3f position;
        Vector3f rotation;
        Vector3f scale;

    public:
        TransformComponent(const Vector3f position = Vector3f(), const Vector3f rotation = Vector3f(),
                           const Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f));

    public:
        inline Matrix4f GetTransform() const noexcept
        {
            Matrix4f transform_mat = glm::identity<Matrix4f>();
            transform_mat          = glm::translate(transform_mat, position);
            transform_mat          = glm::rotate(transform_mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            transform_mat          = glm::rotate(transform_mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            transform_mat          = glm::rotate(transform_mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            transform_mat          = glm::scale(transform_mat, scale);
            return transform_mat;
        }
    };

    struct CODEX_API SpriteRendererComponent : public IComponent
    {
        CX_COMPONENT

    private:
        // TODO: Handle sprite renderers that do not have an actual sprite.
        Sprite m_Sprite;

    public:
        SpriteRendererComponent(Sprite sprite);

    public:
        inline Sprite& GetSprite() { return m_Sprite; }
    };

    struct CODEX_API GridRendererComponent : public IComponent
    {
        CX_COMPONENT

    private:
        scene::Camera* m_Camera;
        Vector2f       m_GridSize;

    public:
        GridRendererComponent(const Vector2f gridSize);

    public:
        inline Vector2f GetGridSize() const { return m_GridSize; }
        inline void     SetGridSize(const Vector2f newGridSize) { m_GridSize = newGridSize; }

    public:
        void Render();
    };

    CX_CUSTOM_EXCEPTION(ScriptException, "An unknown behaviour exception occured.")
    CX_CUSTOM_EXCEPTION(DuplicateBehaviourException, "Cnnaot have more than one type of behaviour on a single entity.")

    struct NativeBehaviourComponent : public IComponent
    {
        CX_COMPONENT

    public:
        std::unordered_map<std::string_view, mem::Box<NativeBehaviour>> behaviours;

    public:
        NativeBehaviourComponent() {}
        NativeBehaviourComponent(const NativeBehaviourComponent&) = delete;

    public:
        void OnInit() override
        {
            for (auto& [k, v] : behaviours)
            {
                v->OnInit();
            }
        }
        void Attach(mem::Box<NativeBehaviour> bh)
        {
            bh->Serialize();
            bh->m_Owner                 = m_Parent;
            const std::string_view name = bh->m_SerializedData.begin().key();
            if (!behaviours.contains(name))
                behaviours[name] = std::move(bh);
        }
        mem::Box<NativeBehaviour> Detach(const std::string_view className)
        {
            auto it = behaviours.find(className);
            if (it != behaviours.end())
            {
                auto ptr = std::move(it->second);
                behaviours.erase(it, behaviours.end());
                return std::move(ptr);
            }
            else
            {
                cx_throw(ScriptException, "Tried to detach a behaviour ({}) that is not attach on first place.",
                         className);
            }
            return nullptr;
        }
        void InstantiateBehaviour(const std::string_view className)
        {
            if (behaviours.contains(className))
                behaviours.at(className)->OnInit();
            else
                cx_throw(ScriptException, "Tried to instantiate a non-existent behaviour class {}.", className);
        }
        void Dispose(const std::string_view className)
        {
            auto it = behaviours.find(className);
            if (it != behaviours.end())
            {
                behaviours.erase(it, behaviours.end());
            }
            else
            {
                cx_throw(ScriptException, "Tried to dispose a behaviour ({}) that is not attach on first place.",
                         className);
            }
        }
        void DisposeBehaviours() { behaviours.clear(); }

    public:
        template <typename T, typename... TArgs>
        T& New(TArgs&&... args)
            requires(std::is_base_of_v<NativeBehaviour, T>)
        {
            for (const auto& [k, v] : behaviours)
            {
                if (typeid(v) == typeid(T))
                    cx_throwd(DuplicateBehaviourException);
            }

            mem::Box<NativeBehaviour> bh{ new T(std::forward<TArgs>(args)...) };
            bh->m_Owner = m_Parent;
            bh->OnInit();
            bh->Serialize();
            const std::string_view name = bh->m_SerializedData.begin().key();
            if (!behaviours.contains(name))
                behaviours[name] = std::move(bh);

            return *(T*)behaviours[name].Get();
        }
    };

    struct CODEX_API TilemapComponent : public IComponent
    {
        CX_COMPONENT

    private:
        ResRef<gfx::Texture2D>                                m_Texture;
        std::map<i32, std::unordered_map<Vector2f, Vector2f>> m_Tiles;
        Vector2f                                              m_GridSize;
        Vector2f                                              m_TileSize;
        i32                                                   m_Layer;
        GridRendererComponent*                                m_GridRenderer;

    public:
        TilemapComponent(ResRef<gfx::Texture2D> texture, const Vector2f gridSize, const Vector2f tileSize);

    protected:
        void OnInit() override;

    public:
        inline const std::map<i32, std::unordered_map<Vector2f, Vector2f>>& GetAllTiles() const { return m_Tiles; }
        inline ResRef<gfx::Texture2D>                                       GetTexture() const { return m_Texture; }
        inline Vector2f                                                     GetGridSize() const { return m_GridSize; }
        inline Vector2f                                                     GetTileSize() const { return m_TileSize; }
        inline i32                                                          GetLayer() const { return m_Layer; }
        inline usize                                                        GetTotalSize()
        {
            usize total_size = m_Tiles.size();
            for (const auto& e : m_Tiles)
                total_size += e.second.size();
            return total_size;
        }
        inline void SetTexture(ResRef<gfx::Texture2D> newTexuture) { m_Texture = newTexuture; }
        inline void SetGridSize(const Vector2f newGridSize)
        {
            m_GridSize = newGridSize;
            m_GridRenderer->SetGridSize(newGridSize);
        }
        inline void SetTileSize(const Vector2f newTileSize) { m_TileSize = newTileSize; }
        inline void SetLayer(const i32 layer) { m_Layer = layer; }

    public:
        void AddTile(const Vector2f worldPos, const i32 tileId);
        void AddTile(const Vector2f worldPos, const Vector2f tilePos);
        void RemoveTile(const Vector2f worldPos);
    };
} // namespace codex

#endif // CODEX_SCENE_COMPONENTS_H
