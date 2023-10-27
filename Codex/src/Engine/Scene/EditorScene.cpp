#include "EditorScene.h"
#include "../Core/MouseHandler.h"
#include "../Core/Window.h"
#include "../Renderer/DebugDraw.h"
#include "../Scene/Components.h"
#include "ECS.h"

namespace codex {
    static std::unique_ptr<SpriteSheet> sp        = nullptr;
    static ResRef<Texture2D>            m_TestTex = nullptr;
    static Entity                       ent((entt::entity)0, nullptr);

    bool     isDragging = false;
    Vector2f mouseStart;

    EditorScene::EditorScene(Renderer* renderer, const i32 width, const i32 height) : Scene(renderer, width, height)
    {
        // TODO: Make Resources to be a singleton sub system that can be initialized and disposed
        // so that we can dispose all of its resources from the Window class from the corecrl thread.
        m_Shader              = Resources::Load<Shader>("GLShaders/batchRenderer.glsl");
        m_CurrentActiveAction = EditorAction::Select;
    }

    EditorScene::~EditorScene()
    {
    }

    void EditorScene::Start()
    {
        // auto grid = m_EditorSceneManager.CreateEntity("grid_renderer_entity");
        // grid.AddComponent<GridRendererComponent>(Vector2f { 32.0f, 32.0f });

        auto tile_preview = CreateEntity("tile_preview_entity");
        tile_preview.AddComponent<SpriteRendererComponent>(Vector4f{ 1.0f, 1.0f, 1.0f, 1.0f });

        m_SelectedTileCoord = { 0.0f, 0.0f };
        // this will create the batch renderer if it doesn't exist or it will return the poi32er to it if it already
        // exists
        m_SpriteBatch = m_Renderer->GetSpriteBatchRenderer(m_Shader.get());

#ifdef CX_MODE_STANDALONE
        fmt::println("creating ice_cube entity.");
        auto   ent    = CreateEntity();
        Sprite sprite = { Resources::Load<Texture2D>("Sprites/ice_cube.jpg") };
        ent.AddComponent<SpriteRendererComponent>(sprite);

        auto  ent1       = CreateEntity();
        auto& trans      = ent1.GetComponent<TransformComponent>();
        trans.position.x = 400;
        trans.position.y = 300;
        ent1.AddComponent<SpriteRendererComponent>(Sprite{ Resources::Load<Texture2D>("Sprites/dun_tileset.png") });
#endif
    }

    void EditorScene::Update(const f32 deltaTime)
    {
        // TODO: Improve.
        static f32      speed = 1.6f;
        static Vector2f previous_mouse_pos;
        if (MouseHandler::IsMouseDown(Mouse::RightMouse))
        {
            if (MouseHandler::IsDragging())
            {
                if (!isDragging)
                {
                    mouseStart = MouseHandler::GetMousePos();
                    isDragging = true;
                }
                if (auto mouse_pos = MouseHandler::GetMousePos(); mouse_pos != previous_mouse_pos && isDragging)
                {
                    GetCamera()->position +=
                        (speed * Vector2f(MouseHandler::GetMouseDeltaX(), MouseHandler::GetMouseDeltaY()));
                }
            }
            else
                isDragging = false;
        }
        else
            isDragging = false;

        switch (m_CurrentActiveAction)
        {
            case EditorAction::TilemapBrush: {
                if (!MouseHandler::IsMouseDown(Mouse::LeftMouse))
                    break;

                auto mouse = GetMousePositionInWorld();
                auto ent   = Entity(m_SelectedEntityId, this);

                if (ent.HasComponent<TilemapComponent>())
                {
                    auto&    tilemap_component = ent.GetComponent<TilemapComponent>();
                    auto     grid_size         = tilemap_component.GetGridSize();
                    Vector2f snapped{ (i32)(mouse.x / grid_size.x) * grid_size.x,
                                      (i32)(mouse.y / grid_size.y) * grid_size.y };

                    fmt::println("\rAdded tile at: {} in layer: {}", snapped, tilemap_component.GetLayer());
                    tilemap_component.AddTile(snapped, m_SelectedTileCoord);
                }
                break;
            }
            case EditorAction::TilemapErase: {
                if (!MouseHandler::IsMouseDown(Mouse::LeftMouse))
                    break;

                auto mouse = GetMousePositionInWorld();
                auto ent   = Entity(m_SelectedEntityId, this);

                if (ent.HasComponent<TilemapComponent>())

                {
                    auto&    tilemap_component = ent.GetComponent<TilemapComponent>();
                    auto     grid_size         = tilemap_component.GetGridSize();
                    Vector2f snapped{ (i32)(mouse.x / grid_size.x) * grid_size.x,
                                      (i32)(mouse.y / grid_size.y) * grid_size.y };

                    fmt::println("Removed tile at: {} in layer: {}", snapped, tilemap_component.GetLayer());
                    tilemap_component.RemoveTile(snapped);
                }
                break;
            }
            default: break;
        }
    }

    void EditorScene::Render(const f32 deltaTime)
    {
        // Render
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
        m_Shader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());

        m_SpriteBatch->Begin();
        // m_SpriteBatch->BindShader(m_Shader.get());

        // Render editor scene specific entities
        for (auto& entity : GetAllEntitiesWithComponent<GridRendererComponent>())
        {
            auto& grid_renderer = entity.GetComponent<GridRendererComponent>();
            grid_renderer.Render();
        }

        {
            /* // TODO: Grab the sprite from current active tilemap component.
            auto& sprite = m_EditorSceneManager.GetAllEntitesWithTag("tile_preview_entity")[0]
                .GetComponent<SpriteRendererComponent>();
            sprite.SetSprite(Sprite())
            */
        }

        // Render entities/components here
        for (auto& entity : GetAllEntitiesWithComponent<SpriteRendererComponent>())
        {
            auto& transform_component = entity.GetComponent<TransformComponent>();
            auto& renderer_component  = entity.GetComponent<SpriteRendererComponent>();
            auto& sprite              = renderer_component.GetSprite();
            Rect  tex_coords          = sprite.GetTextureCoords();
            m_SpriteBatch->RenderRect(sprite.GetTexture().get(), tex_coords,
                                      {
                                          transform_component.position.x,
                                          transform_component.position.y,
                                          tex_coords.w * transform_component.scale.x,
                                          tex_coords.h * transform_component.scale.y,
                                      },
                                      renderer_component.GetColour(), renderer_component.GetZIndex(),
                                      (i32)entity.GetId());
        }

        for (auto& entity : GetAllEntitiesWithComponent<NativeBehaviourComponent>())
        {
            // TODO: This should happen OnScenePlay.
            auto& behaviour_component = entity.GetComponent<NativeBehaviourComponent>();
            if (!behaviour_component.instance)
            {
                behaviour_component.instance          = behaviour_component.Instantiate();
                behaviour_component.instance->m_Owner = entity;
                behaviour_component.instance->Init();
            }
            behaviour_component.instance->Update(deltaTime);
        }

        for (auto& entity : GetAllEntitiesWithComponent<TilemapComponent>())
        {
            // auto& transform_component = entity.GetComponent<TransformComponent>();
            auto& tilemap_component = entity.GetComponent<TilemapComponent>();
            auto& grid_component    = entity.GetComponent<GridRendererComponent>();
            if (entity.GetId() == m_SelectedEntityId)
                grid_component.Render();
            for (const auto& [key, value] : tilemap_component.GetAllTiles())
            {
                for (const auto& t : value)
                {
                    auto src_rect  = Rect{ (i32)t.second.x, (i32)t.second.y, (i32)tilemap_component.GetTileSize().x,
                                          (i32)tilemap_component.GetTileSize().y };
                    auto dest_rect = Rect{ (i32)t.first.x, (i32)t.first.y, (i32)tilemap_component.GetGridSize().x,
                                           (i32)tilemap_component.GetGridSize().y };

                    m_SpriteBatch->RenderRect(tilemap_component.GetTexture().get(), src_rect, dest_rect,
                                              Vector4f{ 1.0f, 1.0f, 1.0f, 1.0f }, key, (i32)entity.GetId());
                }
            }
        }
        m_SpriteBatch->End();
    }
} // namespace codex
