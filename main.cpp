#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

int yylex();
extern char *yytext;
extern FILE *yyin;
int isRunning(void);
void initMe();

int main(int argc, char **argv) { 
  yyin = fopen(argv[1], "r");
  initMe();

  int token;
  while (isRunning()) {
    token = yylex();
  }
  exit(0);
}
