#ifndef CODEX_SCENE_SPRITE_H
#define CODEX_SCENE_SPRITE_H

#include "../Core/Geomtryd.h"
#include "../Renderer/Texture2D.h"
#include "../Core/IResource.h"

namespace codex {
    class Sprite
    {
    private:
        ResRef<Texture2D> m_Texture;
        Rectf             m_SrcRect;

    public:
        Sprite(ResRef<Texture2D> texture) : m_Texture(texture)
        {
            if (m_Texture)
                m_SrcRect = {0.0f, 0.0f, (f32)texture->GetWidth(), (f32)texture->GetHeight()};
        }
        Sprite(ResRef<Texture2D> texture, const Rectf textureCoords) : m_Texture(texture), m_SrcRect(textureCoords) {}

    public:
        inline ResRef<Texture2D> GetTexture() const noexcept { return m_Texture; }
        inline Rectf             GetTextureCoords() const noexcept { return m_SrcRect; }
        inline void              SetTexture(ResRef<Texture2D> texture) noexcept { m_Texture = texture; }
        inline void              SetTextureCoords(const Rectf textureCoords) noexcept { m_SrcRect = textureCoords; }
        inline i32               GetWidth() const noexcept { return m_Texture->GetWidth(); }
        inline i32               GetHeight() const noexcept { return m_Texture->GetHeight(); }
    };
} // namespace codex

#endif // CODEX_SCENE_SPRITE_H
