#include "SceneHierarchyView.h"

#include <Codex.h>

#include "../SceneEditorView.h"

namespace codex::editor {
    SceneHierarchyView::SceneHierarchyView(const Ref<SceneEditorDescriptor>& editorDesc) : m_EditorDesc(editorDesc)
    {
    }

    void SceneHierarchyView::OnImGuiRender()
    {
        {
            auto d = m_EditorDesc.Lock();

            ImGui::Begin("Scene hierarchy");
            if (ImGui::Button("New entity"))
                ImGui::OpenPopup("new_entity_popup");
            if (ImGui::BeginPopup("new_entity_popup", ImGuiWindowFlags_MenuBar))
            {
                std::string name = fmt::format("Entity {}", d->scene->GetEntityCount() + 1);
                ImGui::Text("Entity Name: ");
                ImGui::InputText("##entity_name", &name);
                ImGui::SameLine();
                if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::Button("Add")) // Check for Enter key press
                {
                    // Close the popup
                    ImGui::CloseCurrentPopup();
                    if (d->selectedEntity.entity)
                    {
                        if (d->selectedEntity.entity.HasComponent<SpriteRendererComponent>())
                            d->selectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                                d->selectedEntity.overlayColour;
                    }
                    d->selectedEntity.entity = d->scene->CreateEntity(name);
                }
                ImGui::EndPopup();
            }

            ImGui::Text("Entities:");
            auto entities = d->scene->GetAllEntitiesWithComponent<TagComponent>();
            for (auto& e : entities)
            {
                auto& tag_component = e.GetComponent<TagComponent>();
                if (ImGui::Selectable(tag_component.tag.c_str(), m_EditorDesc.Lock()->selectedEntity.entity == e))
                {
                    if (d->selectedEntity.entity)
                    {
                        if (d->selectedEntity.entity.HasComponent<SpriteRendererComponent>())
                        {
                            auto& s       = d->selectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite();
                            s.GetColour() = d->selectedEntity.overlayColour;
                        }
                    }

                    d->selectedEntity.entity = e;

                    if (d->selectedEntity.entity)
                    {
                        if (d->selectedEntity.entity.HasComponent<SpriteRendererComponent>())
                        {
                            auto& s = d->selectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite();
                            d->selectedEntity.overlayColour = s.GetColour();
                            s.GetColour()                  = d->selectColour;
                        }
                    }

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
                            if (ImGui::IsKeyPressed(ImGuiKey_Enter) ||
                                ImGui::Button("Enter")) // Check for Enter key press
                                ImGui::CloseCurrentPopup();
                            ImGui::EndPopup();
                        }
                    }
                }
            }
            ImGui::End();
        }
    }
} // namespace codex::editor
