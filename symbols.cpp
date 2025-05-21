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
    std::cout << "Symbol: " << s.second->text << ", SymbolType: " << s.second->type << ", dataType: " << s.second->dataType << std::endl;
  }
}

int check_undeclared() {
  int undeclared_symbols = 0;
  for (auto s : symbolTable) {
    if (s.second->type == SYMBOL_IDENTIFIER) {
      ++undeclared_symbols;
      std::cerr << "Undeclared Symbol [" << s.second->text << "], type:" << s.second->type << std::endl;
    }
  }

  return undeclared_symbols;
}
