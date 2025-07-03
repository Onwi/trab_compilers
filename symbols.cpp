#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <map>
#include <ostream>
#include <iostream>
#include <string>
#include "symbols.hpp"
#include "tacs.hpp"

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

Symbol *makeTemp() {
  static int serialTemp = 0;
  static char buffer[32];
  snprintf(buffer, 32, "temp_%d", serialTemp++);
  return insert(SYMBOL_IDENTIFIER, buffer);
}

Symbol *makeLabel() {
  static int serialLabel = 0;
  static char buffer[32] = "";
  snprintf(buffer, 32, "label_%d", serialLabel++);
  return insert(SYMBOL_IDENTIFIER, buffer);
}

void printSymbolsTable() {
  for (auto s : symbolTable) {
    std::cout << "Symbol: " << s.second->text << ", SymbolType: " << s.second->type << ", dataType: " << s.second->dataType << std::endl;
  }
}

void printAsm(std::ofstream &fout, TAC* head) {
  fout << ".data\n";
  fout << ".LC0:\n";
  fout << "\t.string \"%d\\n\"\n";
  int stringCount = 0;
  for (auto s : symbolTable) {
    //printf("lolol:%s\n", s.second->text.c_str());
    std::string type;
    switch (s.second->type) {
      case SYMBOL_LIT_STRING:
        type = ".string";
        fout << "_s" + std::to_string(stringCount) + ":\n";
        fout << "\t" + type + " " + s.second->text + "\n";
        fout << "\t.text\n";
        stringCount++;
        break;
      case SYMBOL_LIT_INT:
        type= ".long";
        fout << "_" + s.second->text + ":\n";
        fout << "\t" + type + " " + s.second->text + "\n";
        break;
    }
  }

  TAC *aux;
  for (aux = head; aux; aux = aux->next) {
    if (aux->type == TAC_MOVE && aux->op1 != NULL) {
        std::string type= ".long";
        fout << "_" + aux->res->text + ":\n";
        fout << "\t" + type + " " + aux->op1->text + "\n";
    }
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
