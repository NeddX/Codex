#ifndef CODEX_REFLECTION_LEXER_H
#define CODEX_REFLECTION_LEXER_H

#include <sdafx.h>

namespace codex::reflect {
    enum class TokenType : u8
    {
        Whitespace,
        Identifier,
        Semicolon,
        OpenCurly,
        CloseCurly,
        OpenParen,
        CloseParen,
        Colon,
        NumbericLiteral,
        PPDirective,
        DoubleQuote,
        SingleQuote,
        OpenAngle,
        CloseAngle,
        Asterisk,
        Plus,
        Minus,
        Ampersand,
        ForwardSlash,
        BackwardSlash,
        StringLiteral,
        Comment,
        BlockComment,
        QuestionMark,
        ExclamationMark,
        PercentSign,
        Tilda,
        DollarSign,
        Pound,
        OpenSquare,
        CloseSquare,
        Dog,
        Caret,
        Underscore,
        Comma,
        Dot,
        Equal
    };

    struct Token
    {
        TokenType   type = TokenType::Whitespace;
        usize       line = 0, cur = 0;
        std::string text;

    public:
        Token() = default;
        Token(const TokenType type, const usize line, const usize cur, const std::string_view text = {})
            : type(type), line(line), cur(cur), text(text)
        {
        }

    public:
        inline const char* ToString() const noexcept
        {
            static const char* const str_arr[] = {
                "Whitespace",    "Identifier",  "Semicolon",    "OpenCurly",     "CloseCurly",
                "OpenParen",     "CloseParen",  "Colon",        "NumberLiteral", "PPDirective",
                "DoubleQuote",   "SingleQuote", "OpenAngle",    "CloseAngle",    "Asterisk",
                "Plus",          "Minus",       "Ampersand",    "ForwardSlash",  "BackwardSlash",
                "StringLiteral", "Comment",     "BlockComment", "QuestionMark",  "ExclamationMark",
                "PercentSign",   "Tilda",       "DollarSign",   "Pound",         "OpenSquare",
                "CloseSquare",   "Dog",         "Caret",        "Underscore",    "Comma",
                "Dot",           "Equal"
            };
            return str_arr[(usize)type];
        }
    };

    using TokenList = std::vector<Token>;

    class Lexer
    {
    private:
        static TokenList                           m_Tokens;
        static Token                               m_CurrentToken;
        static std::unordered_map<char, TokenType> m_SingleCharTokenMatch;
        static usize                               m_Line;
        static usize                               m_Cur;
        static usize                               m_Index;

    public:
        static TokenList Lex(const std::string_view src);
        static void      Print(const TokenList& tokens);

    private:
        static bool SingleMatch(const char c) noexcept;
        static void EndToken();
    };
} // namespace codex::reflect

#endif // CODEX_REFLECTION_LEXER_H
