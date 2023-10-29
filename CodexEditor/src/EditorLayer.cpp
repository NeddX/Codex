#include "EditorLayer.h"

void EditorLayer::OnAttach()
{
    auto width    = Application::GetWindow().GetWidth();
    auto height   = Application::GetWindow().GetHeight();
    m_Scene       = std::make_unique<Scene>(Application::GetWindow().GetWidth(), Application::GetWindow().GetHeight());
    m_Player      = m_Scene->CreateEntity();
    m_BatchShader = Resources::Load<Shader>("GLShaders/batchRenderer.glsl");
    m_Camera      = std::make_unique<Camera>(width, height);

    BatchRenderer2D::BindShader(m_BatchShader.get());

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

void EditorLayer::OnDetach()
{
    BatchRenderer2D::Destroy();
}

void EditorLayer::Update(const f32 deltaTime)
{
    m_BatchShader->Bind();
    m_BatchShader->Bind();
    m_BatchShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
    m_BatchShader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());

    BatchRenderer2D::Begin();
    m_Scene->Update(deltaTime);
    BatchRenderer2D::End();
}

void EditorLayer::ImGuiRender()
{
    auto&  io                  = ImGui::GetIO();
    bool   show_demo_window    = true;
    bool   show_another_window = false;
    ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui::ShowDemoWindow(&show_demo_window);

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

    {
        ImGui::Begin("Render Info");
        ImGui::Text("Renderer information");
        ImGui::Text("Batch count: %zu", BatchRenderer2D::GeBatchCount());
        ImGui::Text("Total quad count: %zu", BatchRenderer2D::GetQuadCount());
        ImGui::End();
    }

    {
        auto& c            = m_Player.GetComponent<TransformComponent>();
        float pos_arr[3]   = { c.position.x, c.position.y, c.position.z };
        float scale_arr[3] = { c.scale.x, c.scale.y, c.scale.z };
        ImGui::Begin("Entity edit panel");
        ImGui::DragFloat3("Entity Position", pos_arr, 1.0f, -1000.0f, 1000.0f);
        ImGui::DragFloat3("Entity Scale", scale_arr, 0.01f, -1000.0f, 1000.0f);
        c.position.x = pos_arr[0];
        c.position.y = pos_arr[1];
        c.position.z = pos_arr[2];
        c.scale.x    = scale_arr[0];
        c.scale.y    = scale_arr[1];
        c.scale.z    = scale_arr[2];
        ImGui::End();
    }
    {
        ImGui::Begin("Scene hierarchy");

        if (ImGui::Button("New entity"))
        {
            ImGui::OpenPopup("new_entity_popup");
            if (ImGui::BeginPopup("new_entity_popup"))
            {
                ImGui::EndPopup();
            }
        }

        ImGui::Text("Entities:");
        auto entities = m_Scene->GetAllEntitiesWithComponent<TagComponent>();
        for (auto& e : entities)
        {
            auto& tag_component = e.GetComponent<TagComponent>();
            if (ImGui::TreeNode(tag_component.tag.c_str()))
            {
                ImGui::Text("Selected");
                m_SelectedEntity = e;
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}
