#include <Codex.h>
#include <Engine/Core/EntryPoint.h>
#include <iostream>

#include "include/ImGuiEditor.h"
#include "include/PlayerController.h"

class TestGame : public codex::Application
{
private:
    codex::Entity m_Player = codex::Entity::None();

public:
    TestGame(const codex::ApplicationProperties& properties) : codex::Application(properties)
    {
        /*
        m_Scene  = (codex::EditorScene*)m_Window->GetCurrentScene();
        m_Player = m_Scene->CreateEntity();

        auto tex = codex::Resources::Load<codex::Texture2D>("Sprites/machine.png");

        codex::Sprite sprite(tex);
        // codex::f32 scale_factor = 0.05f;
        // sprite.SetTextureCoords({ 0.0f, 0.0f, (codex::f32)sprite.GetWidth() * scale_factor,
        // (codex::f32)sprite.GetHeight() * scale_factor, });
        m_Player.AddComponent<codex::SpriteRendererComponent>(sprite);
        auto& res = m_Player.GetComponent<codex::TransformComponent>().scale;
        res.x     = 0.05f;
        res.y     = 0.05f;

        m_Player.AddComponent<codex::NativeBehaviourComponent>().Bind<PlayerController>();
        */

        PushLayer(new ImGuiEditor());
    }

    ~TestGame() override {}
};

codex::Application* codex::CreateApplication(const codex::ApplicationCLIArgs args)
{
    return new TestGame(codex::ApplicationProperties{
        .name             = "TestGame",
        .cwd              = "./",
        .args             = args,
        .windowProperties = { .width = 800, .height = 600, .frameCap = 0, .vsync = false } });
}
