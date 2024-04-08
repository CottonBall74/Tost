// a lexer for the language
// lazer is a program that takes a string of characters and converts it into a sequence of tokens

#pragma once

#include <iostream>
#include <string>


namespace Tost
{
    // the tokens that are available in Tost
    enum class TokenType
    {
        LOG = 0,
        RETURN,

        IF,
        ELSE_IF,
        ELSE,

        WHILE,
        FOR,
        
        INT,
        FLOAT,
        STRING,
        CHAR,
        BOOL,

        IDENTIFIER,
        NUMBER,

        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,

        ASSIGN,
        EQUAL,
        NOT_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,

        AND,
        OR,
        NOT,

        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACE,
        RIGHT_BRACE,
        LEFT_BRACKET,
        RIGHT_BRACKET,

        SEMICOLON,
        COMMA,
        COLON,
        DOT,

        ARROW,
        EOF_TOKEN,

        UNKNOWN
    };

    // how a token is represented
    struct Token
    {
        TokenType type;
        std::string value;
    };



    class Lexer
    {
    public:
        Lexer(const std::string& source);
        ~Lexer();

        Token GetNextToken();

    private:
        Token parseStringLit();
        Token parseNumber();
        Token parseIdentifier();
    private:
        std::string m_source;

        size_t m_position;
        size_t m_line;
    };
}