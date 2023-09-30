#include "Components.h"
#include "../../Core/Window.h"
#include "../../Renderer/DebugDraw.h"

namespace Codex {

	TagComponent::TagComponent()
	{
		tag = "default tag";
	}

	TagComponent::TagComponent(const std::string_view tag) : tag(std::string(tag))
	{

	}

	TransformComponent::TransformComponent(const Vector3f position, const Vector3f rotation, const Vector3f scale) :
		position(position), rotation(rotation), scale(scale)
	{

	}

	SpriteRendererComponent::SpriteRendererComponent(const Vector4f colour) :
		m_Colour(colour), m_Sprite(nullptr), m_ZIndex(0)
	{

	}

	SpriteRendererComponent::SpriteRendererComponent(const Sprite sprite, const Vector4f colour) :
		m_Colour(colour), m_Sprite(sprite), m_ZIndex(0)
	{

	}
	
	GridRendererComponent::GridRendererComponent(const Vector2f gridSize) :
		m_GridSize(gridSize)
	{
		m_Camera = Window::Get()->GetCurrentScene()->GetCamera();
	}

	void GridRendererComponent::Render()
	{
		Vector2f camera_pos = m_Camera->position;
		Vector2f proj_size = { m_Camera->GetWidth(), m_Camera->GetHeight() };

		int first_x = ((int)(camera_pos.x / m_GridSize.x) - 1) * m_GridSize.x;
		int first_y = ((int)(camera_pos.y / m_GridSize.y) - 1) * m_GridSize.y;

		int rows = (int)(proj_size.y / m_GridSize.y) + 2;
		int cols = (int)(proj_size.x / m_GridSize.x) + 2;

		int width = (int)proj_size.x + m_GridSize.x * 2;
		int height = (int)proj_size.y + m_GridSize.y * 2;

		Vector4f colour = { 0.5f, 0.5f, 0.5f, 1.0f };
		int max_lines = std::max(rows, cols);
		for (int i = 0; i < max_lines; ++i)
		{
			int x = m_GridSize.x * i + first_x, y = m_GridSize.y * i + first_y;

			if (i <= cols)
				DebugDraw::DrawLine2D({ x, first_y }, { x, first_y + height }, colour);
			if (i <= rows)
				DebugDraw::DrawLine2D({ first_x, y }, { first_x + width, y }, colour);
		}
	}

	TilemapComponent::TilemapComponent(std::shared_ptr<Texture2D> texture, const Vector2f gridSize, const Vector2f tileSize) :
		m_Texture(texture), m_GridSize(gridSize), m_TileSize(tileSize), m_Layer(0)
	{

	}

	void TilemapComponent::Start() 
	{
		m_GridRenderer = (m_Parent->HasComponent<GridRendererComponent>()) 
			? &m_Parent->GetComponent<GridRendererComponent>()
			: &m_Parent->AddComponent<GridRendererComponent>(m_GridSize);
		m_GridRenderer->SetGridSize(m_GridSize);
	}

	void TilemapComponent::AddTile(const Vector2f worldPos, const int tileId)
	{
		Vector2f vec;
		int columns = m_Texture->GetWidth() / (int)m_GridSize.x;
		//int x = tileId % columns;
		int y = tileId / columns;
		vec.x = y * m_GridSize.x;
		vec.y = y * m_GridSize.y;
		m_Tiles[m_Layer][worldPos] = vec;
	}

	void TilemapComponent::AddTile(const Vector2f worldPos, const Vector2f tilePos)
	{
		m_Tiles[m_Layer][worldPos] = tilePos;
	}

	void TilemapComponent::RemoveTile(const Vector2f worldPos)
	{
		m_Tiles[m_Layer].erase(worldPos);
	}
}
