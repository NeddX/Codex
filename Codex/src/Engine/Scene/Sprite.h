#ifndef CODEX_SCENE_SPRITE_H
#define CODEX_SCENE_SPRITE_H

#include "../Core/Geomtryd.h"
#include "../Core/IResource.h"
#include "../Core/ResourceHandler.h"
#include "../Renderer/Texture2D.h"

namespace codex {
    class Sprite
    {
    private:
        ResRef<Texture2D> m_Texture = nullptr;
        Rectf             m_TextureCoords{};
        Vector2f          m_SpriteSize{};
        Vector4f          m_Colour{};
        i32               m_ZIndex = 0;

    public:
        Sprite() = default;
        Sprite(ResRef<Texture2D> texture, const Vector4f colour = { 1.0f, 1.0f, 1.0f, 1.0f }, const i32 zIndex = 0)
            : m_Texture(texture), m_Colour(colour), m_ZIndex(zIndex)
        {
            m_TextureCoords = { 0.0f, 0.0f, (f32)texture->GetWidth(), (f32)texture->GetHeight() };
            m_SpriteSize    = { m_TextureCoords.w, m_TextureCoords.h };
        }
        Sprite(ResRef<Texture2D> texture, const Rectf textureCoords, const Vector4f colour = { 1.0f, 1.0f, 1.0f, 1.0f },
               const i32 zIndex = 0)
            : m_Texture(texture), m_TextureCoords(textureCoords), m_Colour(colour), m_ZIndex(zIndex)
        {
            m_SpriteSize = { (f32)m_Texture->GetWidth(), (f32)m_Texture->GetHeight() };
        }

    public:
        static inline Sprite Empty() noexcept { return Sprite(); }

    public:
        inline ResRef<Texture2D> GetTexture() const noexcept { return m_Texture; }
        inline Rectf&            GetTextureCoords() noexcept { return m_TextureCoords; }
        inline Vector4f&         GetColour() noexcept { return m_Colour; }
        inline i32&              GetZIndex() noexcept { return m_ZIndex; }
        inline Vector2f          GetSize() const noexcept { return m_SpriteSize; }
        inline void              SetSize(const Vector2f newSize) noexcept { m_SpriteSize = newSize; }
        inline void              SetTexture(ResRef<Texture2D> texture) noexcept
        {
            m_Texture       = texture;
            m_TextureCoords = { 0.0f, 0.0f, (f32)m_Texture->GetWidth(), (f32)m_Texture->GetHeight() };
            m_SpriteSize    = { m_TextureCoords.w, m_TextureCoords.h };
            m_Colour        = { 1.0f, 1.0f, 1.0f, 1.0f };
        }
        inline operator bool() const noexcept { return m_Texture != nullptr; }

    public:
        friend void to_json(nlohmann::json& j, const Sprite& sprite)
        {
            j = { { "m_Texture", *sprite.m_Texture },
                  { "m_TextureCoords", sprite.m_TextureCoords },
                  { "m_SpriteSize", sprite.m_SpriteSize },
                  { "m_Colour", sprite.m_Colour },
                  { "m_ZIndex", sprite.m_ZIndex } };
        }
        friend void from_json(const nlohmann::json& j, Sprite& sprite)
        {
            std::string path;
            j.at("m_Texture").at("m_FilePath").get_to(path);
            j.at("m_TextureCoords").get_to(sprite.m_TextureCoords);
            j.at("m_SpriteSize").get_to(sprite.m_SpriteSize);
            j.at("m_Colour").get_to(sprite.m_Colour);
            j.at("m_ZIndex").get_to(sprite.m_ZIndex);
            sprite.m_Texture = Resources::Load<Texture2D>(path.c_str());
        }
    }; // namespace codex
} // namespace codex

#endif // CODEX_SCENE_SPRITE_H
