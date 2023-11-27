#include "SceneHierarchyView.h"

#include <Codex.h>

#include "../SceneEditorView.h"

namespace codex::editor {
    SceneHierarchyView::SceneHierarchyView(Scene& scene, SelectedEntityDescriptor& selectedEntity,
                                           Vector4f& selectColour)
        : m_Scene(scene), m_SelectedEntity(selectedEntity), m_SelectColour(selectColour)
    {
    }

    void SceneHierarchyView::OnImGuiRender()
    {
        {
            ImGui::Begin("Scene hierarchy");
            if (ImGui::Button("New entity"))
                ImGui::OpenPopup("new_entity_popup");
            if (ImGui::BeginPopup("new_entity_popup", ImGuiWindowFlags_MenuBar))
            {
                std::string name = fmt::format("Entity {}", m_Scene.GetEntityCount() + 1);
                ImGui::Text("Entity Name: ");
                ImGui::InputText("##entity_name", &name);
                ImGui::SameLine();
                if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::Button("Add")) // Check for Enter key press
                {
                    // Close the popup
                    ImGui::CloseCurrentPopup();
                    if (m_SelectedEntity.entity)
                    {
                        if (m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                            m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                                m_SelectedEntity.overlayColour;
                    }
                    m_SelectedEntity.entity = m_Scene.CreateEntity(name);
                }
                ImGui::EndPopup();
            }

            ImGui::Text("Entities:");
            auto entities = m_Scene.GetAllEntitiesWithComponent<TagComponent>();
            for (auto& e : entities)
            {
                auto& tag_component = e.GetComponent<TagComponent>();
                if (ImGui::Selectable(tag_component.tag.c_str(), m_SelectedEntity.entity == e))
                {
                    if (m_SelectedEntity.entity)
                    {
                        if (m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                        {
                            auto& s       = m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite();
                            s.GetColour() = m_SelectedEntity.overlayColour;
                        }
                    }

                    m_SelectedEntity.entity = e;

                    if (m_SelectedEntity.entity)
                    {
                        if (m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                        {
                            auto& s = m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite();
                            m_SelectedEntity.overlayColour = s.GetColour();
                            s.GetColour()                  = m_SelectColour;
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
