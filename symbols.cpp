#include <cstdio>
#include <cstdlib>
#include <map>
#include <ostream>
#include <iostream>
#include "symbols.hpp"

std::map<std::string, Symbol*> symbolTable;

std::string tokenIds[] = {
  "IDENTIFIERS",
  "REAL",
  "INTEGERS",
  "CHARS",
  "STRINGS"
}; 

// enum symbolEnum { TK_IDENTIFIER, LIT_CHAR , LIT_STRING, LIT_INT, LIT_REAL };

Symbol *insert(int type, std::string text) {
  std::map<std::string, Symbol*>::iterator it = symbolTable.find(text);
  if (it != symbolTable.end())  {
    return it->second;
  }

  Symbol *newSymbol = (Symbol *) malloc(sizeof(Symbol)); 
  newSymbol->type = type;
  newSymbol->text = text;
  symbolTable[text] = newSymbol;

  return newSymbol;
}

void printSymbolsTable() {
  for (auto s : symbolTable) {
    std::cout << "Symbol[" << s.second->text << "," << s.second->type << "]" << std::endl;
  }
}
