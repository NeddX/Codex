#ifndef CODEX_UTIL_H
#define CODEX_UTIL_H

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <vector>
#include <string>

namespace util
{
	struct File
	{
		static char* ReadToString(const char* filePath)
		{
			FILE* file_ptr = fopen(filePath, "r");
			size_t file_size = 0;
			char* file_content = NULL;
			
			if (file_ptr)
			{
				fseek(file_ptr, 0, SEEK_END);
				file_size = ftell(file_ptr);
				fseek(file_ptr, 0, SEEK_SET);

				if (file_size > 0)
				{
					file_content = new char[file_size];
					if (file_content)
					{
						memset(file_content, 0, sizeof(char) * file_size);
						fread(file_content, sizeof(char), sizeof(char) * file_size, file_ptr);
					}
				}
				fclose(file_ptr);
			}
			return file_content;
		}

		static void ParseShaderFile(const char* filePath, char* vertexShader, char* fragmentShader = NULL)
		{
			char* file_src = ReadToString(filePath);
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

	struct U824ID
	{
	private:
		uint32_t m_ID = 0;

	public:
		inline uint32_t GetID()
		{
			return m_ID & 0x00ffffff;
		}
		inline uint8_t GetGen()
		{
			return m_ID >> 24;
		}
		inline void SetID(uint32_t id)
		{
			m_ID &= 0xff000000;
			m_ID |= id;
		}
		inline void SetGen(uint8_t gen)
		{
			m_ID &= 0x00ffffff;
			m_ID |= (uint32_t)(gen) << 24;
		}
	};
}

#endif // CODEX_UTIL_H
