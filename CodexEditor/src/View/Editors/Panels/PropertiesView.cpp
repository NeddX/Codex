#include "PropertiesView.h"

#include "../SceneEditorView.h"

#include <tinyfiledialogs.h>

namespace codex::editor {
    using namespace codex::events;

    PropertiesView::PropertiesView(const mem::Ref<SceneEditorDescriptor>& editorDesc)
        : m_EditorDesc(editorDesc)
    {
    }
    void PropertiesView::OnImGuiRender()
    {
        auto d = m_EditorDesc.Lock();

        ImGui::Begin("Entity properties");
        if (d->selectedEntity.entity)
        {
            if (ImGui::Button("Add component"))
                ImGui::OpenPopup("component_popup");
            if (ImGui::BeginPopup("component_popup"))
            {
                if (ImGui::Button("Sprite Renderer Component"))
                {
                    d->selectedEntity.entity.AddComponent<SpriteRendererComponent>(Sprite::Empty());
                    ImGui::CloseCurrentPopup();
                }
                else if (ImGui::Button("C++ Script Component"))
                {
                    d->selectedEntity.entity.AddComponent<NativeBehaviourComponent>();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (d->selectedEntity.entity.HasComponent<TransformComponent>())
            {
                if (ImGui::TreeNodeEx("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& c = d->selectedEntity.entity.GetComponent<TransformComponent>();
                    SceneEditorView::DrawVec3Control("Position: ", c.position, d->columnWidth);
                    SceneEditorView::DrawVec3Control("Rotation: ", c.rotation, d->columnWidth);
                    SceneEditorView::DrawVec3Control("Scale: ", c.scale, d->columnWidth, 0.01f);

                    ImGui::TreePop();
                }
            }

            if (d->selectedEntity.entity.HasComponent<NativeBehaviourComponent>())
            {
                if (ImGui::TreeNodeEx("C++ Script Component", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& c = d->selectedEntity.entity.GetComponent<NativeBehaviourComponent>();

                    // Attached scripts.
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, d->columnWidth);
                    ImGui::Text("Attached behaviours: ");
                    ImGui::NextColumn();

                    if (ImGui::BeginCombo("###script_combo", "Select a script"))
                    {
                        for (const auto& file : d->scripts)
                        {
                            const auto& file_name = file.GetSourceFile().filename().string();

                            for (const auto& script : file.GetClasses())
                            {
                                const auto  script_name = script.name;
                                const auto& behaviours  = c.GetBehaviours();

                                const auto it =
                                    std::find_if(behaviours.cbegin(), behaviours.cend(),
                                                 [&script_name](const auto& e) { return e.first == script_name; });
                                if (it == behaviours.cend())
                                {
                                    if (ImGui::Selectable(script_name.c_str(), false))
                                    {
                                        auto* script = d->scene->CreateBehaviour(script_name.c_str());
                                        script->SetOwner(d->selectedEntity.entity);
                                        c.Attach(std::move(script));
                                    }
                                }
                            }
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::Columns(1);

                    // Display attached scripts and their serialized fields.
                    auto&                       behaviours = c.GetBehaviours();
                    std::list<std::string_view> possible_scripts_to_detach;
                    for (auto it = behaviours.begin(); it != behaviours.end(); ++it)
                    {
                        auto& [k, v] = *it;

                        const auto& j = v->GetSerializedData();
                        if (j.empty())
                            v->Serialize();

                        const auto& klass = j.begin();
                        if (ImGui::TreeNodeEx(klass.key().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            if (ImGui::Button("Detach script"))
                            {
                                possible_scripts_to_detach.push_back(it->first);
                                ImGui::TreePop();
                                continue;
                            }

                            if (klass.value().contains("Fields"))
                            {
                                for (const auto& field : klass.value()["Fields"].items())
                                {
                                    const std::string& field_name = field.key();
                                    const rf::RFType   field_type = field.value().at("Type");

                                    ImGui::Columns(2);
                                    ImGui::SetColumnWidth(0, d->columnWidth);
                                    ImGui::Text("%s", field_name.c_str());
                                    ImGui::NextColumn();

                                    object field_ptr = v->GetField(field_name);
                                    switch (field_type)
                                    {
                                        using enum rf::RFType;

                                        case I32:
                                        case U32: {
                                            ImGui::DragInt("##drag_int", (i32*)field_ptr);
                                            break;
                                        }
                                        case F32:
                                        case F64:
                                        case F128: {
                                            ImGui::DragFloat("##dragger", (f32*)field_ptr);
                                            break;
                                        }
                                        case StdString: {
                                            ImGui::InputText("##input", (std::string*)field_ptr);
                                            break;
                                        }
                                        case Boolean: {
                                            ImGui::Checkbox("##checkbox", (bool*)field_ptr);
                                            break;
                                        }
                                        case Vector2f: {
                                            SceneEditorView::DrawVec2Control("##t", *(math::Vector2f*)field_ptr,
                                                                             d->columnWidth);
                                            break;
                                        }
                                        case Vector3f: {
                                            SceneEditorView::DrawVec3Control("##t", *(math::Vector3f*)field_ptr,
                                                                             d->columnWidth);
                                            break;
                                        }
                                        default: break; // cx_throw(CodexException, "Should not happen."); break;
                                    }
                                    ImGui::Columns(1);
                                }
                            }
                            ImGui::TreePop();
                        }
                    }
                    for (const auto& e : possible_scripts_to_detach)
                        c.Detach(e);
                    ImGui::TreePop();
                }
            }

            if (d->selectedEntity.entity.HasComponent<SpriteRendererComponent>())
            {
                if (ImGui::TreeNodeEx("Sprite Renderer Component", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& c       = d->selectedEntity.entity.GetComponent<SpriteRendererComponent>();
                    auto& sprite  = c.GetSprite();
                    auto  texture = sprite.GetTexture();

                    // Texture prewview image
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, d->columnWidth);
                    ImGui::Text("Texture: ");
                    ImGui::NextColumn();

                    ImGui::BeginGroup();
                    if (sprite)
                        ImGui::Image((void*)(intptr)texture->GetGlId(), { 100.0f, 100.0f }, { 0, 1 }, { 1, 0 });
                    else
                        ImGui::Text("No bound texture.");

                    if (ImGui::Button("Load texture", { 100, 0 }))
                    {
                        const char* filters[] = { "*.png", "*.jpg" };
                        const char* file = tinyfd_openFileDialog("Load texture file.", nullptr, 2, filters, nullptr, 0);
                        if (file)
                        {
                            std::filesystem::path relative_path =
                                std::filesystem::relative(file, std::filesystem::current_path());
                            auto res = Resources::Load<gfx::Texture2D>(relative_path);
                            sprite.SetTexture(res);
                            d->selectedEntity.overlayColour = sprite.GetColour();
                            sprite.GetColour()              = d->selectColour;
                        }
                    }
                    ImGui::EndGroup();
                    ImGui::Columns(1);
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    // Texture coordinates
                    auto&    tex_coords = sprite.GetTextureCoords();
                    Vector2f pos        = { tex_coords.x, tex_coords.y };
                    Vector2f size       = { tex_coords.w, tex_coords.h };
                    SceneEditorView::DrawVec2Control("Texture position: ", pos, d->columnWidth);
                    SceneEditorView::DrawVec2Control("Texture size: ", size, d->columnWidth);
                    tex_coords = { pos.x, pos.y, size.x, size.y };

                    // Texture filter mode
                    if (texture)
                    {
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                        ImGui::Columns(2);
                        ImGui::SetColumnWidth(0, d->columnWidth);
                        ImGui::Text("Texture filter mode: ");
                        ImGui::NextColumn();
                        static int item_current_idx = 0; // Here we store our
                                                         // selection data as an
                                                         // index.
                        const char* preview_item = nullptr;
                        const auto& props        = texture->GetProperties();
                        switch (props.filterMode)
                        {
                            case gfx::TextureFilterMode::Linear: preview_item = "Linear"; break;
                            case gfx::TextureFilterMode::Nearest: preview_item = "Nearest"; break;
                        }
                        if (ImGui::BeginCombo("##texture_filter_mode", preview_item))
                        {
                            if (ImGui::Selectable("Nearest", props.filterMode == gfx::TextureFilterMode::Nearest))
                            {
                                if (props.filterMode != gfx::TextureFilterMode::Nearest)
                                {
                                    auto new_props       = props;
                                    new_props.filterMode = gfx::TextureFilterMode::Nearest;
                                    texture->New(texture->GetFilePath(), new_props);
                                }
                            }
                            if (ImGui::Selectable("Linear", props.filterMode == gfx::TextureFilterMode::Linear))
                            {
                                if (props.filterMode != gfx::TextureFilterMode::Linear)
                                {
                                    auto new_props       = props;
                                    new_props.filterMode = gfx::TextureFilterMode::Linear;
                                    texture->New(texture->GetFilePath(), new_props);
                                }
                            }
                            ImGui::EndCombo();
                        }
                        ImGui::Columns(1);
                    }

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    auto sprite_size = sprite.GetSize();
                    SceneEditorView::DrawVec2Control("Sprite size: ", sprite_size, d->columnWidth);
                    sprite.SetSize(sprite_size);

                    // Texture colour picker
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, d->columnWidth);
                    ImGui::Text("Sprite overlay: ");
                    ImGui::NextColumn();

                    auto&               colour = d->selectedEntity.overlayColour;
                    ImGuiColorEditFlags flags  = 0;
                    flags |= ImGuiColorEditFlags_AlphaBar;
                    flags |= ImGuiColorEditFlags_DisplayRGB; // Override display
                                                             // mode
                    f32 temp_colour[4]{ colour.x, colour.y, colour.z, colour.w };
                    ImGui::ColorPicker4("##color_picker", temp_colour, flags);
                    colour = { temp_colour[0], temp_colour[1], temp_colour[2], temp_colour[3] };
                    ImGui::Columns(1);

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, d->columnWidth);
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
} // namespace codex::editor
