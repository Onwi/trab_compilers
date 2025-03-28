#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

int main(int argc, char **argv) { 
  yyin = fopen(argv[1], "r");

  int token;
  while (running) {
    token = yylex();
  }
  exit(0);
}
