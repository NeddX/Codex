#include "SceneHierarchyView.h"

#include <Codex.h>

#include "../SceneEditorView.h"

namespace codex::editor {
    void SceneHierarchyView::OnInit()
    {
    }

    void SceneHierarchyView::OnImGuiRender()
    {
        {
            auto d     = this->GetDescriptor().Lock();
            auto scene = d->activeScene.Lock();

            ImGui::Begin("Scene hierarchy");
            if (ImGui::Button("New entity"))
                ImGui::OpenPopup("new_entity_popup");
            if (ImGui::BeginPopup("new_entity_popup"))
            {
                auto name = fmt::format("Entity {}", scene->GetEntityCount() + 1);
                ImGui::Text("Entity Name");
                ImGui::SameLine();
                ImGui::InputText("##entity_name", &name);
                ImGui::SameLine();
                if (ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_Enter) ||
                    ImGui::Button("Add")) // Check for Enter key press
                {
                    // Close the popup
                    ImGui::CloseCurrentPopup();
                    d->selectedEntity.Select(scene->CreateEntity(name));
                }
                ImGui::EndPopup();
            }

            ImGui::Text("Entities");
            static auto action_delete = false;
            static auto action_rename = false;
            auto entities = scene->GetAllEntitiesWithComponent<TagComponent>();
            for (auto& e : entities)
            {
                auto& tag_component = e.GetComponent<TagComponent>();
                if (ImGui::Selectable((tag_component.tag + "##entity").c_str(), d->selectedEntity.entity == e,
                                      ImGuiSelectableFlags_DontClosePopups))
                {
                    d->selectedEntity.Select(e);
                }

                if (ImGui::BeginPopupContextItem())
                {
                    d->selectedEntity.Select(e);

                    if (ImGui::MenuItem("Rename"))
                        action_rename = true;
                    if (ImGui::MenuItem("Delete"))
                        action_delete = true;

                    ImGui::EndPopup();
                }
            }

            if (action_rename)
                ImGui::OpenPopup("rename_popup");
            if (ImGui::BeginPopup("rename_popup"))
            {
                ImGui::Text("New Name");
                ImGui::SameLine();
                ImGui::InputText("##entity_name", &d->selectedEntity.entity.GetComponent<TagComponent>().tag);
                ImGui::SameLine();
                if (ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_Enter) ||
                    ImGui::Button("Enter")) // Check for Enter key
                {
                    ImGui::CloseCurrentPopup();
                    action_rename = false;
                }
                ImGui::EndPopup();
            }
            else if (action_delete)
            {
                d->selectedEntity.Deselect();
                scene->RemoveEntity(d->selectedEntity.entity);
                ImGui::CloseCurrentPopup();
                action_delete = false;
            }

            ImGui::End();
        }
    }
} // namespace codex::editor
