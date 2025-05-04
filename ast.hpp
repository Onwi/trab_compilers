// AST - Abstract Syntax Tree
#include "symbols.hpp"
#include <vector>

#ifndef AST_HEADER 
#define AST_HEADER

enum AstTypes {
  AST_UNKNOWN, AST_SYMBOL, AST_SYMBOL_LIT,
  // types
  AST_INT, AST_CHAR, AST_REAL, AST_STRING, AST_BYTE, AST_LIT,
  // operations
  AST_ADD, AST_SUB, AST_MUL, AST_DIV, AST_AND, AST_OR,
  AST_LE, AST_GE, AST_EQ, AST_DIF, AST_LSR, AST_GTR, AST_NOT,
  // ATTRIBUTIONs
  AST_ATTB, AST_ATTB_VEC,
  // FLOW 
  AST_IF, AST_IF_ELSE, AST_WHILE, AST_DO_WHILE,
  // COMMANDS
  AST_READ, AST_PRINT, AST_PRINT_REST, AST_RETURN, AST_EMPTY_CMD,
  // COMMAND BLOCKS
  AST_CMDBLK, AST_EMPTY_CMDBLK, AST_CMDL,
  AST_FUNCTION, AST_FUNCTION_ARG, AST_FUNCTION_ARGS,
  AST_VARIABLES, AST_VARIABLES_VEC, AST_VARIABLES_ARG,
  // BACKBONE
  AST_DECL, AST_PROGRAM, AST_SYMBOL_ID, AST_SYMBOL_VEC, AST_PRTSS,
  AST_SYMBOL_FUNCTION, AST_SYMBOL_FUNCTION_ARGS
};

struct AST {
	int type;
	std::vector<AST*> child;
	SYMBOL* symbol;

	public:
	AST(int t, std::vector<AST*> c, SYMBOL* s=NULL): type(t), child(c), symbol(s) {};
};

void printAST(AST *node, int level);
void astWrite(AST* node, FILE* file);

#endif 
