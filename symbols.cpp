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

FUNC_PARAMS* GetLastFP(SYMBOL* symb) {
  FUNC_PARAMS* fp = symb->funcParams;
  while(fp->nextParam != NULL) {
    fp = fp->nextParam;
  }
  return fp;
}

int GetNumberParamsFP(FUNC_PARAMS* fp) {
  int count = 0;
  while(fp != NULL) {
    fp = fp->nextParam;
    count++;
  }
  return count;
}

Symbol *insert(int type, std::string text, int dataType) {
  std::map<std::string, Symbol*>::iterator it = symbolTable.find(text);
  if (it != symbolTable.end())  {
    return it->second;
  }

  Symbol *newSymbol = (Symbol *) malloc(sizeof(Symbol)); 
  newSymbol->type = type;
  newSymbol->text = text;
  newSymbol->dataType = dataType;
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
