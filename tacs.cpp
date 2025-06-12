#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include "tacs.hpp"

std::string TacTypesNames[] = {
  "TAC_UNKNOWN",
  "TAC_SYMBOL", "TAC_MOVE",
  // ops
  "TAC_ADD", "TAC_SUB", "TAC_MUL", "TAC_DIV", "TAC_AND", "TAC_OR",
  "TAC_LE", "TAC_GE", "TAC_EQ", "TAC_DIF", "TAC_LSR", "TAC_GTR", "TAC_NOT",

  "TAC_LABEL", "TAC_BEGINFUN", "TAC_ENDFUN",
  "TAC_IFF", "TAC_IFZ", "TAC_JUMP", "TAC_CALL", "TAC_ARG", "TAC_RET",
  "TAC_PRINT", "TAC_READ",
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
  TAC *code[4] = {NULL};

  // process children
  for (auto c : node->child) {
    code[i++] = generateCode(c);
  }

  // process this node
  switch (node->type) {
    case AST_SYMBOL_ID:
    case AST_SYMBOL_VEC:
    case AST_SYMBOL_LIT_INT:
    case AST_SYMBOL_LIT_REAL:
    case AST_SYMBOL_LIT_CHAR:
      result = new TAC(TAC_SYMBOL, node->symbol, 0, 0);
      break;

    // OPERATIONS
    case AST_ADD:
      result = tacHandleExpr(code[0], code[1], TAC_ADD);
      break;
    case AST_SUB:
      result = tacHandleExpr(code[0], code[1], TAC_SUB);
      break;
    case AST_MUL:
      result = tacHandleExpr(code[0], code[1], TAC_MUL);
      break;
    case AST_DIV:
      result = tacHandleExpr(code[0], code[1], TAC_DIV);
      break;
    case AST_AND:
      result = tacHandleExpr(code[0], code[1], TAC_AND);
      break;
    case AST_OR:
      result = tacHandleExpr(code[0], code[1], TAC_OR);
      break;
    case AST_LE:
      result = tacHandleExpr(code[0], code[1], TAC_LE);
      break;
    case AST_GE:
      result = tacHandleExpr(code[0], code[1], TAC_GE);
      break;
    case AST_EQ:
      result = tacHandleExpr(code[0], code[1], TAC_EQ);
      break;
    case AST_DIF:
      result = tacHandleExpr(code[0], code[1], TAC_DIF);
      break;
    case AST_LSR:
      result = tacHandleExpr(code[0], code[1], TAC_LSR);
      break;
    case AST_GTR:
      result = tacHandleExpr(code[0], code[1], TAC_GTR);
      break;
    case AST_NOT:
      result = tacJoin(
          code[0],
          new TAC(TAC_NOT, makeTemp(), code[0] ? code[0]->res : 0, 0)
        );
      break;
    // --------------------------------------------------
    
    case AST_ATTB:
      result = tacJoin(code[0], new TAC(TAC_MOVE, node->symbol, code[0] ? code[0]->res : 0, 0));
      break;

    case AST_VARIABLES:
      result = new TAC(TAC_MOVE, node->symbol, code[1] ? code[1]->res : 0, 0);
      break;

    case AST_IF:
      result = makeIfThen(code[0], code[1]);
      break;
    case AST_IF_ELSE:
      result = makeIfThenElse(code[0], code[1], code[2]);
      break;

    case AST_WHILE:
      result = makeWhile(code[0], code[1]);
      break;
    case AST_DO_WHILE:
      result = makeDoWhile(code[0], code[1]);
      break;
    default: // return code for all subtrees
       result = tacJoin(
         tacJoin(tacJoin(code[0], code[1]), code[2]),
         code[3]
       );
  }

  return result;
}

TAC *tacHandleExpr(TAC *code0, TAC *code1, TacTypes type) {
  return tacJoin(
    tacJoin(code0, code1),
      new TAC(type, makeTemp(), code0 ? code0->res : 0, code1 ? code1->res : 0)
     );
}

TAC *makeIfThen(TAC *code0, TAC *code1) {
  TAC *ntif = 0; // jump TAC
  TAC *ntlab = 0; // label TAC
  Symbol *newLabel = makeLabel();

  ntif = new TAC(TAC_IFF, newLabel, code0 ? code0->res : 0, 0); // if false, jump to newLabel
  ntlab = new TAC(TAC_LABEL, newLabel, 0, 0);
  ntif->prev = code0;
  ntlab->prev = code1;

  return tacJoin(ntif, ntlab);
}

TAC *makeWhile(TAC *code0, TAC* code1) {
  Symbol* jumpLabel1 = makeLabel();
  Symbol* jumpLabel2 = makeLabel();
  return tacJoin(
          tacJoin(
              tacJoin(
                  tacJoin(
                      tacJoin(
                          new TAC(TAC_LABEL, jumpLabel1, 0, 0), code0),
                      new TAC(TAC_JZ, jumpLabel2, code0 ? code0->res : 0, 0)),
                  code1),
              new TAC(TAC_JUMP, jumpLabel1, 0, 0)),
          new TAC(TAC_LABEL, jumpLabel2, 0, 0));
}


TAC *makeDoWhile(TAC *code0, TAC* code1) {
  Symbol *jumpLabel = makeLabel();
  return tacJoin(
          tacJoin(
              tacJoin(
                  new TAC(TAC_LABEL, jumpLabel, 0, 0), code0),
              code1),
          new TAC(TAC_JZ, jumpLabel, code1 ? code1->res : 0, 0));
}

TAC *makeIfThenElse(TAC *code0, TAC *code1, TAC *code2) {
  TAC *ntiff = 0; // jump TAC
  TAC *ntlab = 0; // tac label to jump in case if doest not hold 

  Symbol *elseLabel = makeLabel(); // beggin of "else" block
  Symbol *endOfifLabel = makeLabel(); // end of "if then" block jump
  TAC *endJumpLabel = new TAC(TAC_LABEL, endOfifLabel, 0, 0);

  ntiff = new TAC(TAC_IFF, elseLabel, code0 ? code0->res : 0, 0); // if false, jump to elseLabel
  ntlab = new TAC(TAC_LABEL, elseLabel, 0, 0);
  ntiff->prev = code0; // condition
  ntlab->prev = code1; // if body

  // if condition holds, at the end of the "if then" we need to jump to after the "else" ends
  // A good way to do this, is to make the beginning of "else" block have a prev prointer to this jump
  return tacJoin(
          tacJoin(
            ntiff,
            tacJoin(
              code1,
              tacJoin(makeJump(endOfifLabel), tacJoin(new TAC(TAC_LABEL, elseLabel, 0, 0), code2)))),
          endJumpLabel);
}

TAC *makeJump(Symbol *label) {
  return new TAC(TAC_JUMP, label, 0,0);
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

