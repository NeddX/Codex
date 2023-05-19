#ifndef CODEX_NATIVE_API_TEXTURE_2D_H
#define CODEX_NATIVE_API_TEXTURE_2D_H

#include <cstdint>

namespace
{
	enum class TextureWrapMode : uint8_t
	{
		Mirror,
		Stretch,
		Border
	};

	enum TextureFilterMode : uint8_t
	{
		Blur,
		Pixelate
	};

	struct TextureProperties
	{
		TextureWrapMode textureWrapMode = TextureWrapMode::Mirror;
		TextureFilterMode textureFilterMode = TextureFilterMode::Blur;
	};

	class Texture2D
	{
	private:
		const char* m_FilePath;
		TextureProperties m_Properties;

	public:
		inline const char* GetFilePath() const { return m_FilePath; }

	public:
		Texture2D(const char* filePath, TextureProperties properties)
			: m_FilePath(filePath), m_Properties(properties)
		{

		}
	};
}

#endif // CODEX_NATIVE_API_TEXTURE_2D_H