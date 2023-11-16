#include "Lexer.h"

#define TK_IS_NOT_COMMENT_AND_STR(x)                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        if (m_CurrentToken.type != TokenType::Comment && m_CurrentToken.type != TokenType::BlockComment &&             \
            m_CurrentToken.type != TokenType::StringLiteral)                                                           \
            x                                                                                                          \
    } while (0);
#define TK_IS_NOT_COMMENT(x)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        if (m_CurrentToken.type != TokenType::Comment && m_CurrentToken.type != TokenType::BlockComment)               \
            x                                                                                                          \
    } while (0);
#define TK_IS_COMMENT_OR_STR(x)                                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        if (m_CurrentToken.type == TokenType::Comment || m_CurrentToken.type == TokenType::BlockComment ||             \
            m_CurrentToken.type == TokenType::StringLiteral)                                                           \
            x                                                                                                          \
    } while (0);
#define TK_IS_COMMENT(x)                                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        if (m_CurrentToken.type == TokenType::Comment || m_CurrentToken.type == TokenType::BlockComment)               \
            x                                                                                                          \
    } while (0);
#define TK_IS_BLOCK(x)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (m_CurrentToken.type == TokenType::Comment)                                                                 \
            x                                                                                                          \
    } while (0);
#define TK_EMPTY(x)                                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        if (m_CurrentToken.type == TokenType::Whitespace)                                                              \
            x                                                                                                          \
    } while (0);
#define TK_ADD(char) m_CurrentToken.text.append(1, char)

namespace codex {
    TokenList                           Lexer::m_Tokens{};
    Token                               Lexer::m_CurrentToken{};
    usize                               Lexer::m_Line                 = 0;
    usize                               Lexer::m_Cur                  = 0;
    usize                               Lexer::m_Index                = 0;
    std::unordered_map<char, TokenType> Lexer::m_SingleCharTokenMatch = {
        { '~', TokenType::Tilda },       { '!', TokenType::ExclamationMark }, { '@', TokenType::Dog },
        { '#', TokenType::Pound },       { '$', TokenType::DollarSign },      { '%', TokenType::PercentSign },
        { '^', TokenType::Caret },       { '&', TokenType::Ampersand },       { '*', TokenType::Asterisk },
        { '(', TokenType::OpenParen },   { ')', TokenType::CloseParen },      { '-', TokenType::Minus },
        { '+', TokenType::Plus },        { '_', TokenType::Underscore },      { '=', TokenType::Equal },
        { '[', TokenType::OpenSquare },  { ']', TokenType::CloseSquare },     { '{', TokenType::OpenCurly },
        { '}', TokenType::CloseCurly },  { '\\', TokenType::BackwardSlash },  { '/', TokenType::ForwardSlash },
        { ';', TokenType::Semicolon },   { ':', TokenType::Colon },           { '\'', TokenType::SingleQuote },
        { '"', TokenType::DoubleQuote }, { ',', TokenType::Comma },           { '.', TokenType::Dot },
        { '<', TokenType::OpenAngle },   { '>', TokenType::CloseAngle },
    };

    bool Lexer::SingleMatch(const char c) noexcept
    {
        const auto it = m_SingleCharTokenMatch.find(c);
        if (it != m_SingleCharTokenMatch.end())
        {
            TK_IS_COMMENT_OR_STR(return false;)
            if (m_CurrentToken.type == TokenType::Identifier && c == '_')
                return false;
            EndToken();
            m_CurrentToken = Token(it->second, m_Line, m_Cur);
            TK_ADD(c);
            EndToken();
            return true;
        }
        return false;
    }

    TokenList Lexer::Lex(const std::string_view src)
    {
        m_Tokens.clear();
        m_CurrentToken = Token();
        m_Line         = 0;
        m_Cur          = 0;
        m_Index        = 0;

        usize  len  = src.size();
        usize& line = m_Line;
        usize& cur  = m_Cur;

        for (usize i = 0; i < len; ++i)
        {
            const char c = src[i];
            switch (c)
            {
                case ' ':
                    if (m_CurrentToken.type != TokenType::Comment && m_CurrentToken.type != TokenType::BlockComment &&
                        m_CurrentToken.type != TokenType::StringLiteral)
                    {
                        EndToken();
                        break;
                    }
                    m_CurrentToken.text.append(1, c);
                    break;
                case '\n':
                    if (m_CurrentToken.type != TokenType::BlockComment &&
                        m_CurrentToken.type != TokenType::StringLiteral)
                    {
                        EndToken();
                        ++line;
                        cur = 0;
                        break;
                    }
                    m_CurrentToken.text.append(1, c);
                    break;

                case '"':
                    TK_IS_NOT_COMMENT({
                        TK_EMPTY({
                            m_CurrentToken = Token(TokenType::StringLiteral, line, cur);
                            TK_ADD(c);
                            break;
                        })
                        if (m_CurrentToken.type == TokenType::StringLiteral)
                            EndToken();
                    })
                    break;

                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if (m_CurrentToken.type == TokenType::Whitespace)
                        m_CurrentToken = Token(TokenType::NumbericLiteral, line, cur);
                    m_CurrentToken.text.append(1, c);
                    break;
                default:
                    if (SingleMatch(c))
                        break;

                    if (m_CurrentToken.type == TokenType::Whitespace)
                        m_CurrentToken = Token(TokenType::Identifier, line, cur);
                    TK_ADD(c);
                    break;
            }
            ++cur;
        }
        return m_Tokens;
    }

    void Lexer::Print(const TokenList& m_Tokens)
    {
        for (const auto& t : m_Tokens)
        {
            fmt::println("{{ type: {}, line: {}, cur: {}, text: \"{}\" }}", t.ToString(), t.line, t.cur, t.text);
        }
    }

    void Lexer::EndToken()
    {
        if (m_CurrentToken.type != TokenType::Whitespace)
        {
            m_Tokens.push_back(m_CurrentToken);
        }
        if (m_CurrentToken.type != TokenType::Comment || m_CurrentToken.type != TokenType::BlockComment) {}
        m_CurrentToken = Token();
    }
} // namespace codex
