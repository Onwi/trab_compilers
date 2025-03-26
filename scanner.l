%{
  #include "tokens.h"
  int running = 1;
  int lineNumber = 1;
%}

%x COMMENT

%%

byte    { return KW_BYTE; }
int     { return KW_INT; }
real    { return KW_REAL; }
if      { return KW_IF; }
else    { return KW_ELSE; }
do      { return KW_DO; }
while   { return KW_WHILE; }
read    { return KW_READ; }
print   { return KW_PRINT; }
return  { return KW_RETURN; }

[0-9]+  { return LIT_INT; }
[0-9]+[/][0-9]+ { return LIT_REAL; }

[\'].[\'] { return LIT_CHAR; }
\"[^\"]*(\\\"[^\"]*)*\" { return LIT_STRING; }

[,;:()\[\]{}=+\-*/%<>&|~]    { return yytext[0]; }
[a-zA-Z_]+[a-zA-Z_0-9]* { return TK_IDENTIFIER; }

"=="    { return OPERATOR_EQ; }
"!="    { return OPERATOR_DIF; }
"<="    { return OPERATOR_LE; }
">="    { return OPERATOR_GE; }

[ \t]
"\n"    { ++lineNumber; }
"//".*
"/--"    { BEGIN(COMMENT); }
.

<COMMENT>"--/" { BEGIN(INITIAL); }
<COMMENT>"\n" { ++lineNumber; }
<COMMENT>.

%%

int main(int argc, char **argv) { 
  if (argc < 2) {
    fprintf(stderr, "Call: ./a.out file_name\n");
    // exit(1);
  }
  yyin = fopen(argv[1], "r");

  int token;
  while (running) {
    token = yylex();
    switch (token) {
      case KW_BYTE: 
        printf("kw byte found\n");
        break;
      case KW_INT: 
        printf("kew int found\n");
        break;
      case KW_REAL: 
        printf("kew real found\n");
        break;
      case KW_IF: 
        printf("kew if found\n");
        break;
      case KW_ELSE: 
        printf("kew else found\n");
        break;
      case KW_DO: 
        printf("kew do found\n");
        break;
      case KW_WHILE: 
        printf("kew while found\n");
        break;
      case KW_READ: 
        printf("kew read found\n");
        break;
      case KW_PRINT: 
        printf("kew print found\n");
        break;
      case KW_RETURN: 
        printf("kw return found\n");
        break;
      case TK_IDENTIFIER: 
        printf("tk indendifier found\n");
        break;
       case LIT_INT: 
        printf("lit int found\n");
        break;
       case LIT_CHAR: 
        printf("lit char found\n");
        break;
       case LIT_REAL: 
        printf("lit real found\n");
        break;
      case LIT_STRING:
        printf("lit string fround\n");
        break;
      case OPERATOR_LE:
        printf("le operator fround\n");
        break;
      case OPERATOR_GE:
        printf("ge operator fround\n");
        break;
      case OPERATOR_EQ:
        printf("eq operator fround\n");
        break;
      case OPERATOR_DIF:
        printf("diff operator fround\n");
        break;
      case TOKEN_ERROR:
        printf("error token fround\n");
        break;
      default:
        printf("found symbol: %c\n", token);
    }
  }
  exit(0);
}

int yywrap() {
  running = 0;
  return 1;
}
