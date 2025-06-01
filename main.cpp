#include <stdio.h>
#include <stdlib.h>

#include "ast.hpp"
#include "semantic.hpp"
#include "symbols.hpp"
#include "tacs.hpp"

int yylex();
int yyparse();

extern FILE *yyin;
extern FILE *yyout;
extern int SemanticErrors;

int isRunning(void);
void initMe();
int getLineNumber(void);
void printSymbolsTable();
extern AST* head;

int main(int argc, char **argv) { 
  if(argc < 3) {
    fprintf(stderr, "missing file filename\n");
    exit(1);
  }

  if(0 == (yyin=fopen(argv[1],"r"))) {
    fprintf(stderr, "error opening file!!");
    exit(2);
  }

  if(0 == (yyout=fopen(argv[2],"w"))) {
    fprintf(stderr, "error opening file!!");
    exit(2);
  }


  initMe();

  yyparse();
  printSymbolsTable();

  //printAST(head, 0);
  astWrite(head, yyout);
  check_and_set_declarations(head);
  semantic_check_undeclared();
  checkOperands(head, NULL);

  TAC *tacs = generateCode(head);
  tacPrintBackwards(tacs);
                        

  if (SemanticErrors == 0) {
    printf("Compilation was succesfull!!\n");
  } else {
    printf("Main:  Semantic Error!!\n");
    exit(4);
  }

  exit(0);
}
