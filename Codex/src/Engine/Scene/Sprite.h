#ifndef CODEX_SCENE_SPRITE_H
#define CODEX_SCENE_SPRITE_H

#include "../Core/Geomtryd.h"
#include "../Core/IResource.h"
#include "../Renderer/Texture2D.h"

namespace codex {
    class Sprite
    {
    private:
        ResRef<Texture2D> m_Texture = nullptr;
        Rectf             m_SrcRect{};
        Vector4f          m_Colour{};
        i32               m_ZIndex = 0;

    public:
        Sprite() = default;
        Sprite(ResRef<Texture2D> texture, const Vector4f colour = { 1.0f, 1.0f, 1.0f, 1.0f }, const i32 zIndex = 0)
            : m_Texture(texture), m_Colour(colour), m_ZIndex(zIndex)
        {
            if (m_Texture)
                m_SrcRect = { 0.0f, 0.0f, (f32)texture->GetWidth(), (f32)texture->GetHeight() };
        }
        Sprite(ResRef<Texture2D> texture, const Rectf textureCoords, const Vector4f colour = { 1.0f, 1.0f, 1.0f, 1.0f },
               const i32 zIndex = 0)
            : m_Texture(texture), m_SrcRect(textureCoords), m_Colour(colour), m_ZIndex(zIndex)
        {
        }

    public:
        static inline Sprite Empty() noexcept { return Sprite(); }

    public:
        inline ResRef<Texture2D> GetTexture() const noexcept { return m_Texture; }
        inline Rectf&            GetTextureCoords() noexcept { return m_SrcRect; }
        inline i32               GetWidth() const noexcept { return m_Texture->GetWidth(); }
        inline i32               GetHeight() const noexcept { return m_Texture->GetHeight(); }
        inline Vector4f&         GetColour() noexcept { return m_Colour; }
        inline i32&              GetZIndex() noexcept { return m_ZIndex; }
        inline void              SetTexture(ResRef<Texture2D> texture) noexcept { m_Texture = texture; }
        inline operator bool() const noexcept { return m_Texture != nullptr; }
    };
} // namespace codex

#endif // CODEX_SCENE_SPRITE_H
