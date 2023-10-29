#pragma once

#include <Codex.h>

using namespace codex;

class ImGuiEditor : public Layer
{
private:
    std::unique_ptr<Scene>  m_Scene       = nullptr;
    Entity                  m_Player      = Entity::None();
    ResRef<Shader>          m_BatchShader = nullptr;
    std::unique_ptr<Camera> m_Camera      = nullptr;

public:
    void OnAttach() override
    {
        auto width  = Application::GetWindow().GetWidth();
        auto height = Application::GetWindow().GetHeight();
        m_Scene  = std::make_unique<Scene>(Application::GetWindow().GetWidth(), Application::GetWindow().GetHeight());
        m_Player = m_Scene->CreateEntity();
        m_BatchShader = Resources::Load<Shader>("GLShaders/batchRenderer.glsl");
        m_Camera      = std::make_unique<Camera>(width, height);

        Renderer::GetSpriteBatchRenderer(m_BatchShader.get());

        auto tex = Resources::Load<Texture2D>("Sprites/machine.png");

        Sprite sprite(tex);
        // codex::f32 scale_factor = 0.05f;
        // sprite.SetTextureCoords({ 0.0f, 0.0f, (codex::f32)sprite.GetWidth() * scale_factor,
        // (codex::f32)sprite.GetHeight() * scale_factor, });
        m_Player.AddComponent<SpriteRendererComponent>(sprite);
        auto& res = m_Player.GetComponent<TransformComponent>().scale;
        res.x     = 0.05f;
        res.y     = 0.05f;

        // m_Player.AddComponent<NativeBehaviourComponent>().Bind<PlayerController>();
    }

    void Update(const f32 deltaTime) override
    {
        m_BatchShader->Bind();
        m_BatchShader->Bind();
        m_BatchShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
        m_BatchShader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());

        Renderer::Begin();
        m_Scene->Update(deltaTime);
        Renderer::End();
    }

    void ImGuiRender() override
    {
        auto&  io                  = ImGui::GetIO();
        bool   show_demo_window    = true;
        bool   show_another_window = false;
        ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f       = 0.0f;
            static int   counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_demo_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button(
                    "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window",
                         &show_another_window); // Pass a pointer to our bool variable (the window will have a closing
                                                // button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    }
};
