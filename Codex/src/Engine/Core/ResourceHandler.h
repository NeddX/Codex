#ifndef CODEX_CORE_RESOURCE_HANDLER_H
#define CODEX_CORE_RESOURCE_HANDLER_H

#include <sdafx.h>

#include "IResource.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Texture2D.h"

namespace Codex {
	class Resources
	{
#ifdef CX_DEBUG_CUSTOM_ALLOCATORS
	public:
		void* operator new(size_t size)
		{
			void* ptr = std::malloc(size);
			fmt::println("[Memory] :: Allocated memory.\n\tFile: {}\n\tLine: {}\n\tSize: {}\n\tAddress: {}",
				__FILE__, __LINE__, size, ptr);
			return ptr;
		}
		void operator delete(void* ptr)
		{
			fmt::println("[Memory] :: Deallocated memory.\n\tFile: {}\n\tLine: {}\n\tAddress: {}", __FILE__, __LINE__, ptr);
			std::free(ptr);
		}
#endif
	private:
		static Resources* m_Instance;

	private:
		std::unordered_map<size_t, std::shared_ptr<IResource>> m_Resources;

	public:
		Resources() = default;
		~Resources();

	public:
		static void Init();
		static void Destroy();

	public:
		template<typename T>
		static std::shared_ptr<T> Load(const char* filePath)
		{
			static_assert("Type not supported.");
			return nullptr;
		}
		template<>
		std::shared_ptr<Texture2D> Load(const char* filePath)
		{
			if (HasResource(filePath)) return GetResource<Texture2D>(filePath);

			std::ifstream fs(filePath);
			if (fs.is_open())
			{
				size_t id = util::Crypto::DJB2Hash(filePath);

				// TODO: Make it so that the user can pass arguments to Load<T>() !
				TextureProperties props;
				props.filterMode = TextureFilterMode::Nearest;

				std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(filePath, props);
				m_Instance->m_Resources[id] = std::static_pointer_cast<IResource>(texture);
				fs.close();
				fmt::println("[ResourceHandler] >> File: '{}' Id: {}", filePath, id);
				return texture;
			}
			else
			{
				fs.close();
				fmt::println("Couldn't open file '{}' for reading. Failed to load texture asset.", filePath);
				throw std::runtime_error("");
				return nullptr;
			}
		}
		template<>
		std::shared_ptr<Shader> Load(const char* filePath)
		{
			std::ifstream fs(filePath);
			if (fs.is_open())
			{
				size_t id = util::Crypto::DJB2Hash(filePath);
				std::shared_ptr<Shader> texture = std::make_shared<Shader>(filePath);
				m_Instance->m_Resources[id] = std::static_pointer_cast<IResource>(texture);
				fs.close();
				fmt::println("[ResourceHandler] >> File: '{}' Id: {}", filePath, id);
				return texture;
			}
			else
			{
				fs.close();
				fmt::println("Couldn't open file '{}' for reading. Failed to load shader asset.", filePath);
				throw std::runtime_error("");
				return nullptr;
			}
		}

	public:
		template<typename T>
		inline static std::shared_ptr<T> GetResource(size_t id)
		{
			auto it = m_Instance->m_Resources.find(id);
			if (it != m_Instance->m_Resources.end()) return std::static_pointer_cast<T>(it->second);

			// TODO: Add a custom exception class!
			else
			{
				fmt::println("Hash Id {} was not present in the Resource Pool.", id);
				throw std::runtime_error("");
			}
		}
		template<typename T>
		inline static std::shared_ptr<T> GetResource(const char* filePath)
		{
			return GetResource<T>(util::Crypto::DJB2Hash(filePath));
		}
		inline static const bool HasResource(size_t id)
		{
			auto it = m_Instance->m_Resources.find(id);
			if (it != m_Instance->m_Resources.end()) return true;
			return false;
		}
		inline static const bool HasResource(const char* filePath)
		{
			size_t id = util::Crypto::DJB2Hash(filePath);
			return HasResource(id);
		}
		inline static std::unordered_map<size_t, std::shared_ptr<IResource>>& GetAllResources()
		{
			return m_Instance->m_Resources;
		}
	};
}

#endif // CODEX_CORE_RESOURCE_HANDLER_H
