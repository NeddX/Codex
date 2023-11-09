#ifndef CODEX_RENDERER_TEXTURE_PICKING_H
#define CODEX_RENDERER_TEXTURE_PICKING_H

#include <sdafx.h>

namespace codex {
	class TexturePicking
	{
	private:
		u32 m_Id;
		i32 m_DepthTexture;
		std::unique_ptr<mgl::FrameBuffer> m_Fbo;
		 
	public:
		TexturePicking(const i32 width, const i32 height);

	public:
		inline void SetEnableWriting(const bool enable)
		{ 
			if (enable) glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo->GetId()); 
			else glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
		}
		inline i32 ReadPixel(const i32 x, const i32 y)
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Fbo->GetId());
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			f32 pixels[3];
			glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
			i32 ret = (i32)pixels[0];
			return ret;
		}

	public:
		bool Init(const i32 width, const i32 height);
	};
}

#endif // CODEX_RENDERER_TEXTURE_PICKING_H
