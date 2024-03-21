#ifndef CODEX_GRAPHICS_IMAGE_2D_H
#define CODEX_GRAPHICS_IMAGE_2D_H

#include <sdafx.h>

#include "../Core/Exception.h"
#include "../Core/IResource.h"
#include "../Memory/Memory.h"

namespace codex::graphics {
    // Forward declerations.
    class Image2D;
} // namespace codex::graphics

namespace std {
    void swap(codex::graphics::Image2D& lhv, codex::graphics::Image2D& rhv) noexcept;
} // namespace std

namespace codex::graphics {
    CX_CUSTOM_EXCEPTION(ImageLoadException, "Failed to load the image.")

    // Wrapper over stb_image.
    class Image2D : public IResource
    {
    private:
        u8*                   m_RawData  = nullptr;
        i32                   m_Width    = 0;
        i32                   m_Height   = 0;
        i32                   m_Channels = 0;
        std::filesystem::path m_Path{};

    public:
        Image2D() = default;
        explicit Image2D(std::filesystem::path file);
        Image2D(const Image2D& other);
        Image2D(Image2D&& other) noexcept;
        ~Image2D();

    public:
        constexpr operator bool() const noexcept;
        Image2D& operator=(const Image2D& other);
        Image2D& operator=(Image2D&& other) noexcept;

    public:
        u8*                          GetRaw() noexcept { return m_RawData; }
        const u8*                    GetRaw() const noexcept { return m_RawData; }
        i32                          GetWidth() const noexcept { return m_Width; }
        i32                          GetHeight() const noexcept { return m_Height; }
        i32                          GetChannels() const noexcept { return m_Channels; }
        const std::filesystem::path& GetPath() const noexcept { return m_Path; }

    public:
        void friend std::swap(Image2D& lhv, Image2D& rhv) noexcept;
    };
} // namespace codex::graphics

#endif // CODEX_GRAPHICS_IMAGE_2D_H