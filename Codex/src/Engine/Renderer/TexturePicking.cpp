#include "TexturePicking.h"

namespace codex::renderer {

    TexturePicking::TexturePicking(const i32 width, const i32 height)
    {
        Init(width, height);
    }

    bool TexturePicking::Init(const i32 width, const i32 height)
    {
        m_Id           = 0;
        m_DepthTexture = 0;
        /*
        mgl::FrameBufferProperties props;
        props.width = width;
        props.height = height;
        props.colourTexFilterMode = mgl::TextureFilterMode::Nearest;
        props.colourTexFormat = mgl::TextureFormatType::RGB;
        props.colourTexInternalFormat = mgl::TextureFormat::RGB32F;
        props.depthTexFormat = mgl::TextureFormatType::DEPTH_STENCIL;
        props.depthTexInternalFormat = mgl::TextureFormat::DEPTH24_STENCIL8;

        m_Fbo = std::make_unique<mgl::FrameBuffer>(props);
        m_Fbo->SetReadOnly(true);
        m_Fbo->SetDrawBuffer(0);
        m_Fbo->Unbind();*/

        return true;
    }

} // namespace codex::renderer
