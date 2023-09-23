#ifndef CODEX_UTIL_H
#define CODEX_UTIL_H

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace util
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

		static void ParseShaderFile(const char* filePath, char* const vertexShader, char* const fragmentShader = NULL)
		{
			std::string file_src = ReadToString(filePath);
			char* fragment_shader_src = NULL;
			char* vertex_shader_src = NULL;


			enum TokenType
			{
				MACRO,
				SRC
			};
			struct Token
			{
				TokenType Type = SRC;
				std::string Text = "";

			public:
				void Reset()
				{
					Type = SRC;
					Text = "";
				}
			};

			size_t i = 0;
			Token current_token;
			std::vector<Token> tokens;
			while (file_src[i] != 0)
			{
				if (file_src[i] != 0)
				{
					if (file_src[i] == '#' && current_token.Type == SRC)
					{
						tokens.push_back(current_token);
						current_token.Reset();
						current_token.Type = MACRO;
						current_token.Text.append(1, file_src[i]);
					}
					else if (file_src[i] == '\n')
					{
						tokens.push_back(current_token);
						current_token.Reset();
						current_token.Type = SRC;
					}
					else if (current_token.Type == SRC)
					{
						current_token.Text.append(1, file_src[i]);
					}
				}
				i++;
			}
		}
	};

	struct Crypto
	{
		static uint32_t DJB2Hash(const std::string& str)
		{
			uint32_t hash = 5381;
			for (size_t i = 0; i < str.length(); i++)
			{
				hash = ((hash << 5) + hash) + str[i];
			}
			return hash;
		}
	};

	struct U824Id
	{
	private:
		uint32_t m_Id = 0;

	public:
		inline uint32_t GetId()
		{
			return m_Id & 0x00ffffff;
		}
		inline uint8_t GetGen()
		{
			return m_Id >> 24;
		}
		inline void SetId(uint32_t id)
		{
			m_Id &= 0xff000000;
			m_Id |= id;
		}
		inline void SetGen(uint8_t gen)
		{
			m_Id &= 0x00ffffff;
			m_Id |= (uint32_t)(gen) << 24;
		}
	};
}

#endif // CODEX_UTIL_H
