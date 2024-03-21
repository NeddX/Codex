#ifndef CODEX_REFLECTION_REFLECTOR_H
#define CODEX_REFLECTION_REFLECTOR_H

#include <sdafx.h>

#include "../Core/Exception.h"
#include "../NativeBehaviour/NativeBehaviour.h"
#include "Lexer.h"

#define RF_CLASS(...)
#define RF_SERIALIZABLE(...)
#define RF_GENERATE_BODY()                                                                                             \
private:                                                                                                               \
    friend CODEX_EXPORT codex::NativeBehaviour* Rf_CreateInstance(const char* className) noexcept;                     \
    void                                        Serialize() override;                                                  \
    codex::object                               GetField(const std::string_view fieldName) override;

extern "C" CODEX_EXPORT codex::NativeBehaviour* Rf_CreateInstance(const char* className) noexcept;
extern "C" CODEX_EXPORT bool                    Rf_DoesInstanceExist(const char* className) noexcept;
extern "C" CODEX_EXPORT void                    Rf_Dummy() noexcept;

namespace codex::reflect {
    // Forward declarations.
    class RFFieldInfo;

    struct RFAttributeInfo
    {
        std::string name;
    };

    enum class RFType : u8
    {
        U8,
        I8,
        U16,
        I16,
        U32,
        I32,
        U64,
        I64,
        USize,
        Boolean,
        F32,
        F64,
        F128,
        Vector2f,
        Vector2,
        Vector3f,
        Vector3,
        Vector4f,
        Vector4,
        CString,
        StdString,
        UserDefined
    };

    class RFTypeInfo
    {
    public:
        RFType                       type;
        std::string                  namescope;
        std::filesystem::path        file;
        std::string                  name;
        std::string                  qualifiers;
        std::vector<RFFieldInfo>     fields;
        std::vector<RFAttributeInfo> attributes;

    public:
        RFTypeInfo() = default;
        RFTypeInfo(const RFType type, const std::string_view name, const std::string_view qualifiers);
    };

    struct RFFieldInfo
    {
        std::string name;
        RFTypeInfo  type;
        std::string value;
    };

    class Reflector
    {
    private:
        std::vector<RFTypeInfo> m_Classes;
        Lexer                   m_Lexer;
        std::string             m_SourceCode;
        std::optional<Token>    m_CurrentToken  = std::nullopt;
        Token                   m_PreviousToken = Token{ .type = TokenType::Eof };
        std::filesystem::path   m_SourceFile;
        std::filesystem::path   m_OutputPath;
        std::list<std::string>  m_Scopes;

    public:
        explicit Reflector(std::filesystem::path sourceFile);

    public:
        void EmitMetadata(std::filesystem::path outputPath);

    private:
        std::optional<Token>       Consume() noexcept;
        std::optional<std::string> ExpectNamespace() noexcept;
        std::optional<RFTypeInfo>  ExpectRFClass() noexcept;
        std::optional<Token>       ExpectAttribute(const std::string_view attrbName) noexcept;
        std::optional<RFFieldInfo> ExpectRFField() noexcept;
        std::string                EmitClass(const RFTypeInfo& classInfo, const bool generatedIncludes) const noexcept;

    public:
        static void EmitBaseClass(const std::filesystem::path& outputPath, const std::vector<Reflector>& files);
    };

    template <typename T>
    u8 RFTypeOf() noexcept
    {
        using base_type = std::remove_const_t<T>;

        if constexpr (std::is_same_v<T, unsigned char>)
            return (u8)RFType::U8;
        else if constexpr (std::is_same_v<T, signed char>)
            return (u8)RFType::I8;
        else if constexpr (std::is_same_v<T, float>)
            return (u8)RFType::F32;
        else if constexpr (std::is_same_v<T, Vector3f>)
            return (u8)RFType::Vector3f;

        static_assert("Type not supported");
        return {};
    }
    template <typename T>
    u8 RFTypeOf(T) noexcept
    {
        return RFTypeOf<T>();
    }
} // namespace codex::reflect

#endif // CODEX_REFLECTION_REFLECTOR_H
