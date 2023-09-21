#include "TexturePicking.h"

namespace Codex {

	TexturePicking::TexturePicking(int width, int height)
	{
		Init(width, height);
	}

	bool TexturePicking::Init(int width, int height)
	{
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

}