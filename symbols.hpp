#include <string>

typedef struct SYMBOL {
  int type;
  std::string text;
} Symbol;

Symbol *insert(int type, std::string text);
void printSymbolTable();
