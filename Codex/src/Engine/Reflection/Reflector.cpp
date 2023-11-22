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
            for (const auto& t : tokens) {}
        }
        else
            cx_throw(FileNotFoundException, "Couldn't open file {} for reading.", path.string());
        return classes;
    }
} // namespace codex
