#ifndef CODEX_SCENE_COMPONENTS_H
#define CODEX_SCENE_COMPONENTS_H

#include <sdafx.h>

#include "../Sprite.h"
#include "../../Core/Geomtryd.h" 

namespace Codex {
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
	};

	struct TagComponent : public IComponent
	{
		friend class Entity;

	public:
		std::string tag;

	public:
		TagComponent();
		TagComponent(const std::string& tag);
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
			Vector3f position = Vector3f(),
			Vector3f rotation = Vector3f(),
			Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f));
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
		int m_ZIndex;

	public:
		SpriteRendererComponent(const Vector4f colour);
		SpriteRendererComponent(Sprite sprite, Vector4f colour = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

	public:
		inline Vector4f GetColour() const { return m_Colour; }
		inline Sprite& GetSprite() { return m_Sprite; }
		inline int GetZIndex() const { return m_ZIndex; }
		inline void SetSprite(Sprite newSprite) { m_Sprite = newSprite; }
		inline void SetZIndex(int newIndex) { m_ZIndex = newIndex; }
		inline void SetColour(Vector4f newColour) { m_Colour = newColour; }
	};

	struct GridRendererComponent : public IComponent
	{
		friend class Entity;

	private:
		Camera* m_Camera;
		Vector2f m_GridSize;

	public:
		GridRendererComponent(Vector2f gridSize);

	public:
		inline Vector2f GetGridSize() const { return m_GridSize; }
		inline void SetGridSize(Vector2f newGridSize) { m_GridSize = newGridSize; }

	public:
		void Render();
	};

	struct TilemapComponent : public IComponent
	{
		friend class Entity;

	private:
		std::shared_ptr<Texture2D> m_Texture;
		std::map<int, std::unordered_map<Vector2f, Vector2f>> m_Tiles;
		Vector2f m_GridSize;
		Vector2f m_TileSize;
		int m_Layer;
		GridRendererComponent* m_GridRenderer;

	public:
		TilemapComponent(std::shared_ptr<Texture2D> texture, Vector2f gridSize, Vector2f tileSize);

	protected:
		void Start() override;

	public:
		inline const std::map<int, std::unordered_map<Vector2f, Vector2f>>& GetAllTiles() const { return m_Tiles; }
		inline std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }
		inline Vector2f GetGridSize() const { return m_GridSize; }
		inline Vector2f GetTileSize() const { return m_TileSize; }
		inline int GetLayer() const { return m_Layer; }
		inline size_t GetTotalSize()
		{
			size_t total_size = m_Tiles.size();
			for (const auto& e : m_Tiles) total_size += e.second.size();
			return total_size;
		}
		inline void SetTexture(std::shared_ptr<Texture2D> newTexuture) { m_Texture = newTexuture; }
		inline void SetGridSize(Vector2f newGridSize) { m_GridSize = newGridSize; m_GridRenderer->SetGridSize(newGridSize); }
		inline void SetTileSize(Vector2f newTileSize) { m_TileSize = newTileSize; }
		inline void SetLayer(int layer) { m_Layer = layer; }

	public:
		void AddTile(Vector2f worldPos, int tileId);
		void AddTile(Vector2f worldPos, Vector2f tilePos);
		void RemoveTile(Vector2f worldPos);
	};
}

#endif // CODEX_SCENE_COMPONENTS_H