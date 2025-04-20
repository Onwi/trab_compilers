%{
  #include "parser.tab.hpp"
  #include "symbols.hpp"
  int running = 1;
  int lineNumber = 1;
  int getLineNumber(void);
  int isRunning(void);
  void initMe(void);
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

[\']\\*.[\'] { insert(SYMBOL_LIT_CHAR, yytext); return LIT_CHAR; }
\"[^\"]*(\\\"[^\"]*)*\" { insert(SYMBOL_LIT_STRING, yytext); return LIT_STRING; }
[0-9]+  { insert(SYMBOL_LIT_INT, yytext); return LIT_INT; }
[0-9]+[/][0-9]+ { insert(SYMBOL_LIT_REAL, yytext); return LIT_REAL; }

[,;:()\[\]{}=+\-*/%<>&|~]    { return yytext[0]; }
[a-zA-Z_]+[a-zA-Z_0-9]* { insert(SYMBOL_IDENTIFIER, yytext); return TK_IDENTIFIER; }

"=="    { return OPERATOR_EQ; }
"!="    { return OPERATOR_DIF; }
"<="    { return OPERATOR_LE; }
">="    { return OPERATOR_GE; }

[ \t]
\n    {  ++lineNumber; }
"//".*
"/--"    { BEGIN(COMMENT); }
. { return TOKEN_ERROR; }
 
<COMMENT>"--/" { BEGIN(INITIAL); }
<COMMENT>"\n" { ++lineNumber; }
<COMMENT>.

%%

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

void initMe(void) {

}
