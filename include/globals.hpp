#pragma once

#include <string>

enum EToken {
    T_EOF = -1,

    T_FUNC = -2,
    T_EXTERN = -3,

    T_IDENTIFIER = -4,
    T_NUMBER = -5,
};

static inline std::string g_IdentifierString;
static inline double g_NumericValue;
static inline int CurrentToken;