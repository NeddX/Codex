#include "Lexer.h"

namespace codex::rf {
    std::string_view TokenTypeToString(const TokenType type) noexcept
    {
        static const char* str[] = { "None",

                                     // Identifier
                                     "Identifier",

                                     // Literals
                                     "NumberLiteral", "StringLiteral", "CharacterLiteral",

                                     // Operators
                                     "Colon", "DoubleColon", "SemiColon", "Equals", "LeftBrace", "RightBrace",
                                     "LeftCurlyBrace", "RightCurlyBrace", "Plus", "Minus", "Asterisk", "ForwardSlash",
                                     "LeftAngleBracket", "RightAngleBracket", "LeftSquareBracket", "RightSquareBracket",
                                     "DoubleQuote", "Quote", "Comma", "Exclamation", "Bar", "Pound", "Dot", "Tilda",

                                     // Conditionals
                                     "EqualsEquals", "GreaterThanEquals", "LesserThanEquals", "ExclamationEquals",
                                     "AmpersandAmpersand", "BarBar",

                                     // Unary
                                     "Increment", "Decrement",

                                     // Shorthand
                                     "PlusEquals", "MinusEquals", "AsteriskEquals", "ForwardSlashEquals",

                                     // Keywords
                                     "KeywordChar", "KeywordBool", "KeywordShort", "KeywordInt", "KeywordFloat",
                                     "KeywordLong", "KeywordDouble", "KeywordConst", "KeywordConstexpr",
                                     "KeywordVolatile", "KeywordUnsigned", "KeywordSigned", "KeywordImport",
                                     "KeywordExport", "KeywordIf", "KeywordElse", "KeywordElseIf", "KeywordWhile",
                                     "KeywordReturn", "KeywordTrue", "KeywordFalse", "KeywordPublic", "KeywordPrivate",
                                     "KeywordFinal", "KeywordClass", "KeywordStruct", "KeywordNamespace",
                                     "KeywordUsing", "KeywordInline", "KeywordStatic",

                                     // Comments
                                     "LineComment", "BlockComment",

                                     // Eof
                                     "Eof" };
        return str[(usize)type];
    }

    Lexer::Lexer(const std::string_view source)
        : m_Source(source)
    {
    }

    std::optional<Token> Lexer::PeekToken()
    {
        const auto current_state = *this;
        auto       token         = NextToken();
        *this                    = std::move(current_state);
        return token;
    }

    std::optional<Token> Lexer::NextToken()
    {
        // If we've reached the end.
        if (m_CurrentPos == m_Source.size())
        {
            ++m_CurrentPos;
            return Token{ .type = TokenType::Eof };
        }
        else if (m_CurrentPos > m_Source.size())
            // Now we're past the end.
            return std::nullopt;

        auto  c             = CurrentChar();
        usize start         = m_CurrentPos;
        auto  current_token = Token();

        // Skip whitespace and friends
        while (c.has_value() && (*c == ' ' || *c == '\t' || *c == '\n' || *c == '\r'))
        {
            // Consume the whitespace.
            Consume();

            if (*c == '\n')
                // If it's a line then increment the line count.
                ++m_LineCount;

            // Progress to the next character and update start.
            c     = CurrentChar();
            start = m_CurrentPos;
        }

        // If our current character is valid.
        if (c.has_value())
        {
            // If it's a number start.
            if (std::isdigit(*c))
            {
                // Consume the number and set the type.
                auto result = ConsumeNumber();
                if (result) // If ConsumeNumber() returned a nullopt then the number literal is in a different number
                            // system. This parses only decimals.
                    current_token.num = *result;
                current_token.type = TokenType::NumberLiteral;
            }
            else if (IsIdentifierStart(*c)) // Else if it's a possible identifier.
            {
                auto ident = ConsumeIdentifier();

                // Check if it's a keyword.
                if (ident == "char")
                    current_token.type = TokenType::KeywordChar;
                else if (ident == "bool")
                    current_token.type = TokenType::KeywordBool;
                else if (ident == "short")
                    current_token.type = TokenType::KeywordShort;
                else if (ident == "int")
                    current_token.type = TokenType::KeywordInt;
                else if (ident == "float")
                    current_token.type = TokenType::KeywordFloat;
                else if (ident == "long")
                    current_token.type = TokenType::KeywordLong;
                else if (ident == "double")
                    current_token.type = TokenType::KeywordDouble;
                else if (ident == "const")
                    current_token.type = TokenType::KeywordConst;
                else if (ident == "constexpr")
                    current_token.type = TokenType::KeywordConstexpr;
                else if (ident == "volatile")
                    current_token.type = TokenType::KeywordVolatile;
                else if (ident == "unsigned")
                    current_token.type = TokenType::KeywordUnsigned;
                else if (ident == "signed")
                    current_token.type = TokenType::KeywordSigned;
                else if (ident == "import")
                    current_token.type = TokenType::KeywordImport;
                else if (ident == "export")
                    current_token.type = TokenType::KeywordExport;
                else if (ident == "if")
                    current_token.type = TokenType::KeywordIf;
                else if (ident == "else")
                    current_token.type = TokenType::KeywordElse;
                else if (ident == "else if")
                    current_token.type = TokenType::KeywordElseIf;
                else if (ident == "while")
                    current_token.type = TokenType::KeywordWhile;
                else if (ident == "return")
                    current_token.type = TokenType::KeywordReturn;
                else if (ident == "true")
                {
                    current_token.type = TokenType::KeywordTrue;
                    current_token.num  = 1;
                }
                else if (ident == "false")
                {
                    current_token.type = TokenType::KeywordFalse;
                    current_token.num  = 0;
                }
                else if (ident == "public")
                    current_token.type = TokenType::KeywordPublic;
                else if (ident == "private")
                    current_token.type = TokenType::KeywordPrivate;
                else if (ident == "final")
                    current_token.type = TokenType::KeywordFinal;
                else if (ident == "class")
                    current_token.type = TokenType::KeywordClass;
                else if (ident == "struct")
                    current_token.type = TokenType::KeywordStruct;
                else if (ident == "namespace")
                    current_token.type = TokenType::KeywordNamespace;
                else if (ident == "using")
                    current_token.type = TokenType::KeywordUsing;
                else if (ident == "inlie")
                    current_token.type = TokenType::KeywordInline;
                else
                    current_token.type = TokenType::Identifier;
            }
            else // Else it must be an operator.
            {
                current_token.type = ConsumeOperator();

                // Possible string or character literal.
                switch (current_token.type)
                {
                    case TokenType::LineComment: {
                        auto comment = ConsumeLineComment();

                        if (comment)
                        {
                            current_token.type = TokenType::LineComment;
                            current_token.span =
                                TextSpan{ .line = m_LineCount, .cur = m_TokenCount, .text = std::move(*comment) };
                            return current_token;
                        }
                        break;
                    }
                    case TokenType::BlockComment: {
                        auto comment = ConsumeBlockComment();

                        if (comment)
                        {
                            current_token.type = TokenType::BlockComment;
                            current_token.span =
                                TextSpan{ .line = m_LineCount, .cur = m_TokenCount, .text = std::move(*comment) };

                            // Consume '*/'
                            Consume();
                            Consume();

                            return current_token;
                        }
                        break;
                    }
                    case TokenType::DoubleQuote: {
                        // Consume the string.
                        auto str = ConsumeString();

                        // This is always going to be true but whatever I guess.
                        if (str)
                        {
                            // Since we know it's a string we can just handle it right away and move str into our token
                            // span.
                            ++m_TokenCount;
                            current_token.type = TokenType::StringLiteral;
                            current_token.span =
                                TextSpan{ .line = m_LineCount, .cur = m_TokenCount, .text = std::move(*str) };

                            return current_token;
                        }
                        break;
                    }
                    case TokenType::Quote: {
                        // Consume the possible character inside the single quotes.
                        auto c = Consume();
                        if (c)
                        {
                            // Consume the possible second quote.
                            auto ec = Consume();
                            if (ec == '\'')
                            {
                                // Since we know it's a character we can just handle it right away.
                                ++m_TokenCount;
                                current_token.type = TokenType::CharacterLiteral;
                                current_token.num  = *ec;
                                current_token.span = TextSpan{ .line = m_LineCount, .cur = m_TokenCount };
                                current_token.span.text += *c;

                                return current_token;
                            }
                            else
                                throw std::runtime_error("Compile Error: Expected a closing single quote.");
                        }
                        break;
                    }
                    default: break;
                }
            }
        }
        else
        {
            // If our current character is invalid then we've reached the end.
            ++m_CurrentPos;
            return Token{ .type = TokenType::Eof };
        }

        // Increment the token count and create our text span which will hold the line number, cursor number and the
        // text itself.
        ++m_TokenCount;
        usize end          = m_CurrentPos;
        current_token.span = TextSpan{ .line = m_LineCount,
                                       .cur  = m_TokenCount,
                                       .text = std::string{ m_Source.substr(start, end - start) } };

        return current_token;
    }

    std::optional<char> Lexer::CurrentChar() const noexcept
    {
        // Bounds checking.
        if (m_Source.size() > m_CurrentPos)
            return m_Source[m_CurrentPos];
        else
            return std::nullopt;
    }

    std::optional<char> Lexer::Consume() noexcept
    {
        // Progress to the next character and return the previous or nothing if we've reached the end.
        if (m_CurrentPos >= m_Source.size())
            return std::nullopt;
        auto c = CurrentChar();
        ++m_CurrentPos;
        return c;
    }

    std::optional<i64> Lexer::ConsumeNumber() noexcept
    {
        // Consume the number digit by digit until we hit a non-digit character.
        i64 num = 0;
        for (auto c = CurrentChar(); c.has_value(); c = CurrentChar())
        {
            if (std::isdigit(*c))
            {
                // If c was valid then Consume the current character then store c into num.
                Consume();
                num = num * 10 + (*c - '0');
            }
            else
            {
                const char lc = std::tolower(*c);
                switch (lc)
                {
                    case 'f':
                    case 'l':
                    case 'u':
                    case 'x':
                    case '.':
                    case 'b': Consume(); break;
                    default: goto lb_01;
                }
            }
        }
        return num;
lb_01:
        return std::nullopt;
    }

    std::string Lexer::ConsumeIdentifier() noexcept
    {
        std::string ident{};
        for (auto c = CurrentChar(); c.has_value(); c = CurrentChar())
        {
            if ((ident.empty() && std::isalpha(*c)) || (!ident.empty() && std::isalnum(*c)) || *c == '_')
            {
                // If our identifier starts with a letter or contains a possible letter or a number, or it's just an
                // underscore then Consume the current character, append the previous to ident, then continue.
                Consume();
                ident += *c;
            }
            else
                break;
        }
        return ident;
    }

    TokenType Lexer::ConsumeOperator() noexcept
    {
        // We know that this is guaranteed to be an operator so don't even bother with null checking and just unwrap
        // straight away.
        auto c = *Consume();

        // For checking double operators.
        auto p = CurrentChar();
        switch (c)
        {
            using enum TokenType;

            case ':':
                if (p)
                {
                    switch (*p)
                    {
                        case ':': Consume(); return DoubleColon;
                        default: break;
                    }
                }
                return Colon;
            case ';': return SemiColon;
            case '+':
                if (p)
                {
                    switch (*p)
                    {
                        case '+': Consume(); return Increment;
                        case '=': Consume(); return PlusEquals;
                        default: break;
                    }
                }
                return Plus;
            case '-':
                if (p)
                {
                    switch (*p)
                    {
                        case '-': Consume(); return Decrement;
                        case '=': Consume(); return MinusEquals;
                        default: break;
                    }
                }
                return Minus;
            case '*':
                if (p)
                {
                    switch (*p)
                    {
                        case '=': Consume(); return AsteriskEquals;
                        default: break;
                    }
                }
                return Asterisk;
            case '/':
                if (p)
                {
                    switch (*p)
                    {
                        case '=': Consume(); return ForwardSlashEquals;
                        case '/': Consume(); return LineComment;
                        case '*': Consume(); return BlockComment;
                        default: break;
                    }
                }
                return ForwardSlash;
            case '=':
                if (p)
                {
                    switch (*p)
                    {
                        case '=': Consume(); return EqualsEquals;
                        default: break;
                    }
                }
                return Equals;
            case '!':
                if (p)
                {
                    switch (*p)
                    {
                        case '=': Consume(); return ExclamationEquals;
                        default: break;
                    }
                }
                return Exclamation;
            case '|':
                if (p)
                {
                    switch (*p)
                    {
                        case '|': Consume(); return BarBar;
                        default: break;
                    }
                }
                return Bar;
            case '<':
                if (p)
                {
                    switch (*p)
                    {
                        case '=': Consume(); return LesserThanEquals;
                        default: break;
                    }
                }
                return LeftAngleBracket;
            case '>':
                if (p)
                {
                    switch (*p)
                    {
                        case '=': Consume(); return GreaterThanEquals;
                        default: break;
                    }
                }
                return RightAngleBracket;
            case '(': return LeftBrace;
            case ')': return RightBrace;
            case '{': return LeftCurlyBrace;
            case '}': return RightCurlyBrace;
            case '[': return LeftSquareBracket;
            case ']': return RightSquareBracket;
            case '"': return DoubleQuote;
            case '\'': return Quote;
            case ',': return Comma;
            case '#': return Pound;
            case '.': return Dot;
            case '~': return Tilda;
            default: break;
        }
        return TokenType::None;
    }

    std::optional<std::string> Lexer::ConsumeString() noexcept
    {
        std::string str;
        for (auto c = CurrentChar(); c; c = CurrentChar())
        {
            // Consume the string until we hit a double quote.
            Consume();
            if (*c != '"')
                str += *c;
            else
                break;
        }

        // If the string was invalid then return nothing, or str otherwise.
        if (str.empty())
            return std::nullopt;
        else
            return str;
    }

    std::optional<std::string> Lexer::ConsumeLineComment() noexcept
    {
        std::string str;
        for (auto c = CurrentChar(); c; c = CurrentChar())
        {
            Consume();
            if (*c != '\n')
                str += *c;
            else
                break;
        }
        if (str.empty())
            return std::nullopt;
        else
            return str;
    }

    std::optional<std::string> Lexer::ConsumeBlockComment() noexcept
    {
        std::string str;
        for (auto c = CurrentChar(); c; c = CurrentChar())
        {
            Consume();
            if (*c != '*')
                str += *c;
            else
            {
                const auto n = CurrentChar();
                if (!n || *n == '/')
                    break;
                str += *c;
            }
        }
        if (str.empty())
            return std::nullopt;
        else
            return str;
    }

    bool Lexer::IsIdentifierStart(const char c) const noexcept
    {
        // Identifiers start with a letter or an underscore followed by more letters, underscores and numbers.
        return (std::isalpha(c) || c == '_');
    }
} // namespace codex::rf

std::ostream& operator<<(std::ostream& stream, const codex::rf::TextSpan& span) noexcept
{
    stream << "{ Text: '" << span.text << "', Line: " << span.line << ", Cursor: " << span.cur << " }";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const codex::rf::Token& token) noexcept
{
    stream << "{ Type: " << TokenTypeToString(token.type) << ", NumberLiteral: " << token.num
           << ", Span: " << token.span << " }";
    return stream;
}
