#include <Codex.h>
#include <Engine/Core/EntryPoint.h>
#include <iostream>

class PlayerController : public codex::NativeBehaviour
{
private:
    codex::TransformComponent* trans = nullptr;
    codex::Sprite*             sprite;
    codex::f32                 velocity = 180.f;

public:
    void Init() override
    {
        using namespace codex;

        std::cout << "PlayerController initialized.\n";
        trans  = &GetComponent<TransformComponent>();
        sprite = &GetComponent<SpriteRendererComponent>().GetSprite();
    }
    void Update(const codex::f32 deltaTime) override
    {
        using namespace codex;

        if (KeyHandler::IsKeyDown(Key::W))
        {
            trans->position.y -= velocity * deltaTime;
        }
        if (KeyHandler::IsKeyDown(Key::A))
        {
            trans->position.x -= velocity * deltaTime;
        }
        if (KeyHandler::IsKeyDown(Key::S))
        {
            trans->position.y += velocity * deltaTime;
        }
        if (KeyHandler::IsKeyDown(Key::D))
        {
            trans->position.x += velocity * deltaTime;
        }
        if (KeyHandler::IsKeyDown(Key::Up))
        {
            trans->scale.x += 1.0f * deltaTime;
            trans->scale.y += 1.0f * deltaTime;
        }
        if (KeyHandler::IsKeyDown(Key::Down))
        {
            trans->scale.x -= 1.0f * deltaTime;
            trans->scale.y -= 1.0f * deltaTime;
        }
        if (KeyHandler::IsKeyDown(Key::P))
        {
            std::cout << "FPS: " << Application::GetFps() << std::endl;
        }
    }
};

class TestGame : public codex::Application
{
private:
    codex::EditorScene* m_Scene  = nullptr;
    codex::Entity       m_Player = codex::Entity::None();

public:
    TestGame(const codex::ApplicationProperties& properties) : codex::Application(properties)
    {
        m_Scene  = (codex::EditorScene*)m_Window->GetCurrentScene();
        m_Player = m_Scene->CreateEntity();

        auto tex = codex::Resources::Load<codex::Texture2D>("Sprites/machine.png");

        codex::Sprite sprite(tex);
        //codex::f32 scale_factor = 0.05f;
        //sprite.SetTextureCoords({ 0.0f, 0.0f, (codex::f32)sprite.GetWidth() * scale_factor, (codex::f32)sprite.GetHeight() * scale_factor, });
        m_Player.AddComponent<codex::SpriteRendererComponent>(sprite);

        m_Player.AddComponent<codex::NativeBehaviourComponent>().Bind<PlayerController>();
    }

    ~TestGame() override {}
};

codex::Application* codex::CreateApplication(const codex::ApplicationCLIArgs args)
{
    auto props = codex::WindowProperties{};
    props.width = 800;
    props.height = 600;
    props.frameCap = 0;
    props.vsync = true;
    return new TestGame(codex::ApplicationProperties{
        "TestGame",
        ". / ",
        args,
        props
    });
}
