#ifndef CODEX_SCENE_COMPONENTS_H
#define CODEX_SCENE_COMPONENTS_H

#include <sdafx.h>

#include "../Core/Geomtryd.h"
#include "../NativeBehaviour/NativeBehaviour.h"
#include "Sprite.h"

namespace codex {
    // Forward decelerations
    class Camera;
    class Entity;
    class Serializer;

    struct CODEX_API IComponent
    {
        friend class Serializer;
        friend class Entity;

    protected:
        std::unique_ptr<Entity> m_Parent;

    protected:
        virtual void Start() {}
        virtual ~IComponent() = default;

    public:
        CX_COMPONENT_SERIALIZABLE()
    };

    struct CODEX_API TagComponent : public IComponent
    {
        friend class Serializer;
        friend class Entity;

    public:
        std::string tag;

    public:
        TagComponent();
        TagComponent(const std::string_view tag);

    public:
        CX_COMPONENT_SERIALIZABLE()
    };

    struct CODEX_API TransformComponent : public IComponent
    {
        friend class Serializer;
        friend class Entity;

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

    public:
        CX_COMPONENT_SERIALIZABLE()
    };

    struct CODEX_API SpriteRendererComponent : public IComponent
    {
        friend class Serializer;
        friend class Entity;

    private:
        // TODO: Handle sprite renderers that do not have an actual sprite.
        Sprite m_Sprite;

    public:
        SpriteRendererComponent(const Sprite& sprite);

    public:
        inline Sprite& GetSprite() { return m_Sprite; }

    public:
        CX_COMPONENT_SERIALIZABLE()
    };

    struct CODEX_API GridRendererComponent : public IComponent
    {
        friend class Serializer;
        friend class Entity;

    private:
        Camera*  m_Camera;
        Vector2f m_GridSize;

    public:
        GridRendererComponent(const Vector2f gridSize);

    public:
        inline Vector2f GetGridSize() const { return m_GridSize; }
        inline void     SetGridSize(const Vector2f newGridSize) { m_GridSize = newGridSize; }

    public:
        void Render();

    public:
        CX_COMPONENT_SERIALIZABLE()
    };

    class CODEX_API ScriptException : public CodexException
    {
        using CodexException::CodexException;

    public:
        constexpr const char* default_message() const noexcept override { return "An unknown behaviour exception."; }
    };

    struct NativeBehaviourComponent : public IComponent
    {
        friend class Serializer;
        friend class Entity;

    public:
        std::unordered_map<std::string_view, NativeBehaviour*> behaviours;

    public:
        void Attach(NativeBehaviour* bh)
        {
            bh->Serialize();
            const std::string_view name = bh->m_SerializedData.begin().key();
            if (!behaviours.contains(name))
                behaviours[name] = bh;
        }
        NativeBehaviour* Detach(const std::string_view className)
        {
            auto it = behaviours.find(className);
            if (it != behaviours.end())
            {
                auto* ptr = it->second;
                behaviours.erase(it, behaviours.end());
                return ptr;
            }
            else
            {
                cx_throw(ScriptException, "Tried to detach a behaviour ({}) that is not attach on first place.",
                         className);
                return nullptr;
            }
        }
        void InstantiateBehaviour(const std::string_view className)
        {
            if (behaviours.contains(className))
                behaviours.at(className)->Init();
            else
                cx_throw(ScriptException, "Tried to instantiate a non-existent behaviour class {}.", className);
        }
        void Dispose(const std::string_view className)
        {
            auto it = behaviours.find(className);
            if (it != behaviours.end())
            {
                delete it->second;
                behaviours.erase(it, behaviours.end());
            }
            else
            {
                cx_throw(ScriptException, "Tried to dispose a behaviour ({}) that is not attach on first place.",
                         className);
            }
        }

    public:
        CX_COMPONENT_SERIALIZABLE()
    };

    struct CODEX_API TilemapComponent : public IComponent
    {
        friend class Serializer;
        friend class Entity;

    private:
        ResRef<Texture2D>                                     m_Texture;
        std::map<i32, std::unordered_map<Vector2f, Vector2f>> m_Tiles;
        Vector2f                                              m_GridSize;
        Vector2f                                              m_TileSize;
        i32                                                   m_Layer;
        GridRendererComponent*                                m_GridRenderer;

    public:
        TilemapComponent(ResRef<Texture2D> texture, const Vector2f gridSize, const Vector2f tileSize);

    protected:
        void Start() override;

    public:
        inline const std::map<i32, std::unordered_map<Vector2f, Vector2f>>& GetAllTiles() const { return m_Tiles; }
        inline ResRef<Texture2D>                                            GetTexture() const { return m_Texture; }
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
        inline void SetTexture(ResRef<Texture2D> newTexuture) { m_Texture = newTexuture; }
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

    public:
        CX_COMPONENT_SERIALIZABLE()
    };
} // namespace codex

#endif // CODEX_SCENE_COMPONENTS_H
