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
        inline const char* default_message() const noexcept override { return "Bad resource."; }
    };

    class ResourceNotFoundException : public ResourceException
    {
        using ResourceException::ResourceException;

    public:
        inline const char* default_message() const noexcept override { return "Failed to load resource."; }
    };

    class Resources
    {
    private:
        static Resources* m_Instance;

    private:
        std::unordered_map<usize, ResRef<IResource>> m_Resources;

    public:
        static void Init();
        static void Destroy();

    private:
        static ResRef<Texture2D> Load_Texture2D(const std::string_view filePath, const TextureProperties props = {});
        static ResRef<Shader> Load_Shader(const std::string_view filePath, const std::string_view version = "330 core");

    public:
        template <typename T, typename... TArgs>
        static ResRef<T> Load(const std::string_view filePath, TArgs&&... args)
        {
            if constexpr (std::is_same_v<T, Texture2D>)
                return Load_Texture2D(filePath, std::forward<TArgs>(args)...);
            else if constexpr (std::is_same_v<T, Shader>)
                return Load_Shader(filePath, std::forward<TArgs>(args)...);

            static_assert("Type not supported.");
            return nullptr;
        }

    public:
        template <typename T>
        inline static ResRef<T> GetResource(const usize id)
        {
            auto it = m_Instance->m_Resources.find(id);
            if (it != m_Instance->m_Resources.end())
                return std::static_pointer_cast<T>(it->second);
            else
                throw ResourceNotFoundException(
                    fmt::format("Hash Id {} was not present in the resource pool.", id).c_str());
        }
        template <typename T>
        inline static ResRef<T> GetResource(const std::string_view filePath)
        {
            return GetResource<T>(util::Crypto::DJB2Hash(filePath));
        }
        inline static bool HasResource(const usize id)
        {
            auto it = m_Instance->m_Resources.find(id);
            if (it != m_Instance->m_Resources.end())
                return true;
            return false;
        }
        inline static bool HasResource(const std::string_view filePath)
        {
            usize id = util::Crypto::DJB2Hash(filePath);
            return HasResource(id);
        }
        inline static std::unordered_map<usize, ResRef<IResource>>& GetAllResources()
        {
            return m_Instance->m_Resources;
        }
    };
} // namespace codex

#endif // CODEX_CORE_RESOURCE_HANDLER_H