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

		static void ParseShaderFile(const char* filePath, char* const vertexShader, char* const fragmentShader = NULL)
		{
			std::string file_src = ReadToString(filePath);

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

			usize i = 0;
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
		static u32 DJB2Hash(const std::string& str)
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
