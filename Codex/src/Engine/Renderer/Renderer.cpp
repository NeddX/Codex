#include "Renderer.h"

namespace codex {
    i32            Renderer::m_Width            = 0;
    i32            Renderer::m_Height           = 0;
    mgl::Renderer* Renderer::m_InternalRenderer = nullptr;

    void Renderer::Init(const i32 width, const i32 height)
    {
        m_Width            = width;
        m_Height           = height;
        m_InternalRenderer = new mgl::Renderer(width, height);
        GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        fmt::println("Renderer subsystem initialized.");
    }

    void Renderer::Destroy()
    {
        delete m_InternalRenderer;
        m_InternalRenderer = nullptr;
        m_Width            = 0;
        m_Height           = 0;
        fmt::println("Renderer subsystem disposed.");
    }
} // namespace codex
