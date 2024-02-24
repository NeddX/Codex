#include "Components.h"
#include "../Core/Application.h"
#include "../Core/Window.h"
#include "../Graphics/DebugDraw.h"
#include "ECS.h"

namespace codex {
    using namespace codex::graphics;

    TagComponent::TagComponent()
    {
        tag = "default tag";
    }

    TagComponent::TagComponent(const std::string_view tag) : tag(std::string(tag))
    {
    }

    TransformComponent::TransformComponent(const Vector3f position, const Vector3f rotation, const Vector3f scale)
        : position(position), rotation(rotation), scale(scale)
    {
    }

    SpriteRendererComponent::SpriteRendererComponent(Sprite sprite) : m_Sprite(std::move(sprite))
    {
    }

    GridRendererComponent::GridRendererComponent(const Vector2f gridSize) : m_GridSize(gridSize)
    {
        // m_Camera = Application::GetWindow().GetCurrentScene()->GetCamera();
    }

    void GridRendererComponent::Render()
    {
        Vector2f camera_pos = m_Camera->position;
        Vector2f proj_size  = { m_Camera->GetWidth(), m_Camera->GetHeight() };

        i32 first_x = (i32)(((i32)(camera_pos.x / m_GridSize.x) - 1) * m_GridSize.x);
        i32 first_y = (i32)(((i32)(camera_pos.y / m_GridSize.y) - 1) * m_GridSize.y);

        i32 rows = (i32)(proj_size.y / m_GridSize.y) + 2;
        i32 cols = (i32)(proj_size.x / m_GridSize.x) + 2;

        i32 width  = (i32)(proj_size.x + m_GridSize.x * 2);
        i32 height = (i32)(proj_size.y + m_GridSize.y * 2);

        Vector4f colour    = { 0.5f, 0.5f, 0.5f, 1.0f };
        i32      max_lines = std::max(rows, cols);
        for (i32 i = 0; i < max_lines; ++i)
        {
            i32 x = (i32)(m_GridSize.x * i + first_x), y = (i32)(m_GridSize.y * i + first_y);

            if (i <= cols)
                DebugDraw::DrawLine2D({ x, first_y }, { x, first_y + height }, colour);
            if (i <= rows)
                DebugDraw::DrawLine2D({ first_x, y }, { first_x + width, y }, colour);
        }
    }

    TilemapComponent::TilemapComponent(ResRef<Texture2D> texture, const Vector2f gridSize, const Vector2f tileSize)
        : m_Texture(texture), m_GridSize(gridSize), m_TileSize(tileSize), m_Layer(0)
    {
    }

    void TilemapComponent::Start()
    {
        m_GridRenderer = (m_Parent.HasComponent<GridRendererComponent>())
                             ? &m_Parent.GetComponent<GridRendererComponent>()
                             : &m_Parent.AddComponent<GridRendererComponent>(m_GridSize);
        m_GridRenderer->SetGridSize(m_GridSize);
    }

    void TilemapComponent::AddTile(const Vector2f worldPos, const i32 tileId)
    {
        Vector2f vec;
        i32      columns = m_Texture->GetWidth() / (i32)m_GridSize.x;
        // i32 x = tileId % columns;
        i32 y                      = tileId / columns;
        vec.x                      = y * m_GridSize.x;
        vec.y                      = y * m_GridSize.y;
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
} // namespace codex
