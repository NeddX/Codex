#ifndef CODEX_REFLECTION_REFLECTOR_H
#define CODEX_REFLECTION_REFLECTOR_H

#include <sdafx.h>

#include "../Core/Exception.h"
#include "../NativeBehaviour/NativeBehaviour.h"
#include "Lexer.h"

#define RF_CLASS(...)
#define RF_SERIALIZABLE(...)
#define RF_INSTANCE_CREATE(dlib, name) dlib->Invoke<NativeBehaviour*(const char*)>("Rf_CreateInstance", name)
#define RF_INSTANCE_CHECK(dlib, name)  dlib->Invoke<bool(const char*)>("Rf_DoesInstanceExist", name)
#define RF_GENERATE_BODY()                                                                                             \
private:                                                                                                               \
    friend CODEX_EXPORT codex::NativeBehaviour* Rf_CreateInstance(const char* className) noexcept;                     \
    void                                        Serialize() const noexcept override;                                   \
    codex::object                               GetField(const std::string_view fieldName) noexcept override;

extern "C" CODEX_EXPORT codex::NativeBehaviour* Rf_CreateInstance(const char* className) noexcept;
extern "C" CODEX_EXPORT bool                    Rf_DoesInstanceExist(const char* className) noexcept;
extern "C" CODEX_EXPORT void                    Rf_Dummy() noexcept;

namespace codex::rf {
    // Forward declarations.
    class RFFieldInfo;

    struct RFAttributeInfo
    {
        std::string name;
    };

    enum class RFType : u8
    {
        None,
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
        RFType                       type = RFType::None;
        std::string                  namescope;
        std::filesystem::path        file;
        std::string                  name;
        std::string                  qualifiers;
        std::vector<RFFieldInfo>     fields;
        std::vector<RFAttributeInfo> attributes;

    public:
        constexpr RFTypeInfo() noexcept = default;
        RFTypeInfo(const RFType type, const std::string_view name, const std::string_view qualifiers);
    };

    struct RFFieldInfo
    {
        std::string name;
        RFTypeInfo  type;
        std::string value;
    };

    class RFScript
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
        explicit RFScript(std::filesystem::path sourceFile);

    private:
    public:
        inline std::vector<RFTypeInfo>&       GetClasses() noexcept { return m_Classes; }
        inline const std::vector<RFTypeInfo>& GetClasses() const noexcept { return m_Classes; }
        inline std::filesystem::path          GetSourceFile() const noexcept { return m_SourceFile; }

    public:
        RFScript& Parse();
        RFScript& EmitMetadata(std::filesystem::path outputPath);

    private:
        std::optional<Token>       Consume() noexcept;
        std::optional<std::string> ExpectNamespace() noexcept;
        std::optional<RFTypeInfo>  ExpectRFClass() noexcept;
        std::optional<Token>       ExpectAttribute(const std::string_view attrbName) noexcept;
        std::optional<RFFieldInfo> ExpectRFField() noexcept;
        std::string                EmitClass(const RFTypeInfo& classInfo, const bool generatedIncludes) const noexcept;

    public:
        static void EmitBaseClass(const std::filesystem::path& outputPath, const std::vector<RFScript>& files);
    };

    template <typename T>
    u8 RFTypeOf() noexcept
    {
        if constexpr (std::is_same_v<T, unsigned char>)
            return (u8)RFType::U8;
        else if constexpr (std::is_same_v<T, signed char>)
            return (u8)RFType::I8;
        else if constexpr (std::is_same_v<T, float>)
            return (u8)RFType::F32;
        else if constexpr (std::is_same_v<T, Vector3f>)
            return (u8)RFType::Vector3f;
        else if constexpr (std::is_same_v<T, bool>)
            return (u8)RFType::Boolean;

        static_assert("Type not supported");
        return 0;
    }
    template <typename T>
    u8 RFTypeOf(T) noexcept
    {
        return RFTypeOf<T>();
    }
} // namespace codex::rf

#endif // CODEX_REFLECTION_REFLECTOR_H
