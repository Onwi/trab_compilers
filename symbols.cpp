#include <cstdio>
#include <cstdlib>
#include <map>
#include <ostream>
#include <iostream>
#include "symbols.hpp"

std::map<std::string, Symbol*> symbolTable;

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

void printSymbolTable() {
  for (auto s : symbolTable) {
    std::cout << "Symbol[" << s.second->type << "," << s.second->text << "]" << std::endl;
  }
}
