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
    auto& io               = ImGui::GetIO();
    bool  show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    {
        ImGui::Begin("Render Info");
        ImGui::Text("Renderer information");
        ImGui::Text("Batch count: %zu", BatchRenderer2D::GeBatchCount());
        ImGui::Text("Total quad count: %zu", BatchRenderer2D::GetQuadCount());
        ImGui::End();
    }

    {
        ImGui::Begin("Scene hierarchy");
        if (ImGui::Button("New entity"))
            ImGui::OpenPopup("new_entity_popup");
        if (ImGui::BeginPopup("new_entity_popup", ImGuiWindowFlags_MenuBar))
        {
            std::string name = fmt::format("Entity {}", m_Scene->GetEntityCount() + 1);
            ImGui::Text("Entity Name: ");
            ImGui::InputText("##entity_name", &name);
            ImGui::SameLine();
            if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::Button("Add")) // Check for Enter key press
            {
                // Close the popup
                ImGui::CloseCurrentPopup();
                m_Scene->CreateEntity(name);
            }
            ImGui::EndPopup();
        }

        ImGui::Text("Entities:");
        auto entities = m_Scene->GetAllEntitiesWithComponent<TagComponent>();
        for (auto& e : entities)
        {
            auto& tag_component = e.GetComponent<TagComponent>();
            if (ImGui::Selectable(tag_component.tag.c_str(), m_SelectedEntity == e))
            {
                m_SelectedEntity = e;
                if (ImGui::IsMouseDoubleClicked(0))
                {
                    auto& t = e.GetComponent<TagComponent>();
                    ImGui::OpenPopup("new_name_popup");
                    if (ImGui::BeginPopup("new_name_popup", ImGuiWindowFlags_MenuBar))
                    {
                        ImGui::Text("New Name: ");
                        ImGui::SameLine();
                        ImGui::InputText("##entity_name", &t.tag);
                        ImGui::SameLine();
                        if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::Button("Enter")) // Check for Enter key press
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }
                }
            }
        }
        ImGui::End();
    }

    {
        ImGui::Begin("Entity properties");
        if (m_SelectedEntity)
        {
            if (ImGui::Button("Add component"))
                ImGui::OpenPopup("component_popup");
            if (ImGui::BeginPopup("component_popup"))
            {
                if (ImGui::Button("Sprite Renderer Component"))
                {
                    m_SelectedEntity.AddComponent<SpriteRendererComponent>(Sprite::Empty());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (m_SelectedEntity.HasComponent<TransformComponent>())
            {
                if (ImGui::TreeNodeEx("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& c = m_SelectedEntity.GetComponent<TransformComponent>();
                    DrawVec3Control("Position: ", c.position);
                    DrawVec3Control("Rotation: ", c.rotation);
                    DrawVec3Control("Scale: ", c.scale, 0.01f);

                    ImGui::TreePop();
                }
            }

            if (m_SelectedEntity.HasComponent<SpriteRendererComponent>())
            {
                if (ImGui::TreeNodeEx("Sprite Renderer Component", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& c      = m_SelectedEntity.GetComponent<SpriteRendererComponent>();
                    auto& sprite = c.GetSprite();

                    ImGui::Text("Texture: ");
                    ImGui::SameLine();

                    ImGui::BeginGroup();
                    if (sprite)
                        ImGui::Image((void*)(intptr)sprite.GetTexture()->GetGlId(), { 100.0f, 100.0f }, { 0, 1 },
                                     { 1, 0 });
                    else
                        ImGui::Text("No bound texture.");

                    if (ImGui::Button("Load texture", { 100, 0 }))
                    {
                        ImGui::OpenPopup("modal_reject");

                        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                        if (ImGui::BeginPopupModal("modal_reject", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            ImGui::Text("Opening files is not yet supported.");
                            ImGui::Separator();
                            if (ImGui::Button("Ok", { 120, 0 }))
                            {
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }
                    }
                    ImGui::EndGroup();

                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }
}

void EditorLayer::DrawVec3Control(const char* label, Vector3f& values, const f32 speed, const f32 resetValue,
                                  const f32 columnWidth)
{
    ImGuiIO& io        = ImGui::GetIO();
    auto     bold_font = io.Fonts->Fonts[0];

    ImGui::PushID(label);

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label);
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(bold_font);
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(bold_font);
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(bold_font);
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}
