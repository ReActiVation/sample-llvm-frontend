#pragma once

#include <cstdio>
#include <cstdlib>

#include "globals.hpp"

namespace {
static int GetToken() {
    static int lastChar = ' ';

    while (std::isspace(lastChar))
        lastChar = getchar();

    if (std::isalpha(lastChar)) {
        g_IdentifierString = lastChar;

        while (std::isalnum(lastChar = getchar()))
            g_IdentifierString += lastChar;

        if (g_IdentifierString == "fun")
            return T_FUNC;
        if (g_IdentifierString == "extern")
            return T_EXTERN;

        return T_IDENTIFIER;
    }

    if (std::isdigit(lastChar) || lastChar == '.') {
        std::string numericString;

        do {
            numericString += lastChar;
            lastChar = getchar();
        } while (std::isdigit(lastChar) || lastChar == '.');

        g_NumericValue = std::strtod(numericString.c_str(), 0);
        return T_NUMBER;
    }

    if (lastChar == '#') {
        do
            lastChar = getchar();
        while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

        if (lastChar != EOF)
            return GetToken();
    }

    if (lastChar == EOF)
        return T_EOF;

    int thisChar = lastChar;
    lastChar = getchar();
    return thisChar;
}

static int GetNextToken() {
    return CurrentToken = GetToken();
}
}