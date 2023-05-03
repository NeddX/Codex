#include "ResourceHandler.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Shader.h"

namespace Codex
{
	std::unordered_map<size_t, std::shared_ptr<IResource>> Resources::m_Resources;

	/*
	template<typename T, typename>
	std::shared_ptr<T> Resources::Load(const char* filePath)
	{
		static_assert("Type not supported.");
		return nullptr;
	}

	template<>
	std::shared_ptr<Texture2D> Resources::Load(const char* filePath)
	{
		std::ifstream fs(filePath);
		if (fs.is_open())
		{
			size_t id = util::Crypto::DJB2Hash(filePath);
			std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(filePath); // TODO: Can't pass texture properties! fix that!
			m_Resources[id] = std::static_pointer_cast<IResource>(texture);
			fs.close();
			fmt::println("[ResourceHandler] >> File: '{}' ID: {}", filePath, id);
			return texture;
		}
		else
		{
			fs.close();
			throw std::runtime_error(fmt::format("Couldn't open file '{}' for reading. Failed to load texture asset.", filePath));
			return nullptr;
		}
	}

	template<>
	std::shared_ptr<Shader> Resources::Load(const char* filePath)
	{
		std::ifstream fs(filePath);
		if (fs.is_open())
		{
			size_t id = util::Crypto::DJB2Hash(filePath);
			std::shared_ptr<Shader> texture = std::make_shared<Shader>(filePath);
			m_Resources[id] = std::static_pointer_cast<IResource>(texture);
			fs.close();
			fmt::println("[ResourceHandler] >> File: '{}' ID: {}", filePath, id);
			return texture;
		}
		else
		{
			fs.close();
			throw std::runtime_error(fmt::format("Couldn't open file '{}' for reading. Failed to load shader asset.", filePath));
			return nullptr;
		}
	}
	*/
}