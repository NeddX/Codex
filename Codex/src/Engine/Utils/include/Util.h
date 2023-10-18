#ifndef CODEX_UTIL_H
#define CODEX_UTIL_H

#include <sdafx.h>

namespace codex::util
{
	struct File
	{
		static std::string ReadToString(const char* filePath)
		{
			std::ifstream fs(filePath);
			std::stringstream buffer;
			if (fs.is_open())
			{
				buffer << fs.rdbuf();
				fs.close();
			}
			return buffer.str();
		}
	};

	struct Crypto
	{
		static u32 DJB2Hash(const std::string_view str)
		{
			u32 hash = 5381;
			for (usize i = 0; i < str.length(); i++)
			{
				hash = ((hash << 5) + hash) + str[i];
			}
			return hash;
		}
	};

	struct U824Id
	{
	private:
		u32 m_Id = 0;

	public:
		inline u32 GetId()
		{
			return m_Id & 0x00ffffff;
		}
		inline u8 GetGen()
		{
			return m_Id >> 24;
		}
		inline void SetId(const u32 id)
		{
			m_Id &= 0xff000000;
			m_Id |= id;
		}
		inline void SetGen(const u8 gen)
		{
			m_Id &= 0x00ffffff;
			m_Id |= (u32)(gen) << 24;
		}
	};
}

#endif // CODEX_UTIL_H
