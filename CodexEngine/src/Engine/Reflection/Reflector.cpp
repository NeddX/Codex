#include "Reflector.h"

#include <Engine/Core/Exception.h>

#include "Lexer.h"

namespace codex::rf {
    namespace fs = std::filesystem;

    RFTypeInfo::RFTypeInfo(const RFType type, const std::string_view name, const std::string_view qualifiers)
        : type(type)
        , name(name)
        , qualifiers(qualifiers)
    {
    }

    RFScript::RFScript(fs::path sourceFile)
        : m_SourceFile(std::move(sourceFile))
    {
        std::ifstream fs(m_SourceFile);
        if (fs.is_open())
        {
            m_SourceCode = std::string((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));
            m_Lexer      = Lexer(m_SourceCode);
        }
        else
            cx_throwd(FileNotFoundException);
    }

    RFScript& RFScript::Parse()
    {
        m_CurrentToken = m_Lexer.NextToken();
        while (m_CurrentToken->IsValid())
        {
            if (m_CurrentToken->type == TokenType::LeftCurlyBrace)
            {
                m_Scopes.emplace_back("#");
                Consume();
            }
            else if (m_CurrentToken->type == TokenType::RightCurlyBrace)
            {
                m_Scopes.pop_back();
                Consume();
            }
            else if (auto possible_namespace = ExpectNamespace(); possible_namespace)
                m_Scopes.emplace_back(std::move(*possible_namespace));
            else if (auto type = ExpectRFClass(); type)
                m_Classes.push_back(std::move(*type));
            else
                m_PreviousToken = *Consume();
        }
        return *this;
    }

    RFScript& RFScript::EmitMetadata(fs::path outputPath)
    {
        m_OutputPath = std::move(outputPath);

        if (!fs::exists(m_OutputPath))
            cx_throw(DirectoryNotFoundException, "Provided directory at: {} does not exist.", m_OutputPath.string());

        bool          generate_includes = true;
        const auto    file_path = m_OutputPath / (m_SourceFile.filename().replace_extension().string() + ".cxr.cpp");
        std::ofstream fs(file_path, std::ios::in | std::ios::trunc);
        if (fs.is_open())
        {
            for (const auto& e : m_Classes)
            {
                const auto content = EmitClass(e, generate_includes);
                if (generate_includes)
                    generate_includes = false;
                fs << content;
            }
        }
        else
            cx_throw(IOException, "Failed to write to: {}.", file_path.string());

        return *this;
    }

    std::optional<Token> RFScript::Consume() noexcept
    {
        const auto token = m_CurrentToken;
        if (m_CurrentToken)
            m_CurrentToken = m_Lexer.NextToken();
        return token;
    }

    std::optional<std::string> RFScript::ExpectNamespace() noexcept
    {
        std::string namespaceName;

        if (m_CurrentToken->type != TokenType::KeywordNamespace)
        {
            // return back to before parsing the namespace
            return std::nullopt;
        }
        else if (m_PreviousToken.type != TokenType::KeywordUsing)
        {
            Consume(); // Consume 'namespace'

            // Process namespace identifiers
            while (m_CurrentToken->type == TokenType::Identifier)
            {
                namespaceName += m_CurrentToken->span.text;
                Consume();

                if (m_CurrentToken->type == TokenType::DoubleColon)
                {
                    namespaceName += "::";
                    Consume(); // Consume '::'
                }
                else
                    break; // Break if no more nested namespaces
            }

            Consume(); // Consume the '{'
        }

        if (!namespaceName.empty())
        {
            // Add our namespace to our scope stack.
            return namespaceName;
        }
        else
            return std::nullopt;
    }

    std::optional<RFTypeInfo> RFScript::ExpectRFClass() noexcept
    {
        RFTypeInfo type_info;

        // Consume all tokens until we reach an identifier.
        if (!ExpectAttribute("RF_CLASS"))
            return std::nullopt;

        while (m_CurrentToken->type != TokenType::KeywordClass)
            Consume();

        if (m_CurrentToken->IsValid())
        {
            // Consume the class keyword token.
            Consume();

            std::optional<Token> ident = std::nullopt;
            while (m_CurrentToken->type != TokenType::Identifier)
                Consume();

            while (m_CurrentToken->type == TokenType::Identifier)
                ident = Consume();

            type_info.type = RFType::UserDefined;
            type_info.name = std::move(ident->span.text);

            while (m_CurrentToken->IsValid())
            {
                if (auto field = ExpectRFField(); field)
                    type_info.fields.push_back(std::move(*field));
                else
                {
                    // If it's a scope.
                    if (m_CurrentToken->type == TokenType::LeftCurlyBrace)
                    {
                        if (type_info.namescope.empty())
                        {
                            m_Scopes.emplace_back(type_info.name);

                            for (const auto& e : m_Scopes)
                            {
                                if (e[0] != '#')
                                    type_info.namescope += e + "::";
                            }
                            type_info.namescope.erase(type_info.namescope.size() - 2);
                        }
                        else
                            m_Scopes.emplace_back("#");
                    }
                    else if (m_CurrentToken->type == TokenType::RightCurlyBrace)
                    {
                        // Meaning our class' scope has ended so terminate.
                        if (m_Scopes.back() == type_info.name)
                        {
                            Consume();
                            m_Scopes.pop_back();
                            break;
                        }
                        m_Scopes.pop_back();
                    }
                    Consume();
                }
            }

            return type_info;
        }
        return std::nullopt;
    }

    std::optional<Token> RFScript::ExpectAttribute(const std::string_view attrbName) noexcept
    {
        if (m_CurrentToken->type == TokenType::Identifier)
        {
            auto prev_state = m_Lexer;
            auto prev_token = m_CurrentToken;
            auto ident      = Consume();

            if (ident->span.text == attrbName)
            {
                if (m_CurrentToken->type == TokenType::LeftBrace)
                    Consume();

                if (m_CurrentToken->type == TokenType::RightBrace)
                {
                    Consume();
                    return ident;
                }
            }
            m_Lexer        = std::move(prev_state);
            m_CurrentToken = prev_token;
        }
        return std::nullopt;
    }

    std::optional<RFFieldInfo> RFScript::ExpectRFField() noexcept
    {
        if (ExpectAttribute("RF_SERIALIZABLE"))
        {
            RFFieldInfo field;

            // Get the field name.
            Token ident;
            while (m_CurrentToken->type != TokenType::SemiColon)
            {
                if (m_CurrentToken->type == TokenType::Identifier)
                    ident = *m_CurrentToken;
                Consume();
            }

            field.name = std::move(ident.span.text);
            return field;
        }
        return std::nullopt;
    }

    std::string RFScript::EmitClass(const RFTypeInfo& classInfo, const bool generateIncludes) const noexcept
    {
        std::string content;

        if (generateIncludes)
        {
            content += "// Auto-generated by Codex Reflector for " + m_SourceFile.string();
            content += "\n\n";
            content += "#include \"" + fs::relative(m_SourceFile, m_OutputPath).string() + "\"\n\n";
            content += "#include <string>\n";
            content += "#include <string_view>\n";
            content += "#include <Codex.h>\n\n";
        }

        // Serialize()
        {
            content += "void " + classInfo.namescope + "::Serialize() const noexcept\n{\n";

            content += fmt::format(R"( m_SerializedData["{}"]["{}"] = {};)", classInfo.name, "Id", 0);
            content += '\n';

            for (const auto& e : classInfo.fields)
            {
                content += fmt::format(R"( m_SerializedData["{}"]["{}"]["{}"]["{}"] = codex::rf::RFTypeOf({});)",
                                       classInfo.name, "Fields", e.name, "Type", e.name);
                content += '\n';
                content += fmt::format(R"( m_SerializedData["{}"]["{}"]["{}"]["{}"] = {};)", classInfo.name, "Fields",
                                       e.name, "Value", e.name);
                content += '\n';
            }

            content += "}\n";
        }

        content += '\n';

        // GetField()
        {
            content += "codex::object " + classInfo.namescope + "::GetField(const std::string_view name) noexcept\n{\n";

            bool first_if = true;
            for (const auto& e : classInfo.fields)
            {
                if (first_if)
                {
                    content += fmt::format(R"( if (name == "{}"))", e.name);
                    first_if = false;
                }
                else
                    content += fmt::format(R"( else if (name == "{}"))", e.name);
                content += fmt::format("\n  return &{};\n", e.name);
            }

            content += " return codex::nullobj;\n";
            content += "}\n";
        }

        content += '\n';

        return content;
    }

    void RFScript::EmitBaseClass(const fs::path& outputPath, const std::vector<RFScript>& files)
    {
        const auto file_path = outputPath / "Rf_Base.cxr.cpp";
        auto       fs        = std::ofstream(file_path);
        if (fs.is_open())
        {
            fs << "// Auto-generated by Codex Reflector.";
            fs << "\n\n";
            // fs << "#include \"" + fs::relative(m_SourceFile, m_OutputPath).string() + "\"\n\n";

            for (const auto& e : files)
                fs << "#include \"" + fs::relative(e.m_SourceFile, outputPath).string() + "\"\n";

            fs << '\n';

            fs << "#include <cstdio>\n";
            fs << "#include <Codex.h>\n\n";

            // Dummy
            {
                fs << "void Rf_Dummy() noexcept { std::puts(\"dummy was called.\"); }\n";
            }

            fs << '\n';

            // Rf_DoesInstanceExist()
            {
                fs << "bool Rf_DoesInstanceExist(const char* className) noexcept\n{\n";

                bool first_if = true;
                for (const auto& f : files)
                {
                    for (const auto& e : f.m_Classes)
                    {
                        if (first_if)
                        {
                            fs << fmt::format(R"( if (std::strcmp(className, "{}") == 0))", e.name);
                            first_if = false;
                        }
                        else
                            fs << fmt::format(R"( else if (std::strcmp(className, "{}") == 0))", e.name);
                        fs << fmt::format("\n  return true;\n", e.name);
                    }
                }

                fs << " return false;\n";
                fs << "}\n";
            }

            fs << '\n';

            // Rf_CreateInstance()
            {
                fs << "codex::NativeBehaviour* Rf_CreateInstance(const char* className, codex::Entity parent) noexcept\n{\n";

                bool first_if = true;
                for (const auto& f : files)
                {
                    for (const auto& e : f.m_Classes)
                    {
                        if (first_if)
                        {
                            fs << fmt::format(R"( if (std::strcmp(className, "{}") == 0))", e.name);
                            first_if = false;
                        }
                        else
                            fs << fmt::format(R"( else if (std::strcmp(className, "{}") == 0))", e.name);
                        fs << "\n {\n";
                        fs << fmt::format("  auto* ptr = new {};\n", e.name);
                        fs << "  ptr->m_Parent = std::move(parent);\n";
                        fs << "  return ptr;\n";
                        fs << " }\n";
                    }
                }

                fs << " return nullptr;\n";
                fs << "}\n";
            }

            fs << '\n';
        }
        else
            cx_throw(IOException, "Failed to write to: {}.", file_path.string());
    }
} // namespace codex::rf
