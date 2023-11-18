#include "Reflector.h"

#include "Lexer.h"

namespace codex {
    std::vector<RFTypeInfo> Reflector::GetClasses(const std::filesystem::path path)
    {
        std::vector<RFTypeInfo> classes;
        std::ifstream           fs(path);
        if (fs.is_open())
        {
            std::string contents((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));
            TokenList   tokens = Lexer::Lex(contents);
            for (usize i = 0; i < tokens.size(); ++i)
            {
                auto& token = tokens[i];
                switch (token.type)
                {
                    case TokenType::Identifier:
                        if (token.text == "class")
                        {
                            if (tokens[i + 1].type != TokenType::Identifier)
                                cx_throw(ReflectorBadSyntaxException,
                                         "Expected an identifier after the class keyword @ ({}, {})",
                                         tokens[i + 1].line, tokens[i + 1].cur);
                        }
                        break;
                    default: break;
                }
            }
        }
        else
            cx_throw(FileNotFoundException, "Couldn't open file {} for reading.", path.string());
        return classes;
    }
} // namespace codex
