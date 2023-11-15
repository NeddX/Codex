#include "Lexer.h"

namespace codex {
    TokenList Lexer::Lex(const std::string_view src)
    {
        TokenList tokens;
        usize     len  = src.size();
        usize     line = 0;
        usize     cur  = 0;
        Token     current_token;

        for (usize i = 0; i < len; ++i)
        {
            const char c = src[i];
            switch (c)
            {
                case '\n':
                    if (current_token.type != TokenType::BlockComment && current_token.type != TokenType::StringLiteral)
                    {
                        tokens.push_back(current_token);
                        current_token = Token();
                    }
                    ++line;
                    cur = 0;
                    break;
                case ' ':
                    if (current_token.type != TokenType::Comment && current_token.type != TokenType::BlockComment &&
                        current_token.type != TokenType::StringLiteral && current_token.type != TokenType::PPDirective)
                    {
                        tokens.push_back(current_token);
                        current_token = Token();
                    }
                    else
                        current_token.text.append(1, c);
                    break;
                case '*':
                    if (current_token.type == TokenType::BlockComment && src[i + 1] == '/')
                    {
                        if (src[i + 1] == '/')
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                    }
                    else if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::Asterisk;
                        current_token.line = line;
                        current_token.cur  = cur;
                        current_token.text.append(1, c);
                        tokens.push_back(current_token);
                        current_token = Token();
                    }
                    else
                        current_token.text.append(1, c);
                    break;
                case '/':
                    if (src[i + 1] == '/' && current_token.type == TokenType::Whitespace ||
                        current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::Comment;
                        current_token.line = line;
                        current_token.cur  = cur;
                    }
                    else if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::ForwardSlash;
                        current_token.line = line;
                        current_token.cur  = cur;
                    }
                    current_token.text.append(1, c);
                    break;
                case '+':
                    if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::Plus;
                        current_token.line = line;
                        current_token.cur  = cur;
                    }
                case '-':
                    if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::Minus;
                        current_token.line = line;
                        current_token.cur  = cur;
                    }
                    current_token.text.append(1, c);
                    break;
                case ':':
                    if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::Colon;
                        current_token.line = line;
                        current_token.cur  = cur;
                    }
                case ';':
                    if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::Semicolon;
                        current_token.line = line;
                        current_token.cur  = cur;
                    }
                    current_token.text.append(1, c);
                    break;
                case '#':
                    if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::PPDirective;
                        current_token.line = line;
                        current_token.cur  = cur;
                    }
                    current_token.text.append(1, c);
                    break;
                case '{':
                    if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::OpenCurlyBrace;
                        current_token.line = line;
                        current_token.cur  = cur;
                        current_token.text.append(1, c);
                        tokens.push_back(current_token);
                        current_token = Token();
                        break;
                    }
                case '}':
                    if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::CloseCurlyBrace;
                        current_token.line = line;
                        current_token.cur  = cur;
                        current_token.text.append(1, c);
                        tokens.push_back(current_token);
                        current_token = Token();
                        break;
                    }
                    current_token.text.append(1, c);
                    break;
                case '(':
                    if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::OpenParen;
                        current_token.line = line;
                        current_token.cur  = cur;
                        current_token.text.append(1, c);
                        tokens.push_back(current_token);
                        current_token = Token();
                        break;
                    }
                case ')':
                    if (current_token.type == TokenType::Whitespace || current_token.type == TokenType::Identifier)
                    {
                        if (current_token.type == TokenType::Identifier)
                        {
                            tokens.push_back(current_token);
                            current_token = Token();
                        }
                        current_token.type = TokenType::CloseParen;
                        current_token.line = line;
                        current_token.cur  = cur;
                        current_token.text.append(1, c);
                        tokens.push_back(current_token);
                        current_token = Token();
                        break;
                    }
                    current_token.text.append(1, c);
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
                    if (current_token.type == TokenType::Whitespace)
                    {
                        current_token.type = TokenType::NumberLiteral;
                        current_token.line = line;
                        current_token.cur  = cur;
                    }
                    current_token.text.append(1, c);
                    break;
                default:
                    if (current_token.type == TokenType::Whitespace)
                    {
                        current_token.type = TokenType::Identifier;
                        current_token.line = line;
                        current_token.cur  = cur;
                    }
                    current_token.text.append(1, c);
                    break;
            }
            ++cur;
        }
        return tokens;
    }

    void Lexer::Print(const TokenList& tokens)
    {
        for (const auto& t : tokens)
        {
            fmt::println("{{ type: {}, line: {}, cur: {}, text: \"{}\" }}", t.ToString(), t.line, t.cur, t.text);
        }
    }
} // namespace codex
