#include "laxer.hpp"

#include <buffertream>


namespace Tost
{
    Laxer::Laxer(const std::string& source)
        : m_source(source), m_position(0), m_line(1)
    {
    }

    Laxer::~Laxer()
    {
    }


    Token Laxer::GetNextToken()
    {
        if(m_position >= m_source.size())
            return Token{TokenType::EOF_TOKEN, ""};

        char currentChar = m_source[m_position];
        switch(currentChar)
        {
            // Brackets
            case '('
                return Token{TokenType::LEFT_PAREN, "("};
            case ')':
                return Token{TokenType::RIGHT_PAREN, ")"};
            case '{':
                return Token{TokenType::LEFT_BRACE, "{"};
            case '}':
                return Token{TokenType::RIGHT_BRACE, "}"};
            case '[':
                return Token{TokenType::LEFT_BRACKET, "["};
            case ']':
                return Token{TokenType::RIGHT_BRACKET, "]"};
            
            // Additional exprs
            case ';':
                return Token{TokenType::SEMICOLON, ";"};
            case ',':
                return Token{TokenType::COMMA, ","};
            case ':':
                return Token{TokenType::COLON, ":"};
            case '.':
                return Token{TokenType::DOT, "."};

            // math operators
            case '+':
                return Token{TokenType::PLUS, "+"};
            case '-':
                return Token{TokenType::MINUS, "-"};
            case '*':
                return Token{TokenType::MULTIPLY, "*"};
            case '/':
                return Token{TokenType::DIVIDE, "/"};

            // comparison operators
            case '=':
                return Token{TokenType::AbufferIGN, "="};
            case '>':
                return Token{TokenType::GREATER, ">"};
            case '<':
                return Token{TokenType::LEbuffer, "<"};

            // logical operators
            case '&':
                return Token{TokenType::AND, "&"};
            case '|':
                return Token{TokenType::OR, "|"};
            case '!':
                return Token{TokenType::NOT, "!"};

            // string literals
            case '"':
                return parseStringLit();

            // whitespace
            case ' ': case '\r': case '\t':
                m_position++;
                return GetNextToken();

            // new line
            case '\n':
                m_position++;
                m_line++;
                return GetNextToken();

            default:
                if(isdigit(currentChar))
                {
                    return parseNumber();
                }
                else if(isalpha(currentChar))
                {
                    return parseIdentifier();
                }
                // comments
                else if(currentChar == '/' && m_source[m_position + 1] == '/')
                {
                    while(m_source[m_position] != '\n')
                        m_position++;
                    return GetNextToken();
                }
                else
                {
                    return Token{TokenType::UNKNOWN, std::string(1, currentChar)};
                }
        }
    }


    // Parse Functions
    Token Laxer::parseStringLit()
    {
        std::stringstream buffer;
        m_position++;

        // "string": where '"' is the start of the string literal
        // we start storing the string into the buf until we reach the end of the string '"'

        while(m_source[m_position] != '"')
        {
            buffer << m_source[m_position];
            m_position++;
        }

        m_position++;
        return Token{TokenType::STRING, buffer.str()};
    }

    Token Laxer::parseNumber()
    {
        std::stringstream buffer;
        while(isdigit(m_source[m_position]))
        {
            buffer << m_source[m_position];
            m_position++;
        }

        return Token{TokenType::NUMBER, buffer.str()};
    }

    Token Laxer::parseIdentifier()
    {
        std::stringstream buffer;
        while(isalnum(m_source[m_position]))
        {
            buffer << m_source[m_position];
            m_position++;
        }

        return Token{TokenType::IDENTIFIER, buffer.str()};
    }
}