%{
  #include "tokens.h"
  #include "symbols.cpp"
  int running = 1;
  int lineNumber = 1;
  int getLineNumber(void);
  int isRunning(void);
%}

%option nounput
%option noinput
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

[0-9]+  { insert(4, yytext); return LIT_INT; }
[0-9]+[/][0-9]+ { insert(5, yytext); return LIT_REAL; }

[\'].[\'] { insert(2, yytext); return LIT_CHAR; }
\"[^\"]*(\\\"[^\"]*)*\" { insert(3, yytext); return LIT_STRING; }

[,;:()\[\]{}=+\-*/%<>&|~]    { return yytext[0]; }
[a-zA-Z_]+[a-zA-Z_0-9]* { insert(1, yytext); return TK_IDENTIFIER; }

"=="    { return OPERATOR_EQ; }
"!="    { return OPERATOR_DIF; }
"<="    { return OPERATOR_LE; }
">="    { return OPERATOR_GE; }

[ \t]
"\n"    {  ++lineNumber; }
"//".*
"/--"    { BEGIN(COMMENT); }
.

<COMMENT>"--/" { BEGIN(INITIAL); }
<COMMENT>"\n" { ++lineNumber; }
<COMMENT>.

%%

#include "main.cpp"

int yywrap() {
  running = 0;
  return 1;
}

int getLineNumber(void) {
  return lineNumber;
}

int isRunning(void) {
  return running;
}
