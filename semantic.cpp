#include "semantic.hpp"
#include <cstdio>

int SemanticErrors = 0;

void check_and_set_declarations(AST *node) {
  if (!node) return;

  switch (node->type) {
    case AST_VARIABLES:
      if (node->symbol)
        validateType(node->symbol);
      node->symbol->type = SYMBOL_VARIABLE;
      break;
    case AST_FUNCTION: 
      if (node->symbol)
        validateType(node->symbol);
      node->symbol->type = SYMBOL_FUNCTION;
      break;
  }

  for (auto s : node->child) {
    check_and_set_declarations(s);
  }
}

void validateType(Symbol *symbol) {
  if (symbol->type != SYMBOL_IDENTIFIER) {
    fprintf(stderr, "SEMANTIC ERROR: %s already declared", symbol->text.c_str());
    ++SemanticErrors;
  }
}
