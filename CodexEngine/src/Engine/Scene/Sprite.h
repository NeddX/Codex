#ifndef CODEX_SCENE_SPRITE_H
#define CODEX_SCENE_SPRITE_H

#include "../Core/Geomtryd.h"
#include "../Core/IResource.h"
#include "../Core/ResourceHandler.h"
#include "../Graphics/Texture2D.h"

namespace codex {
    class Sprite
    {
    private:
        ResRef<gfx::Texture2D> m_Texture = nullptr;
        Rectf                  m_TextureCoords{};
        Vector2f               m_Size{};
        Vector4f               m_Colour{};
        i32                    m_ZIndex = 0;

    public:
        Sprite() = default;
        Sprite(ResRef<gfx::Texture2D> texture, const Vector4f colour = { 1.0f, 1.0f, 1.0f, 1.0f }, const i32 zIndex = 0)
            : m_Texture(texture)
            , m_Colour(colour)
            , m_ZIndex(zIndex)
        {
            m_TextureCoords = { 0.0f, 0.0f, static_cast<f32>(texture->GetWidth()),
                                static_cast<f32>(texture->GetHeight()) };
            m_Size          = { m_TextureCoords.w, m_TextureCoords.h };
        }
        Sprite(ResRef<gfx::Texture2D> texture, const Rectf textureCoords,
               const Vector4f colour = { 1.0f, 1.0f, 1.0f, 1.0f }, const i32 zIndex = 0)
            : m_Texture(texture)
            , m_TextureCoords(textureCoords)
            , m_Colour(colour)
            , m_ZIndex(zIndex)
        {
            m_Size = { (f32)m_Texture->GetWidth(), (f32)m_Texture->GetHeight() };
        }

    public:
        static inline Sprite Empty() noexcept { return Sprite(); }

    public:
        [[nodiscard]] inline ResRef<gfx::Texture2D> GetTexture() const noexcept { return m_Texture; }
        inline void SetTexture(ResRef<gfx::Texture2D> newTexture) noexcept { Sprite{ newTexture }.Swap(*this); }
        CX_PROPERTY(TextureCoords)
        CX_PROPERTY(Colour)
        CX_PROPERTY(ZIndex)
        CX_PROPERTY(Size)

    public:
        inline void Swap(Sprite& other) noexcept
        { 
            std::swap(m_Texture, other.m_Texture);
            std::swap(m_TextureCoords, other.m_TextureCoords);
            std::swap(m_Size, other.m_Size);
            std::swap(m_Colour, other.m_Colour);
            std::swap(m_ZIndex, other.m_ZIndex);
        }

    public:
        inline operator bool() const noexcept { return m_Texture; }

    public:
        friend void to_json(nlohmann::ordered_json& j, const Sprite& sprite)
        {
            j = { { "m_Texture", *sprite.m_Texture },
                  { "m_TextureCoords", sprite.m_TextureCoords },
                  { "m_Size", sprite.m_Size },
                  { "m_Colour", sprite.m_Colour },
                  { "m_ZIndex", sprite.m_ZIndex } };
        }
        friend void from_json(const nlohmann::ordered_json& j, Sprite& sprite)
        {
            using namespace codex::gfx;

            std::string       path;
            TextureProperties props;
            j.at("m_Texture").at("m_FilePath").get_to(path);
            j.at("m_Texture").at("filterMode").get_to(props.filterMode);
            j.at("m_Texture").at("wrapMode").get_to(props.wrapMode);
            j.at("m_Texture").at("format").get_to(props.format);

            j.at("m_TextureCoords").get_to(sprite.m_TextureCoords);
            j.at("m_Size").get_to(sprite.m_Size);
            j.at("m_Colour").get_to(sprite.m_Colour);
            j.at("m_ZIndex").get_to(sprite.m_ZIndex);
            sprite.m_Texture = Resources::Load<Texture2D>(path, props);
        }
    }; // namespace codex
} // namespace codex

#endif // CODEX_SCENE_SPRITE_H
