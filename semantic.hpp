#ifndef SEMANTIC
#define SEMANTIC

#include "ast.hpp"
#include "symbols.hpp"

void check_and_set_declarations(AST *node);
void check_undeclared(AST *node);
void validateType(Symbol *symbol);

#endif // !SEMANTIC
