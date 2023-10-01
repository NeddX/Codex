#ifndef CODEX_SCENE_COMPONENTS_H
#define CODEX_SCENE_COMPONENTS_H

#include <sdafx.h>

#include "../Sprite.h"
#include "../../Core/Geomtryd.h" 

namespace codex {
	// Forward decelerations
	class Camera;
	class Entity;

	struct IComponent
	{
		friend class Entity;

	protected:
		std::unique_ptr<Entity> m_Parent;

	protected:
		virtual void Start() { }
		virtual ~IComponent() = default;
	};

	struct TagComponent : public IComponent
	{
		friend class Entity;

	public:
		std::string tag;

	public:
		TagComponent();
		TagComponent(const std::string_view tag);
	};

	struct TransformComponent : public IComponent
	{
		friend class Entity;

	public:
		Vector3f position;
		Vector3f rotation; // Use quaternions
		Vector3f scale;

	public:
		TransformComponent(
			const Vector3f position = Vector3f(),
			const Vector3f rotation = Vector3f(),
			const Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f));

	};

	/*struct TransformComponent
	{
	public:
		Matrix4f transform;

	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(Matrix4f transform) : transform(transform) {}
	};*/

	struct SpriteRendererComponent : public IComponent
	{
		friend class Entity;

	private:
		Vector4f m_Colour;
		// TODO: Handle sprite renderers that do not have an actual sprite.
		Sprite m_Sprite;
		i32 m_ZIndex;

	public:
		SpriteRendererComponent(const Vector4f colour);
		SpriteRendererComponent(const Sprite sprite, const Vector4f colour = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

	public:
		inline Vector4f GetColour() const
			{ return m_Colour; }
		inline Sprite& GetSprite()
			{ return m_Sprite; }
		inline i32 GetZIndex() const
			{ return m_ZIndex; }
		inline void SetSprite(const Sprite newSprite)
			{ m_Sprite = newSprite; }
		inline void SetZIndex(const i32 newIndex)
			{ m_ZIndex = newIndex; }
		inline void SetColour(const Vector4f newColour)
			{ m_Colour = newColour; }
	};

	struct GridRendererComponent : public IComponent
	{
		friend class Entity;

	private:
		Camera* m_Camera;
		Vector2f m_GridSize;

	public:
		GridRendererComponent(const Vector2f gridSize);

	public:
		inline Vector2f GetGridSize() const
			{ return m_GridSize; }
		inline void SetGridSize(const Vector2f newGridSize)
			{ m_GridSize = newGridSize; }

	public:
		void Render();
	};

	struct TilemapComponent : public IComponent
	{
		friend class Entity;

	private:
		std::shared_ptr<Texture2D> m_Texture;
		std::map<i32, std::unordered_map<Vector2f, Vector2f>> m_Tiles;
		Vector2f m_GridSize;
		Vector2f m_TileSize;
		i32 m_Layer;
		GridRendererComponent* m_GridRenderer;

	public:
		TilemapComponent(std::shared_ptr<Texture2D> texture, const Vector2f gridSize, const Vector2f tileSize);

	protected:
		void Start() override;

	public:
		inline const std::map<i32, std::unordered_map<Vector2f, Vector2f>>& GetAllTiles() const
			{ return m_Tiles; }
		inline std::shared_ptr<Texture2D> GetTexture() const
			{ return m_Texture; }
		inline Vector2f GetGridSize() const
			{ return m_GridSize; }
		inline Vector2f GetTileSize() const
			{ return m_TileSize; }
		inline i32 GetLayer() const
			{ return m_Layer; }
		inline usize GetTotalSize()
		{
			usize total_size = m_Tiles.size();
			for (const auto& e : m_Tiles) total_size += e.second.size();
			return total_size;
		}
		inline void SetTexture(std::shared_ptr<Texture2D> newTexuture)
			{ m_Texture = newTexuture; }
		inline void SetGridSize(const Vector2f newGridSize)
			{ m_GridSize = newGridSize; m_GridRenderer->SetGridSize(newGridSize); }
		inline void SetTileSize(const Vector2f newTileSize)
			{ m_TileSize = newTileSize; }
		inline void SetLayer(const i32 layer)
			{ m_Layer = layer; }

	public:
		void AddTile(const Vector2f worldPos, const i32 tileId);
		void AddTile(const Vector2f worldPos, const Vector2f tilePos);
		void RemoveTile(const Vector2f worldPos);
	};
}

#endif // CODEX_SCENE_COMPONENTS_H
