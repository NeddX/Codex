#ifndef CODEX_CORE_RESOURCE_HANDLER_H
#define CODEX_CORE_RESOURCE_HANDLER_H

#include <sdafx.h>

#include "../Renderer/Shader.h"
#include "../Renderer/Texture2D.h"
#include "Exception.h"
#include "IResource.h"

namespace codex {
    class ResourceException : public CodexException
    {
        using CodexException::CodexException;

    public:
        constexpr const char* default_message() const noexcept override { return "Bad resource."; }
    };

    class ResourceNotFoundException : public ResourceException
    {
        using ResourceException::ResourceException;

    public:
        constexpr const char* default_message() const noexcept override { return "Failed to load resource."; }
    };

    class CODEX_API Resources
    {
    private:
        static Resources* m_Instance;

    private:
        std::unordered_map<usize, ResRef<IResource>> m_Resources;

    public:
        static void Init();
        static void Destroy();

    private:
        static ResRef<graphics::Texture2D> Load_Texture2D(const std::filesystem::path       filePath,
                                                          const graphics::TextureProperties props = {});
        static ResRef<graphics::Shader>    Load_Shader(const std::filesystem::path filePath,
                                                       const std::string_view      version = "330 core");

    public:
        template <typename T, typename... TArgs>
        static ResRef<T> Load(const std::filesystem::path filePath, TArgs&&... args)
        {
            using namespace codex::graphics;

            if constexpr (std::is_same_v<T, Texture2D>)
                return Load_Texture2D(filePath, std::forward<TArgs>(args)...);
            else if constexpr (std::is_same_v<T, Shader>)
                return Load_Shader(filePath, std::forward<TArgs>(args)...);

            static_assert("Type not supported.");
            return nullptr;
        }

    public:
        template <typename T>
        static ResRef<T> GetResource(const usize id)
        {
            auto it = m_Instance->m_Resources.find(id);
            if (it != m_Instance->m_Resources.end())
                return std::static_pointer_cast<T>(it->second);
            else
                throw ResourceNotFoundException(
                    fmt::format("Hash Id {} was not present in the resource pool.", id).c_str());
        }
        template <typename T>
        static ResRef<T> GetResource(const std::filesystem::path filePath)
        {
            return GetResource<T>(util::Crypto::DJB2Hash(filePath.c_str()));
        }
        static bool                                          HasResource(const usize id);
        static bool                                          HasResource(const std::filesystem::path filePath);
        static std::unordered_map<usize, ResRef<IResource>>& GetAllResources();
    };
} // namespace codex

#endif // CODEX_CORE_RESOURCE_HANDLER_H
