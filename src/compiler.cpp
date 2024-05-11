#include "parser.hpp"
#include "globals.hpp"
#include "processing_utils.hpp"


static void HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        GetNextToken();
    }
}

static void HandleExtern() {
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        GetNextToken();
    }
}

static void HandleTopLevelExpression() {
    if (ParseTopLevelExpression()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        GetNextToken();
    }
}

static void MainLoopProcessor() {
    while (true) {
        fprintf(stderr, "ready>");
        switch (g_CurrentToken) {
            case T_EOF:
                return;
            case ';':
                GetNextToken();
                break;
            case T_FUNC:
                HandleDefinition();
                break;
            case T_EXTERN:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}

int main() {
    g_BinaryOpPrecedence['<'] = 10;
    g_BinaryOpPrecedence['+'] = 20;
    g_BinaryOpPrecedence['-'] = 30;
    g_BinaryOpPrecedence['*'] = 40;

    fprintf(stderr, "ready> ");
    GetNextToken();

    MainLoopProcessor();

    return 0;
}