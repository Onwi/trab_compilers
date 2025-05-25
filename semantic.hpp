#ifndef SEMANTIC
#define SEMANTIC

#include "ast.hpp"
#include "symbols.hpp"

void check_and_set_declarations(AST *node);
void validateDeclaration(Symbol *symbol);
void semantic_check_undeclared();
void assignDataType(AST *node);
void handleTypes(AST *node, SymbolType type);


void checkOperands(AST *node, Symbol *function);
void validateOperandsTypes(int operation, AST *left, AST *right);
bool isValidOperand(int type, AST *node);


void validateFunctionParams(AST *node);
void SetFunctionParams(AST* node, SYMBOL* func);



#endif // !SEMANTIC
