#ifndef CODEX_REFLECTION_REFLECTOR_H
#define CODEX_REFLECTION_REFLECTOR_H

#include <sdafx.h>

#include "../Core/Exception.h"

namespace codex {
    // Forward declaration.
    class RFFieldInfo;
    class RFMethodInfo;

    CX_CUSTOM_EXCEPTION(ReflectorBadSyntaxException, "Bad syntax")

    class RFAttributeInfo
    {
    private:
        std::string m_Name;
    };

    class RFTypeInfo
    {
    private:
        std::string                  m_TypeName;
        std::vector<RFFieldInfo>     m_Fields;
        std::vector<RFMethodInfo>    m_Methods;
        std::vector<RFAttributeInfo> m_Attributes;
    };

    class RFMethodInfo
    {
    private:
        std::string m_Name;
        RFTypeInfo  m_ReturnType;
    };

    class RFFieldInfo
    {
    private:
        std::string m_Name;
        RFTypeInfo  m_Type;
    };

    class Reflector
    {
    public:
        static std::vector<RFTypeInfo> GetClasses(const std::filesystem::path file);
    };
} // namespace codex

#endif // CODEX_REFLECTION_REFLECTOR_H
