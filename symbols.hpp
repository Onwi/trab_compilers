#include <string>

#ifndef SYMBOL_HEADER 
#define SYMBOL_HEADER 

struct FUNC_PARAMS;

enum SymbolType{
  SYMBOL_IDENTIFIER, SYMBOL_VARIABLE, SYMBOL_FUNCTION, SYMBOL_VEC,
  SYMBOL_LIT_INT, SYMBOL_LIT_CHAR, SYMBOL_LIT_REAL, SYMBOL_LIT_STRING,
  SYMBOL_VAR_INT, SYMBOL_VAR_BYTE, SYMBOL_VAR_REAL,
  SYMBOL_FUNC_INT, SYMBOL_FUNC_BYTE, SYMBOL_FUNC_REAL,
  SYMBOL_LIT_BYTE,
};

enum DataTypes{
  JUST_FOR_SKIP, DATATYPE_REAL, DATATYPE_BYTE, DATATYPE_INT,
  DATATYPE_CHAR, DATATYPE_STRING, DATATYPE_BOOL
};

typedef struct SYMBOL {
  // lets use 64(2x32) bits to store a few types
  // its 2025, memory is cheap and I'm lazy
  int type;
  int dataType;
  std::string text;
  FUNC_PARAMS* funcParams;
} Symbol;

struct FUNC_PARAMS {
  FUNC_PARAMS* nextParam;
  SYMBOL* symbol;
  
  public:
    FUNC_PARAMS(FUNC_PARAMS* np, SYMBOL* s): nextParam(np), symbol(s) {};
};

Symbol *insert(int type, std::string text, int dataType=JUST_FOR_SKIP);
void printSymbolsTable();
int check_undeclared();


int GetNumberParamsFP(FUNC_PARAMS* fp);
FUNC_PARAMS* GetLastFP(SYMBOL* symb);



#endif 
