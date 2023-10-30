#include "EditorLayer.h"

#include <tinyfiledialogs.h>

void EditorLayer::OnAttach()
{
    auto width    = Application::GetWindow().GetWidth();
    auto height   = Application::GetWindow().GetHeight();
    m_Scene       = std::make_unique<Scene>(Application::GetWindow().GetWidth(), Application::GetWindow().GetHeight());
    m_BatchShader = Resources::Load<Shader>("GLShaders/batchRenderer.glsl");
    m_Camera      = std::make_unique<Camera>(width, height);

    Renderer::Init(width, height);
    BatchRenderer2D::BindShader(m_BatchShader.get());

    mgl::FrameBufferProperties props;
    props.attachments = { mgl::TextureFormat::RGBA8 };
    props.width       = width;
    props.height      = height;
    m_Framebuffer     = std::make_unique<mgl::FrameBuffer>(props);
}

void EditorLayer::OnDetach()
{
    BatchRenderer2D::Destroy();
    Renderer::Destroy();
}

void EditorLayer::Update(const f32 deltaTime)
{
    m_BatchShader->Bind();
    m_BatchShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
    m_BatchShader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());

    m_Framebuffer->Bind();
    Renderer::SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);
    Renderer::Clear();
    BatchRenderer2D::Begin();
    m_Scene->Update(deltaTime);
    BatchRenderer2D::End();
    m_Framebuffer->Unbind();
}

void EditorLayer::ImGuiRender()
{
    auto& io               = ImGui::GetIO();
    bool  show_demo_window = true;

    // Enable docking on the main window.
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::ShowDemoWindow(&show_demo_window);

    // Engine viewport
    {
        ImGui::Begin("Viewport");
        static ImVec2 viewport_window_size;
        ImVec2        current_viewport_window_size = ImGui::GetWindowSize();
        if (viewport_window_size.x != current_viewport_window_size.x ||
            viewport_window_size.y != current_viewport_window_size.y)
        {
            viewport_window_size = current_viewport_window_size;
            m_Camera->SetWidth((i32)viewport_window_size.x);
            m_Camera->SetHeight((i32)viewport_window_size.y);
        }
        ImGui::Image((void*)(intptr)m_Framebuffer->GetColourAttachmentIdAt(0), viewport_window_size, { 0, 1 },
                     { 1, 0 });
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
                m_SelectedEntity = m_Scene->CreateEntity(name);
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
                    DrawVec3Control("Position: ", c.position, m_ColumnWidth);
                    DrawVec3Control("Rotation: ", c.rotation, m_ColumnWidth);
                    DrawVec3Control("Scale: ", c.scale, m_ColumnWidth, 0.01f);

                    ImGui::TreePop();
                }
            }

            if (m_SelectedEntity.HasComponent<SpriteRendererComponent>())
            {
                if (ImGui::TreeNodeEx("Sprite Renderer Component", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& c      = m_SelectedEntity.GetComponent<SpriteRendererComponent>();
                    auto& sprite = c.GetSprite();

                    // Texture prewview image
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, m_ColumnWidth);
                    ImGui::Text("Texture: ");
                    ImGui::NextColumn();

                    ImGui::BeginGroup();
                    if (sprite)
                        ImGui::Image((void*)(intptr)sprite.GetTexture()->GetGlId(), { 100.0f, 100.0f }, { 0, 1 },
                                     { 1, 0 });
                    else
                        ImGui::Text("No bound texture.");

                    if (ImGui::Button("Load texture", { 100, 0 }))
                    {
                        const char* filters[] = { "*.png", "*.jpg" };
                        const char* file = tinyfd_openFileDialog("Load texture file.", nullptr, 2, filters, nullptr, 0);
                        if (file)
                        {
                            auto res = Resources::Load<Texture2D>(file);
                            sprite.SetTexture(res);
                        }
                    }
                    ImGui::EndGroup();
                    ImGui::Columns(1);
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    // Texture coordinates

                    auto&    tex_coords = sprite.GetTextureCoords();
                    Vector2f pos        = { tex_coords.x, tex_coords.y };
                    Vector2f size       = { tex_coords.w, tex_coords.h };
                    DrawVec2Control("Texture position: ", pos, m_ColumnWidth);
                    DrawVec2Control("Texture size: ", size, m_ColumnWidth);
                    tex_coords = { pos.x, pos.y, size.x, size.y };

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    auto sprite_size = sprite.GetSize();
                    DrawVec2Control("Sprite size: ", sprite_size, m_ColumnWidth);
                    sprite.SetSize(sprite_size);

                    // Texture colour picker
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, m_ColumnWidth);
                    ImGui::Text("Sprite overlay: ");
                    ImGui::NextColumn();

                    auto&               colour = sprite.GetColour();
                    ImGuiColorEditFlags flags  = 0;
                    flags |= ImGuiColorEditFlags_AlphaBar;
                    flags |= ImGuiColorEditFlags_DisplayRGB; // Override display mode
                    f32 temp_colour[4]{ colour.x, colour.y, colour.z, colour.w };
                    ImGui::ColorPicker4("##color_picker", temp_colour, flags);
                    colour = { temp_colour[0], temp_colour[1], temp_colour[2], temp_colour[3] };
                    ImGui::Columns(1);

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, m_ColumnWidth);
                    ImGui::Text("Z Index: ");
                    ImGui::NextColumn();
                    ImGui::DragInt("##drag_int", &sprite.GetZIndex());
                    ImGui::Columns(1);

                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }
}

void EditorLayer::DrawVec3Control(const char* label, Vector3f& values, const f32 columnWidth, const f32 speed,
                                  const f32 resetValue)
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

void EditorLayer::DrawVec2Control(const char* label, Vector2f& values, const f32 columnWidth, const f32 speed,
                                  const f32 resetValue)
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

    f32    lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
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

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}
