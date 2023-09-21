#ifndef MGL_FRAME_BUFFER_H
#define MGL_FRAME_BUFFER_H

#include <cstdint>
#include <vector>

#include "Texture.h"
#include "Constants.h"

namespace mgl {
	struct FrameBufferProperties
	{
	public:
		uint32_t width;
		uint32_t height;
		uint32_t samples = 1;
		std::vector<TextureProperties> attachments;

	public:
		FrameBufferProperties() = default; 
		FrameBufferProperties(uint32_t width, uint32_t height, std::initializer_list<TextureProperties> attachments) :
			width(width), height(height), attachments(attachments) {  }
	};

	class FrameBuffer
	{
	private:
		uint32_t m_RendererId = 0;
		FrameBufferProperties m_Properties;
		std::vector<TextureProperties> m_ColourAttachments;
		std::vector<uint32_t> m_ColourAttachmentIds;
		TextureProperties m_DepthAttachment;
		uint32_t m_DepthAttachmentId;

	public:
		FrameBuffer(const FrameBufferProperties& props);
		~FrameBuffer();

	public:
		void Invalidate();
		void Bind();
		void Unbind();
		int ReadPixel(uint32_t index, int x, int y);
		std::vector<uint8_t> FrameBuffer::ReadAllPixels(uint32_t index)
		{
			std::vector<uint8_t> pixel_data(m_Properties.width * m_Properties.height * 4);
			GL_Call(glReadBuffer(GL_COLOR_ATTACHMENT0 + index));
			GL_Call(glReadPixels(0, 0, m_Properties.width, m_Properties.height, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data.data()));
			return pixel_data;
		}

	private:
		void Resize(uint32_t width, uint32_t height);
		void SetCurrentlyBoundTextureProperties(TextureWrapMode wrapMode, TextureFilterMode filterMode);
		GLenum DetermineFormatType(TextureFormat format);

	public:
		inline uint32_t GetId() const { return m_RendererId; }
		inline FrameBufferProperties GetProperties() const { return m_Properties; }
		inline uint32_t GetColourAttachmentIdAt(uint32_t index = 0) const { MGL_ASSERT(index < m_ColourAttachmentIds.size(), "Index outside bounds of colour attachments."); return m_ColourAttachmentIds[index]; }
		inline uint32_t GetDepthAttachmentId() const { return m_DepthAttachmentId; }
		inline void SetProperties(FrameBufferProperties newProps) { m_Properties = newProps; Invalidate(); }
		inline void SetReadOnly(bool readOnly) { Bind(); if (readOnly) glReadBuffer(GL_NONE); else glReadBuffer(GL_ONE); Unbind(); }
		inline void SetDrawBuffer(int attachmentId = 0) { Bind(); glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachmentId); Unbind(); }
	};
}

#endif // MGL_FRAME_BUFFER_H