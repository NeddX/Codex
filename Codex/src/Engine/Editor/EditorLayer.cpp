#include "EditorLayer.h"
#include "../Core/Scene.h"

namespace Codex {
    EditorLayer::EditorLayer(const char* name, Renderer* renderer) : Layer(name), m_Renderer(renderer)
    {

    }

    void EditorLayer::OnAttach()
    {

    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::Update(float deltaTime, Scene* scene)
    {
        //scene->SceneEditor();
    }

    void EditorLayer::Render(float deltaTime)
    {

    }

    void EditorLayer::OnEvent()
    {

    }
}