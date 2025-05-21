#include "semantic.hpp"
#include <cstdio>

int SemanticErrors = 0;

std::string AstTypesNames2[] = {
  "AST_UNKNOWN", "AST_SYMBOL", "AST_SYMBOL_LIT",
  // types
  "AST_INT", "AST_CHAR", "AST_REAL", "AST_STRING", "AST_BYTE", "AST_LIT",
  // operations
  "AST_ADD", "AST_SUB", "AST_MUL", "AST_DIV", "AST_AND", "AST_OR",
  "AST_LE", "AST_GE", "AST_EQ", "AST_DIF", "AST_LSR", "AST_GTR", "AST_NOT",
  // ATTRIBUTIONs
  "AST_ATTB", "AST_ATTB_VEC",
  // FLOW 
  "AST_IF", "AST_IF_ELSE", "AST_WHILE", "AST_DO_WHILE",
  // COMMANDS
  "AST_READ", "AST_PRINT", "AST_PRINT_REST", "AST_RETURN", "AST_EMPTY_CMD",
  // COMMAND BLOCKS
  "AST_CMDBLK", "AST_EMPTY_CMDBLK", "AST_CMDL", 
  "AST_FUNCTION", "AST_FUNCTION_ARG", "AST_FUNCTION_ARGS", 
  "AST_VARIABLES", "AST_VARIABLES_VEC", "AST_VARIABLES_ARG",
  // BACKBONE
  "AST_DECL", "AST_PROGRAM", "AST_SYMBOL_ID", "AST_SYMBOL_VEC", "AST_PRTSS",
  "AST_SYMBOL_FUNCTION", "AST_SYMBOL_FUNCTION_ARGS"
};

void check_and_set_declarations(AST *node) {
  if (!node) return;

  switch (node->type) {
    case AST_VARIABLES:
      handleTypes(node, SYMBOL_VARIABLE);
      break;
    case AST_FUNCTION: 
      handleTypes(node, SYMBOL_FUNCTION);
      break;
  }

  for (auto s : node->child) {
    check_and_set_declarations(s);
  }
}

void handleTypes(AST *node, SymbolType type) {
  if (node->symbol)
    validateType(node->symbol);

  node->symbol->type = type;
  assignDataType(node);
}

void assignDataType(AST *node) {
  if (!node->child.front()) {
    fprintf(stderr, "Unable to find child for node %s\n", node->symbol->text.c_str());
    return;
  }

  switch (node->child.front()->type) {
    case AST_INT:
      node->symbol->dataType = DATATYPE_INT;
      break;
    case AST_BYTE:
      node->symbol->dataType = DATATYPE_BYTE;
      break;
    case AST_REAL:
      node->symbol->dataType = DATATYPE_REAL;
      break;
  }
}

void validateType(Symbol *symbol) {
  if (symbol->type != SYMBOL_IDENTIFIER) {
    fprintf(stderr, "SEMANTIC ERROR: %s already declared\n", symbol->text.c_str());
    ++SemanticErrors;
  }
}

void semantic_check_undeclared() {
  SemanticErrors += check_undeclared();
}

void checkOperands(AST *node) {
  if (!node) return;

  switch (node->type) {
    case AST_ADD:
      validateOperandsTypes(AST_ADD, node->child.at(0), node->child.at(1));
      break;
  }

  for (auto s : node->child) {
    checkOperands(s);
  }
}

bool isValidOperand(int type, AST *node) {
  if (type == AST_ADD ||
      type == AST_SUB ||
      type == AST_MUL ||
      type == AST_DIV ||
      type == AST_AND ||
      type == AST_OR  ||
      type == AST_LE  ||
      type == AST_GE  ||
      type == AST_EQ  ||
      type == AST_DIF ||
      type == AST_LSR ||
      type == AST_GTR ||
      type == AST_NOT
    ) return true;

  if (type == AST_SYMBOL_LIT) {
    return node->child.front()->symbol->type == SYMBOL_LIT_INT;
  }

  if (type == AST_SYMBOL_FUNCTION) {
    return (
        node->child.front()->symbol->dataType == DATATYPE_INT   //||
        //node->child.front()->symbol->dataType == DATATYPE_BYTE  ||
        //node->child.front()->symbol->dataType == DATATYPE_REAL 
      );
  }

  return false;
}

void validateOperandsTypes(AstTypes operation, AST *left, AST *right) {
  if (!isValidOperand(left->type, left)) {
    fprintf(stderr, "SEMANTIC ERROR: invalid left operand for %s\n", AstTypesNames2[operation].c_str());
    ++SemanticErrors;
  }

  if (!isValidOperand(left->type, right)) {
    fprintf(stderr, "SEMANTIC ERROR: invalid right operand for %s\n", AstTypesNames2[operation].c_str());
    ++SemanticErrors;
  }
}

