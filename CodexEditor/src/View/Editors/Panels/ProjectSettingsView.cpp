#include "ProjectSettingsView.h"

#include "../SceneEditorView.h"

namespace codex::editor {
    void ProjectSettingsView::OnInit()
    {
    }

    void ProjectSettingsView::OnImGuiRender()
    {
        ImGui::Begin("Project settings");

        auto d = this->GetDescriptor().Lock();

        // Physics settings.
        {
            if (ImGui::CollapsingHeader("Physics"))
            {
                auto& physics_props = d->activeScene.Lock()->GetPhysicsProperties();

                // Tick rate.
                {
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, 300);
                    ImGui::Text("Tick Rate");
                    ImGui::NextColumn();
                    ImGui::SliderInt("###tick_rate", reinterpret_cast<i32*>(&physics_props.tickRate), 0, 120);
                    ImGui::Columns(1);
                }

                // Velocity iterations.
                {
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, 300);
                    ImGui::Text("Velocity iterations");
                    ImGui::NextColumn();
                    ImGui::SliderInt("###vel_iter", reinterpret_cast<i32*>(&physics_props.velocityIterations), 0, 120);
                    ImGui::Columns(1);
                }

                // Position iterations.
                {
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, 300);
                    ImGui::Text("Position iterations");
                    ImGui::NextColumn();
                    ImGui::SliderInt("###pos_iter", reinterpret_cast<i32*>(&physics_props.positionIterations), 0, 120);
                    ImGui::Columns(1);
                }

                // Scaling factor.
                {
                    static f32 scaling_factor;

                    scaling_factor = 1.0f / physics_props.scalingFactor;

                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, 300);
                    ImGui::Text("Scaling factor");
                    ImGui::NextColumn();
                    ImGui::DragFloat("###scale_conv", &scaling_factor);
                    ImGui::Columns(1);

                    physics_props.scalingFactor = 1.0f / scaling_factor;
                }

                // Gravity.
                {
                    SceneEditorView::DrawVec2Control("Gravity", physics_props.gravity, d->columnWidth);
                }
            }
        }

        ImGui::End();
    }
} // namespace codex::editor
