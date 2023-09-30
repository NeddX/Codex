#include "Renderer.h"

namespace Codex {
	Renderer::Renderer(const int width, const int height) :
		m_Width(width), m_Height(height)
	{
		m_InternalRenderer = std::make_unique<mgl::Renderer>(m_Width, m_Height);
		m_BatchRenderer = nullptr;
		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
}
