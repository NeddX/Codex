#ifndef CODEX_EDITOR_EDITOR_LAYER_H
#define CODEX_EDITOR_EDITOR_LAYER_H

#include <sdafx.h>

#include "../Core/KeyHandler.h"
#include "../Core/Layer.h"
#include "../Renderer/Renderer.h"

namespace Codex {
	class EditorLayer : public Layer
	{
    private:
        Renderer* m_Renderer;

    public:
        EditorLayer(const char* name, Renderer* renderer);

    public:
        void OnAttach() override;
        void OnDetach() override;
        void Update(float deltaTime, Scene* scene) override;
        void Render(float deltaTime) override;
        void OnEvent() override;
	};
}

#endif // CODEX_EDITOR_EDITOR_LAYER_H