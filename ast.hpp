// AST - Abstract Syntax Tree
#include "symbols.hpp"
#include <vector>

#ifndef AST_HEADER 
#define AST_HEADER

enum AstTypes {
  AST_UNKNOWN, AST_SYMBOL, 
  AST_INT, AST_CHAR, AST_REAL, AST_STRING,
  AST_ADD, AST_SUB, 
  AST_MUL, AST_DIV
};

struct AST {
	int type;
	std::vector<AST*> child;
	SYMBOL* symbol;

	public:
	AST(int t, std::vector<AST*> c, SYMBOL* s=NULL): type(t), child(c), symbol(s) {};
};

void printAST(AST *node, int level);

#endif 
