#include "Renderer.h"

namespace Codex
{
	Renderer::Renderer(const int width, const int height) :
		m_Width(width), m_Height(height)
	{
		m_InternalRenderer = std::unique_ptr<mgl::Renderer>(new mgl::Renderer(width, height));
		m_BatchRenderer = nullptr;
		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	Renderer::~Renderer()
	{
		delete m_BatchRenderer;
	}
}