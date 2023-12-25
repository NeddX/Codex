#include "Reflector.h"

#include "../Core/Exception.h"
#include "Lexer.h"

namespace codex {
    std::vector<RFTypeInfo> Reflector::GetClasses(const std::filesystem::path path)
    {
        std::vector<RFTypeInfo> classes;
        std::ifstream           fs(path);
        if (fs.is_open()) {}
        else
            cx_throw(FileNotFoundException, "Couldn't open file {} for reading.", path.string());
        return classes;
    }
} // namespace codex
