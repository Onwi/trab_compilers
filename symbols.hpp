#include <string>

#ifndef SYMBOL_HEADER 
#define SYMBOL_HEADER 

enum SymbolType{
  SYMBOL_IDENTIFIER, SYMBOL_VARIABLE, SYMBOL_FUNCTION,
  SYMBOL_LIT_INT, SYMBOL_LIT_CHAR, SYMBOL_LIT_REAL, SYMBOL_LIT_STRING,
  SYMBOL_VAR_INT, SYMBOL_VAR_BYTE, SYMBOL_VAR_REAL,
  SYMBOL_ARRAY_INT, SYMBOL_ARRAY_BYTE, SYMBOL_ARRAY_REAL,
  SYMBOL_FUNC_INT, SYMBOL_FUNC_BYTE, SYMBOL_FUNC_REAL
};

enum DataTypes{
  DATATYPE_INT, DATATYPE_BYTE, DATATYPE_REAL,
  DATATYPE_CHAR, DATATYPE_STRING,
};

typedef struct SYMBOL {
  // lets use 64(2x32) bits to store a few types
  // its 2025, memory is cheap and I'm lazy
  int type;
  int dataType;
  std::string text;
} Symbol;

Symbol *insert(int type, std::string text);
void printSymbolsTable();
int check_undeclared();

#endif 
