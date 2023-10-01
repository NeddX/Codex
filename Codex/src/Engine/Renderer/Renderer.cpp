#include "Renderer.h"

namespace Codex {
	Renderer::Renderer(const i32 width, const i32 height) :
		m_Width(width), m_Height(height)
	{
		m_InternalRenderer = std::make_unique<mgl::Renderer>(m_Width, m_Height);
		m_BatchRenderer = nullptr;
		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
}
