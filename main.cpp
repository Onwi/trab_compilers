#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "ast.hpp"

int yylex();
int yyparse();

extern FILE *yyin;
extern FILE *yyout;

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
    exit(1);
  }

  if(0 == (yyout=fopen(argv[2],"w"))) {
    fprintf(stderr, "error opening file!!");
    exit(1);
  }


  initMe();

  yyparse();
  //printSymbolsTable();

  //printAST(head, 0);
  astWrite(head, yyout);

  printf("compilation was succesfull!!\n");

  exit(0);
}
