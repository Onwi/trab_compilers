#include <cstdio>
#include "tacs.hpp"

std::string TacTypesNames[] = {
  "TAC_UNKNOWN",
  "TAC_SYMBOL",
  "TAC_ADD", "TAC_SUB", "TAC_MUL", "TAC_DIV",
  "TAC_ASSIGN",
};

TAC *tacJoin(TAC* l1, TAC* l2) {
  if (!l1) return l2;
  if (!l2) return l1;

  TAC *t;
  for (t = l2; t->prev != 0; t = t->prev);

  t->prev = l1;
  return l2;
}

TAC *generateCode(AST *node) {
  if(!node) return 0;

  int i = 0;
  TAC *result = 0;
  TAC *code[10];

  // process children
  for (auto c : node->child) {
    code[i++] = generateCode(c);
  }

  // process this node
  switch (node->type) {
    case AST_SYMBOL:
      result = new TAC(TAC_SYMBOL, node->symbol, 0, 0);
      break;
    case AST_ADD:
      result = tacJoin(
        tacJoin(code[0], code[1]),
        new TAC(TAC_ADD, makeTemp(), code[0] ? code[0]->res : 0, code[0] ? code[1]->res : 0)
      );
      break;
    case AST_ATTB:
      result = tacJoin(code[0], new TAC(TAC_ASSIGN, node->symbol, code[0] ? code[0]->res : 0, 0));
      break;
    default: // return code for all subtrees
      result = tacJoin(
        tacJoin(tacJoin(code[0], code[1]), code[2]),
        code[3]
      );
  }

  return result;
}

void tacPrintSingle(TAC *tac) {
  if (!tac) return;
  if (tac->type == TAC_SYMBOL) return;

  fprintf(
    stderr,
    "TAC(%s, %s, %s, %s)\n", 
    TacTypesNames[tac->type].c_str(),
    (tac->res) ? tac->res->text.c_str() : "0",
    (tac->op1) ? tac->op1->text.c_str() : "0",
    (tac->op2) ? tac->op2->text.c_str() : "0"
  );
}

void tacPrintBackwards(TAC *tac) {
  if (!tac) return;

  for (; tac; tac = tac->prev)
    tacPrintSingle(tac);
}

