#include <iostream>
#include <Codex.h>
#include <Engine/Core/EntryPoint.h>

class PlayerController : public codex::NativeBehaviour
{
private:
	codex::TransformComponent* trans = nullptr;
	codex::Sprite* sprite;
	codex::f32 velocity = 180.f;

public:
	void Init() override
	{
		using namespace codex;

		std::cout << "PlayerController initialized.\n";
		trans = &GetComponent<TransformComponent>();
		sprite = &GetComponent<codex::SpriteRendererComponent>().GetSprite();
	}
	void Update(const codex::f32 deltaTime) override
	{
		using namespace codex;

		if (KeyHandler::IsKeyDown(SDLK_w))
		{
			trans->position.y -= velocity * deltaTime;
		}
		else if (KeyHandler::IsKeyDown(SDLK_a))
		{
			trans->position.x -= velocity * deltaTime;
		}
		else if (KeyHandler::IsKeyDown(SDLK_s))
		{
			trans->position.y += velocity * deltaTime;
		}
		else if (KeyHandler::IsKeyDown(SDLK_d))
		{
			trans->position.x += velocity * deltaTime;
		}
	}
};

class TestGame : public codex::Application
{
private:
	codex::EditorScene* m_Scene = nullptr;
	codex::Entity m_Player = codex::Entity::None();

public:
	TestGame(const codex::ApplicationProperties& properties) : codex::Application(properties)
	{
		m_Scene = (codex::EditorScene*)m_Window->GetCurrentScene();
		m_Player = m_Scene->CreateEntity();

		auto tex = codex::Resources::Load<codex::Texture2D>("Sprites/machine.png");

		codex::Sprite sprite(tex);
		m_Player.AddComponent<codex::SpriteRendererComponent>(sprite);
		m_Player.GetComponent<codex::TransformComponent>().scale = { 0.05f, 0.05f, 0.0f };

		m_Player.AddComponent<codex::NativeBehaviourComponent>().Bind<PlayerController>();
	}

	~TestGame() override
	{

	}
};

codex::Application* codex::CreateApplication(const codex::ApplicationCLIArgs args)
{
    return new TestGame(codex::ApplicationProperties{
        .name             = "TestGame",
        .cwd              = "./",
        .args             = args,
        .windowProperties = codex::WindowProperties { .width = 800, .height = 600, .frameCap = 0, .vsync = true }
    });
}
