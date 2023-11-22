#ifndef CODEX_REFLECTION_ANALYZER_H
#define CODEX_REFLECTION_ANALYZER_H

#include <sdafx.h>

#include "Lexer.h"

namespace codex {
    enum class FundamentalTypes : u8
    {
        I8,
        U8,
        I16,
        U16,
        I32,
        U32,
        I64,
        U64,
        Usize,
        F32,
        F64,
        F128,
        Void
    };

    struct Type
    {
    public:
        std::string       name;
        FundamentalTypes  type;
        std::vector<Type> field;
    };

    class Analyzer
    {
    private:
        TokenList::iterator                   m_CurrenToken;
        TokenList::iterator                   m_TokenEnd;
        std::unordered_map<std::string, Type> m_Types;
    };
} // namespace codex

#endif // CODEX_REFLECTION_ANALYZER_H
