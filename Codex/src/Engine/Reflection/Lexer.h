#ifndef CODEX_REFLECTION_LEXER_H
#define CODEX_REFLECTION_LEXER_H

#include <sdafx.h>

namespace codex {
    enum class TokenType : u8
    {
        Whitespace,
        Identifier,
        Semicolon,
        OpenCurlyBrace,
        CloseCurlyBrace,
        OpenParen,
        CloseParen,
        Colon,
        NumberLiteral,
        PPDirective,
        DoubleQuote,
        SingleQuote,
        OpenAngleBracket,
        CloseAngleBracket,
        Asterisk,
        Plus,
        Minus,
        Ampersand,
        ForwardSlash,
        BackwardSlash,
        StringLiteral,
        Comment,
        BlockComment
    };

    struct Token
    {
        TokenType   type = TokenType::Whitespace;
        usize       line = 0, cur = 0;
        std::string text;

    public:
        inline const char* ToString() const noexcept
        {
            static const char* const str_arr[] = { "Whitespace",
                                                   "Identifier",
                                                   "Semicolon",
                                                   "OpenCurlyBarce",
                                                   "CloseCurlyBarce",
                                                   "OpenParen",
                                                   "CloseParen",
                                                   "Colon",
                                                   "NumberLiteral",
                                                   "PPDirective",
                                                   "DoubleQuote",
                                                   "SingleQuote",
                                                   "OpenAngleBracket",
                                                   "loseAngleBracket",
                                                   "Asterisk",
                                                   "Plus",
                                                   "Minus",
                                                   "Ampersand",
                                                   "ForwardSlash",
                                                   "ackwardSlash",
                                                   "StringLiteral",
                                                   "Comment",
                                                   "BlockComment" };
            return str_arr[(usize)type];
        }
    };

    using TokenList = std::vector<Token>;

    class Lexer
    {
    public:
        static TokenList Lex(const std::string_view src);
        static void      Print(const TokenList& tokens);
    };
} // namespace codex

#endif // CODEX_REFLECTION_LEXER_H
