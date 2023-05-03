#ifndef MARFX_TEXTURE_H
#define MARFX_TEXTURE_H

#include <cstdint>

namespace mfx
{
	enum class TextureWrapMode : uint8_t
	{
		MIRROR,
		STRETCH,
		BORDER
	};

	enum class TextureFilterMode : uint8_t
	{
		BLUR,
		PIXELATE
	};

	struct TextureProperties
	{
		TextureWrapMode textureWrapMode = TextureWrapMode::MIRROR;
		TextureFilterMode textureFilterMode = TextureFilterMode::BLUR;

	public:
		TextureProperties(const TextureFilterMode filterMode = TextureFilterMode::PIXELATE, 
			const TextureWrapMode wrapMode = TextureWrapMode::BORDER) :
			textureWrapMode(wrapMode), textureFilterMode(filterMode)
		{

		}
	};

	class Texture
	{
	private:
		uint32_t m_RendererID;
		const char* m_FilePath;
		uint8_t* m_Buffer;
		int m_Width;
		int m_Height;
		int m_GPP;
		uint32_t m_Slot;

	public:
		inline int GetWidth() 		const { return m_Width; }
		inline int GetHeight() 		const { return m_Height; }
		inline uint32_t GetSlot()	const { return m_Slot; }
		
	public:
		Texture(const char* filePath, const TextureProperties properties = {});
		~Texture();
		
	public:
		void Bind(uint32_t slot = 0);
		void Unbind() const;
	};
}

#endif // MARFX_TEXTURE_H