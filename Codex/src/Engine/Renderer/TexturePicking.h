#ifndef CODEX_RENDERER_TEXTURE_PICKING_H
#define CODEX_RENDERER_TEXTURE_PICKING_H

#include <sdafx.h>

namespace Codex {
	class TexturePicking
	{
	private:
		uint32_t m_Id;
		int m_DepthTexture;
		std::unique_ptr<mgl::FrameBuffer> m_Fbo;
		 
	public:
		TexturePicking(const int width, const int height);

	public:
		inline void SetEnableWriting(const bool enable)
		{ 
			if (enable) glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo->GetId()); 
			else glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
		}
		inline int ReadPixel(const int x, const int y)
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Fbo->GetId());
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			float* pixels = new float[3];
			glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
			int ret = (int)pixels[0];
			delete[] pixels;
			return ret;
		}

	public:
		bool Init(const int width, const int height);
	};
}

#endif // CODEX_RENDERER_TEXTURE_PICKING_H
