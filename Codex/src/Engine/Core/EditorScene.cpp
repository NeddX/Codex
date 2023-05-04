#include "EditorScene.h"
#include "../Core/MouseHandler.h"

namespace Codex
{
	static std::unique_ptr<SpriteSheet> sp = nullptr;
	static std::shared_ptr<Texture2D> m_TestTex = nullptr;
	static Entity ent((entt::entity)0, nullptr);

	EditorScene::EditorScene(Renderer* renderer, int width, int height) : Scene(renderer, width, height)
	{
		m_Shader = Resources::Load<Shader>("batcher.glsl");
	}

	EditorScene::~EditorScene()
	{

	}

	void EditorScene::Start()
	{
		// this will create the batch renderer if it doesn't exist or it will return the pointer to it if it already exists
		m_SpriteBatch = m_Renderer->GetSpriteBatchRenderer(m_Shader.get());

		m_TestTex = Resources::Load<Texture2D>("dun_tileset.png");
		sp = std::make_unique<SpriteSheet>(m_TestTex, 16, 16, (m_TestTex->GetWidth() / 16) * (m_TestTex->GetHeight() / 16), 0);

		ent = m_Manager.CreateEntity();
		auto& a_trans = ent.GetComponent<TransformComponent>();
		a_trans.position = { 9.0f, 0.0f, 0.0f };
		a_trans.scale = { 5.0f, 5.0f, 0.0f };
		ent.AddComponent<SpriteRendererComponent>(sp->GetSpriteAt(377, 117)).SetZIndex(1);
	}

	void EditorScene::Update(float deltaTime)
	{
		// Logic update
		/*std::for_each(m_Entities.begin(),
			std::remove_if(m_Entities.begin(), m_Entities.end(),
				[](auto* e)
				{
					return !e->GetActive();
				}),
			[deltaTime](auto* e)
			{
				e->Update(deltaTime);
			});*/

		// Render
		m_Renderer->SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);
		m_Renderer->Clear();

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
		m_Shader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());

		m_SpriteBatch->Begin(); 
		
		// Render entities/components here
		auto entities = m_Manager.GetAllEntitiesWithComponent<SpriteRendererComponent>();
		for (auto& entity : entities)
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
				renderer_component.GetZIndex()
				);
		}

		m_SpriteBatch->End();
	}
}
