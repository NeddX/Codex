#ifndef CODEX_CORE_TEXTURE_2D_H
#define CODEX_CORE_TEXTURE_2D_H

#include <sdafx.h>

#include "../Core/IResource.h"
#include "../Memory/Memory.h"

namespace codex::gfx {
    typedef mgl::TextureProperties TextureProperties;
    typedef mgl::TextureFilterMode TextureFilterMode;
    typedef mgl::TextureWrapMode   TextureWrapMode;

    class Texture2D : public IResource
    {
        friend class ResourceHandler;

    private:
        mem::Box<mgl::Texture> m_RawTexture = nullptr;

    public:
        Texture2D() = default;
        Texture2D(std::filesystem::path filePath, const TextureProperties textureProperties = {})
        {
            m_RawTexture = mem::Box<mgl::Texture>::New(std::move(filePath), textureProperties);
        }

    public:
        explicit inline                 operator bool() const noexcept { return (bool)m_RawTexture; }
        inline u32                      GetGlId() const noexcept { return m_RawTexture->GetGlId(); }
        inline u32                      GetSlot() const { return m_RawTexture->GetSlot(); }
        inline i32                      GetWidth() const { return m_RawTexture->GetWidth(); }
        inline i32                      GetHeight() const { return m_RawTexture->GetHeight(); }
        inline std::filesystem::path    GetFilePath() const { return m_RawTexture->GetFilePath(); }
        inline void                     Bind(u32 slot = 0) { m_RawTexture->Bind(slot); }
        inline void                     Unbind() const { m_RawTexture->Unbind(); }
        inline const TextureProperties& GetProperties() const noexcept { return m_RawTexture->GetProperties(); }
        inline void New(const std::filesystem::path filePath, const TextureProperties textureProperties = {})
        {
            m_RawTexture.Reset(new mgl::Texture(filePath, textureProperties));
        }

    public:
        friend void to_json(nlohmann::ordered_json& j, const Texture2D& texture)
        {
            const auto& props = texture.m_RawTexture->GetProperties();
            j                 = { { "m_Id", texture.m_Id },
                                  { "m_FilePath", texture.GetFilePath() },
                                  { "filterMode", props.filterMode },
                                  { "wrapMode", props.wrapMode },
                                  { "format", props.format } };
        }
    };
} // namespace codex::gfx

#endif // CODEX_CORE_TEXTURE_2D_H
