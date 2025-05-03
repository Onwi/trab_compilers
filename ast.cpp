#include <cstdio>
#include "ast.hpp"

std::string AstTypesNames[] = {
  "AST_UNKNOWN", "AST_SYMBOL", 
  "AST_INT", "AST_CHAR", "AST_REAL", "AST_STRING",
  "AST_ADD", "AST_SUB", 
  "AST_MUL", "AST_DIV"
};

void printAST(AST *node, int level) {
  if (!node) return;

  for (int i = 0; i < level; ++i) 
    fprintf(stderr, " ");

  fprintf(stderr, "AST: %s, %s \n", AstTypesNames[node->type].c_str(), 
          node->symbol ? node->symbol->text.c_str() : "");

  for (auto s : node->child) {
    printAST(s, level+1);
  }
}

