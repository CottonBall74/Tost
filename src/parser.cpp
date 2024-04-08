#include "parser.hpp"

#include <iostream>
#include <stdexcept>


namespace Tost
{
    Parser::Parser(Lexer& lexer)
        : m_lexer(lexer), m_currentToken(lexer.GetNextToken())
    {}

 
    void Parser::parse()
    {
        while(m_currentToken.type != TokenType::EOF_TOKEN)
        {
            statement();
        }
    }

    void Parser::advance()
    {
        m_currentToken = m_lexer.GetNextToken();
    }

    void Parser::eat(TokenType type)
    { // eat is basicly like pop from the tree
        if(m_currentToken.type == type)
            advance();
        else
            error("Unexpected token");
    }

    void Parser::error(const std::string& message)
    {
        throw std::runtime_error(message);
    }

    // checks for what the token is and returns the value
    void Parser::statement()
    {
        if(m_currentToken.type == TokenType::LOG)
        {
            eat(TokenType::LOG);
            eat(TokenType::LEFT_PAREN);
            std::cout << expression() << std::endl;
            eat(TokenType::RIGHT_PAREN);
            eat(TokenType::SEMICOLON);
        }
        if(m_currentToken.type == TokenType::IF)
            conditionalStatement();
        if(m_currentToken.type == TokenType::WHILE)
            loopStatement();
        else
            error("Unexpected token");
    }

    // this is the conditional if/else statements
    void Parser::conditionalStatement()
    {
        eat(TokenType::IF);
        eat(TokenType::LEFT_PAREN);
        bool condition = expression();
        eat(TokenType::RIGHT_PAREN);
        eat(TokenType::LEFT_BRACE);

        // first does if, then checks for else if, then checks for else
        if(condition)
        {
            statement();
        }

        // WARNING: highly test-ical stuff
        else if(m_currentToken.type == TokenType::ELSE_IF)
        {
            eat(TokenType::ELSE_IF);
            eat(TokenType::LEFT_PAREN);
            bool condition = expression();
            eat(TokenType::RIGHT_PAREN);
            eat(TokenType::LEFT_BRACE);

            if(condition)
            {
                statement();
            }
            else
            {
                while(m_currentToken.type != TokenType::ELSE)
                {
                    advance();
                }
            }

            eat(TokenType::RIGHT_BRACE);
        }
        else if(m_currentToken.type == TokenType::ELSE)
        {
            eat(TokenType::ELSE);
            statement();

            eat(TokenType::RIGHT_BRACE);
        }
        else
        {
            while(m_currentToken.type != TokenType::EOF_TOKEN)
            {
                advance();
            }
        }

        eat(TokenType::RIGHT_BRACE);
    }


    // does looping statements
    void Parser::loopStatement()
    {
        eat(TokenType::WHILE);
        eat(TokenType::LEFT_PAREN);
        bool condition = expression();
        eat(TokenType::RIGHT_PAREN);
        eat(TokenType::LEFT_BRACE);

        // might end up in an infinite loop... so...
        // WARNING: highly test-ical stuff
        while(condition)
        {
            parse();
            condition = expression();
        }
        // loops are dangerous man

        eat(TokenType::RIGHT_BRACE);
    }


    // this is the expression
    // basicly, it's the math part of the code
    int Parser::expression()
    {
        int result = term();

        while(m_currentToken.type == TokenType::PLUS || m_currentToken.type == TokenType::MINUS)
        {
            if(m_currentToken.type == TokenType::PLUS)
            {
                eat(TokenType::PLUS);
                result += term();
            }
            else if(m_currentToken.type == TokenType::MINUS)
            {
                eat(TokenType::MINUS);
                result -= term();
            }
        }

        return result;
    }


    // this is the term
    // basicly, it's the multiplication and division part of the code   
    int Parser::term()
    {
        int result = factor();

        while(m_currentToken.type == TokenType::MULTIPLY || m_currentToken.type == TokenType::DIVIDE)
        {
            if(m_currentToken.type == TokenType::MULTIPLY)
            {
                eat(TokenType::MULTIPLY);
                result *= factor();
            }
            else if(m_currentToken.type == TokenType::DIVIDE)
            {
                eat(TokenType::DIVIDE);
                if(factor() != 0)
                    result /= factor();
                else
                    error("Division by zero");
            }
        }

        return result;
    }


    // alright, now time for the factor
    // this is the part where the numbers and parenthesis are
    int Parser::factor()
    {
        if(m_currentToken.type == TokenType::NUMBER)
        {
            int value = std::stoi(m_currentToken.value);
            eat(TokenType::NUMBER);
            return value;
        }
        else if(m_currentToken.type == TokenType::LEFT_PAREN)
        {
            eat(TokenType::LEFT_PAREN);
            int result = expression();
            eat(TokenType::RIGHT_PAREN);
            return result;
        }
        else
            error("Unexpected token");
    }
}