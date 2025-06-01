#ifndef TACS_HEADER
#define TACS_HEADER

#include "symbols.hpp"
#include "ast.hpp"

enum TacTypes {
  TAC_UNKNOWN,
  TAC_SYMBOL,
  TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV,
  TAC_ASSIGN,
};

struct TAC {
  int type;
  Symbol *res;
  Symbol *op1;
  Symbol *op2;
  TAC *prev;
  TAC *next;
  
  public:
    TAC(int t, Symbol *r, Symbol *op1, Symbol *op2):
      type(t), res(r), op1(op1), op2(op2) {prev=next=0;}
};

TAC *tacJoin(TAC* l1, TAC* l2);
TAC *generateCode(AST *node);

void tacPrintSingle(TAC *tac);
void tacPrintBackwards(TAC *tac);


#endif // !TACS_HEADER
