#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyparse();

extern FILE *yyin;
int isRunning(void);
void initMe();
int getLineNumber(void);
void printSymbolsTable();

int main(int argc, char **argv) { 
  if(argc < 2) {
    fprintf(stderr, "missing file filename\n");
    exit(1);
  }

  if(0 == (yyin=fopen(argv[1],"r"))) {
    fprintf(stderr, "error opening file!!");
    exit(1);

  }


  initMe();
  
  yyparse();
  printSymbolsTable();

  printf("compilation was succesfull!!\n");

  exit(0);
}
