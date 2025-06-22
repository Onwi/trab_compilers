#ifndef TACS_HEADER
#define TACS_HEADER

#include "symbols.hpp"
#include "ast.hpp"

enum TacTypes {
  TAC_UNKNOWN,
  TAC_SYMBOL, TAC_MOVE, TAC_VARIABLE,
  // ops
  TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV, TAC_AND, TAC_OR,
  TAC_LE, TAC_GE, TAC_EQ, TAC_DIF, TAC_LSR, TAC_GTR, TAC_NOT,

  TAC_LABEL, TAC_BEGINFUN, TAC_ENDFUN,
  TAC_IFF, TAC_IFZ, TAC_JUMP, TAC_JZ, TAC_CALL, TAC_ARG, TAC_RET,
  TAC_PRINT, TAC_READ,
};


struct TAC {
  TacTypes type;
  Symbol *res;
  Symbol *op1;
  Symbol *op2;
  TAC *prev;
  TAC *next;
  
  public:
    TAC(TacTypes t, Symbol *r, Symbol *op1, Symbol *op2):
      type(t), res(r), op1(op1), op2(op2) {prev=next=0;}
};

TAC *tacJoin(TAC* l1, TAC* l2);
TAC *generateCode(AST *node);
TAC *makeIfThen(TAC *code0, TAC *code1);
TAC *makeIfThenElse(TAC *code0, TAC *code1, TAC* code2);
TAC *makeJump(Symbol *label);
TAC *makeWhile(TAC *code0, TAC* code1);
TAC *makeDoWhile(TAC *code0, TAC* code1);
TAC *tacHandleExpr(TAC *code0, TAC *code1, TacTypes type);

void tacPrintSingle(TAC *tac);
void tacPrintBackwards(TAC *tac);


#endif // !TACS_HEADER
