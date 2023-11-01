#include "ResourceHandler.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Texture2D.h"

namespace codex {
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

    template <>
    ResRef<Texture2D> Resources::Load(const char* filePath)
    {
        if (HasResource(filePath))
            return GetResource<Texture2D>(filePath);

        std::ifstream fs(filePath);
        if (fs.is_open())
        {
            usize id = util::Crypto::DJB2Hash(filePath);

            // TODO: Make it so that the user can pass arguments to Load<T>() !
            TextureProperties props;
            props.filterMode = TextureFilterMode::Nearest;

            ResRef<Texture2D> texture   = std::make_shared<Texture2D>(filePath, props);
            m_Instance->m_Resources[id] = std::static_pointer_cast<IResource>(texture);
            fs.close();
            fmt::println("[ResourceHandler] >> File: '{}' Id: {}", filePath, id);
            return texture;
        }
        else
        {
            CX_THROW(
                ResourceNotFoundException,
                fmt::format("Couldn't open file '{}' for reading. Failed to load texture asset.", filePath).c_str());
            return nullptr;
        }
    }

    template <>
    ResRef<Shader> Resources::Load(const char* filePath)
    {
        std::ifstream fs(filePath);
        if (fs.is_open())
        {
            usize          id           = util::Crypto::DJB2Hash(filePath);
            ResRef<Shader> texture      = std::make_shared<Shader>(filePath);
            m_Instance->m_Resources[id] = std::static_pointer_cast<IResource>(texture);
            fs.close();
            fmt::println("[ResourceHandler] >> File: '{}' Id: {}", filePath, id);
            return texture;
        }
        else
        {
            CX_THROW(
                ResourceNotFoundException,
                fmt::format("Couldn't open file '{}' for reading. Failed to load shader asset.", filePath).c_str());
            return nullptr;
        }
    }
} // namespace codex
