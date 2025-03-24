

%%

byte    {printf("kw");}
int     {printf("kw");}
real    {printf("kw");}
if      {printf("kw");}
else    {printf("kw");}
do      {printf("kw");}
while   {printf("kw");}
read    {printf("kw");}
print   {printf("kw");}
return  {printf("kw");}
[,;:()\[\]{}=+\-*/%<>&|~]    {printf("%d", yytext[0]);}


[a-zA-Z]+ {printf("indentifier");}
[ \n\t]
.

%%

int main() { 
  yylex();
}

int yywrap() {
  return 1;
}
