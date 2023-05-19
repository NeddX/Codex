#include "EditorScene.h"
#include "../Core/MouseHandler.h"
#include "../Renderer/DebugDraw.h"

namespace Codex {
	static std::unique_ptr<SpriteSheet> sp = nullptr;
	static std::shared_ptr<Texture2D> m_TestTex = nullptr;
	static Entity ent((entt::entity)0, nullptr);

	bool isDragging = false;
	Vector2f mouseStart;

	EditorScene::EditorScene(Renderer* renderer, int width, int height) : Scene(renderer, width, height)
	{
		m_Shader = Resources::Load<Shader>("batchRenderer.glsl");
		m_CurrentActiveAction = EditorAction::TilemapBrush;
	}

	EditorScene::~EditorScene()
	{

	}

	void EditorScene::Start()
	{
		auto grid = m_EditorSceneManager.CreateEntity("grid_renderer_entity");
		grid.AddComponent<GridRendererComponent>(Vector2f { 32.0f, 32.0f });

		auto tile_preview = m_EditorSceneManager.CreateEntity("tile_preview_entity");
		tile_preview.AddComponent<SpriteRendererComponent>(Vector4f { 1.0f, 1.0f, 1.0f, 1.0f });

		m_SelectedTileCoord = { 0.0f, 0.0f };
		// this will create the batch renderer if it doesn't exist or it will return the pointer to it if it already exists
		m_SpriteBatch = m_Renderer->GetSpriteBatchRenderer(m_Shader.get());

#ifdef CFX_MODE_STANDALONE
		auto ent = m_Manager.CreateEntity();
		
#endif

		/*
		m_TestTex = Resources::Load<Texture2D>("dun_tileset.png");
		sp = std::make_unique<SpriteSheet>(m_TestTex, 16, 16, (m_TestTex->GetWidth() / 16) * (m_TestTex->GetHeight() / 16), 0);

		ent = m_Manager.CreateEntity();
		auto& a_trans = ent.GetComponent<TransformComponent>();
		a_trans.position = { 9.0f, 0.0f, 0.0f };
		a_trans.scale = { 5.0f, 5.0f, 0.0f };
		ent.AddComponent<SpriteRendererComponent>(sp->GetSpriteAt(377, 117)).SetZIndex(1);*/
	}

	void EditorScene::Update(float deltaTime)
	{
		// TODO: Improve.
		switch (m_CurrentActiveAction)
		{
			case EditorAction::Select:
			{
				if (MouseHandler::IsMouseDown(2))
				{
					if (MouseHandler::IsDragging())
					{
						if (!isDragging)
						{
							mouseStart = MouseHandler::GetMousePos();
							isDragging = true;
						}
						if (isDragging)
						{
							auto mouse_pos = MouseHandler::GetMousePos();
							auto displacement = mouse_pos - mouseStart;
							GetCamera()->position -= (displacement * deltaTime);
						}
					}
					else
						isDragging = false;
				}
				else 
					isDragging = false;
				break;
			}
			case EditorAction::TilemapBrush:
			{
				if (!MouseHandler::IsMouseDown(0)) break;

				auto mouse = GetMousePositionInWorld();
				auto ent = Entity(0, &m_Manager);

				if (ent.HasComponent<TilemapComponent>())
				{
					auto& tilemap_component = ent.GetComponent<TilemapComponent>();
					auto grid_size = tilemap_component.GetGridSize();
					Vector2f snapped
					{
						mouse.x - ((int)mouse.x % (int)grid_size.x),
						mouse.y - ((int)mouse.y % (int)grid_size.y)
					};

					fmt::print("\rAdded tile at: {} in layer: {}      ", snapped, tilemap_component.GetLayer());
					tilemap_component.AddTile(snapped, m_SelectedTileCoord);
				}
				break;
			}
			case EditorAction::TilemapErase:
			{
				if (!MouseHandler::IsMouseDown(0)) break;

				auto mouse = GetMousePositionInWorld();
				auto ent = Entity(0, &m_Manager);

				if (ent.HasComponent<TilemapComponent>())
				{
					auto& tilemap_component = ent.GetComponent<TilemapComponent>();
					auto grid_size = tilemap_component.GetGridSize();
					Vector2f snapped
					{
						mouse.x - ((int)mouse.x % (int)grid_size.x),
						mouse.y - ((int)mouse.y % (int)grid_size.y)
					};

					fmt::print("\rRemoved tile at: {} in layer: {}      ", snapped, tilemap_component.GetLayer());
					tilemap_component.RemoveTile(snapped);
				}
				break;
			}
		}

		// Render
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
		m_Shader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());

		m_SpriteBatch->Begin(); 
		
		// Render editor scene specific entities
		for (auto& entity : m_EditorSceneManager.GetAllEntitiesWithComponent<GridRendererComponent>())
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
		for (auto& entity : m_Manager.GetAllEntitiesWithComponent<SpriteRendererComponent>())
		{
			auto& transform_component = entity.GetComponent<TransformComponent>();
			auto& renderer_component = entity.GetComponent<SpriteRendererComponent>();
			auto& sprite = renderer_component.GetSprite();
			Rect tex_coords = sprite.GetTextureCoords();
			m_SpriteBatch->RenderRect(
				sprite.GetTexture().get(),
				tex_coords,
				{
					transform_component.position.x,
					transform_component.position.y,
					tex_coords.w * transform_component.scale.x,
					tex_coords.h * transform_component.scale.y,
				},
				renderer_component.GetColour(),
				renderer_component.GetZIndex()
				);
		}

		for (auto& entity : m_Manager.GetAllEntitiesWithComponent<TilemapComponent>())
		{
			auto& transform_component = entity.GetComponent<TransformComponent>();
			auto& tilemap_component = entity.GetComponent<TilemapComponent>();
			for (const auto& [key, value] : tilemap_component.GetAllTiles())
			{
				for (const auto& t : value)
				{
					auto src_rect = Rect{ (int)t.second.x,		(int)t.second.y, (int)tilemap_component.GetTileSize().x, (int)tilemap_component.GetTileSize().y };
					auto dest_rect = Rect 
					{ 
						(int)t.first.x,		
						(int)t.first.y,  
						(int)tilemap_component.GetGridSize().x, 
						(int)tilemap_component.GetGridSize().y 
					};

					m_SpriteBatch->RenderRect(
						tilemap_component.GetTexture().get(),
						src_rect,
						dest_rect,
						Vector4f { 1.0f, 1.0f, 1.0f, 1.0f },
						key
					);
				}
			}
		}
		m_SpriteBatch->End();
	}
}
