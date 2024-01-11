#ifndef CODEX_REFLECTION_REFLECTOR_H
#define CODEX_REFLECTION_REFLECTOR_H

#include <sdafx.h>

#include "../Core/Exception.h"

namespace codex::reflect {
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
    public:
        enum RFType : u8
        {
            Undefined,
            Fundamental,
            UserDefined
        };

    private:
        RFType                       m_Type;
        std::filesystem::path        m_File;
        std::string                  m_FileContent;
        std::string                  m_TypeName;
        std::string                  m_Qualifiers;
        std::vector<RFFieldInfo>     m_Fields;
        std::vector<RFMethodInfo>    m_Methods;
        std::vector<RFAttributeInfo> m_Attributes;

    public:
        RFTypeInfo() = default;
        RFTypeInfo(const RFType type, const std::string_view name, const std::string_view qualifiers);
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
        std::string m_Value;
        std::string m_Qualifiers;
    };

    class Reflector
    {
    public:
    };

    template <typename T>
    RFTypeInfo RFTypeOf() noexcept
    {
        using base_type = std::remove_const_t<T>;

        std::string qualifiers;

        if constexpr (std::is_const<T>::value)
            qualifiers = "const";

        if constexpr (std::is_same_v<T, unsigned char>)
            return RFTypeInfo(RFTypeInfo::Fundamental, "unsigned char", qualifiers);
        else if constexpr (std::is_same_v<T, signed char>)
            return RFTypeInfo(RFTypeInfo::Fundamental, "signed char", qualifiers);
    }
} // namespace codex::reflect

#endif // CODEX_REFLECTION_REFLECTOR_H
