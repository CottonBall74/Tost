#pragma once

#include "lexer.hpp"
#include "parser.hpp"


namespace Tost
{
    class Interpreter
    {
    public:
        Interpreter(const std::string& source);
        void interpret();

    private:
        Lexer& m_lexer;
        Parser& m_parser;

        void error(const std::string& message);
        void executeStatement();
    };
}