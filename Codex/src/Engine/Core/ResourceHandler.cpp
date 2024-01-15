#include "ResourceHandler.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Texture2D.h"

namespace codex {
    using namespace codex::graphics;

    Resources* Resources::m_Instance = nullptr;

    void Resources::Init()
    {
        if (!m_Instance)
        {
            m_Instance = new Resources();

            // TODO: Add proper logging system.
            fmt::println("ResourceHandler subsystem initialized.");
        }
    }

    void Resources::Destroy()
    {
        if (m_Instance)
        {
            delete m_Instance;
            m_Instance = nullptr;

            // TODO: Add proper logging system.
            fmt::println("ResourceHandler subsystem disposed.");
        }
    }

    ResRef<Texture2D> Resources::Load_Texture2D(const std::filesystem::path filePath, const TextureProperties props)
    {
        // I could use .c_str() but on MSVC c_str() is of value_type type which itself is wchar_t.
        // To put it short, It is for compatability reasons.
        if (HasResource(filePath.string()))
            return GetResource<Texture2D>(filePath);

        // TODO: Use std::filesystem::path instead of string_view.
        std::ifstream fs(filePath.string());
        if (fs.is_open())
        {
            usize id = util::Crypto::DJB2Hash(filePath.string());

            ResRef<Texture2D> texture   = std::make_shared<Texture2D>(filePath.string(), props);
            m_Instance->m_Resources[id] = std::static_pointer_cast<IResource>(texture);
            fs.close();
            fmt::println("[ResourceHandler] >> File: '{}' Id: {}", filePath.string(), id);
            return texture;
        }
        else
        {
            cx_throw(ResourceNotFoundException, "Couldn't open file '{}' for reading. Failed to load texture asset.",
                     filePath.string());
            return nullptr;
        }
    }

    ResRef<Shader> Resources::Load_Shader(const std::filesystem::path filePath, const std::string_view version)
    {
        if (HasResource(filePath))
            return GetResource<Shader>(filePath);

        // TODO: Use std::filesystem::path instead of string_view.
        std::ifstream fs(filePath.string());
        if (fs.is_open())
        {
            usize          id           = util::Crypto::DJB2Hash(filePath.string());
            ResRef<Shader> texture      = std::make_shared<Shader>(filePath.string(), version);
            m_Instance->m_Resources[id] = std::static_pointer_cast<IResource>(texture);
            fs.close();
            fmt::println("[ResourceHandler] >> File: '{}' Id: {}", filePath.string(), id);
            return texture;
        }
        else
        {
            cx_throw(ResourceNotFoundException, "Couldn't open file '{}' for reading. Failed to load shader asset.",
                     filePath.string());
            return nullptr;
        }
    }

    bool Resources::HasResource(const usize id)
    {
        auto it = m_Instance->m_Resources.find(id);
        if (it != m_Instance->m_Resources.end())
            return true;
        return false;
    }

    bool Resources::HasResource(const std::filesystem::path filePath)
    {
        usize id = util::Crypto::DJB2Hash(filePath.string());
        return HasResource(id);
    }

    std::unordered_map<usize, ResRef<IResource>>& Resources::GetAllResources()
    {
        return m_Instance->m_Resources;
    }
} // namespace codex
