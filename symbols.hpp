#include <string>

enum SymbolType{
  SYMBOL_IDENTIFIER,
  SYMBOL_LIT_INT, SYMBOL_LIT_CHAR, SYMBOL_LIT_REAL, SYMBOL_LIT_STRING,
  SYMBOL_VAR_INT, SYMBOL_VAR_BYTE, SYMBOL_VAR_REAL,
  SYMBOL_ARRAY_INT, SYMBOL_ARRAY_BYTE, SYMBOL_ARRAY_REAL,
  SYMBOL_FUNC_INT, SYMBOL_FUNC_BYTE, SYMBOL_FUNC_REAL
};

typedef struct SYMBOL {
  int type;
  std::string text;
} Symbol;

Symbol *insert(int type, std::string text);
void printSymbolsTable();
