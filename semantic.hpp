#ifndef SEMANTIC
#define SEMANTIC

#include "ast.hpp"
#include "symbols.hpp"

void check_and_set_declarations(AST *node);
void validateType(Symbol *symbol);
void semantic_check_undeclared();
void assignDataType(AST *node);
void handleTypes(AST *node, SymbolType type);
void checkOperands(AST *node);
void validateOperandsTypes(AstTypes operation, AST *left, AST *right);

#endif // !SEMANTIC
