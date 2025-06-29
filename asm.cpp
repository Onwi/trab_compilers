#include <iostream>
#include <fstream>
#include <string>
#include "asm.hpp" 
#include "tacs.hpp"

void generateASM(TAC *head) {
  int TAC_PRINT_count = 0;
  int lit_string_print = 0;
  std::ofstream fout("res.s");

  // init
  fout << "\t.file \"res.c\"\n\t.text\n\n";

  // Hash Table
  printAsm(fout);

  // each TAC
  TAC *tac;
  for(tac = head; tac; tac = tac->next){
    switch (tac->type) {
      case TAC_BEGINFUN:
        fout <<
          "\t.globl " + tac->res->text + "\n"
          "\t.type  " + tac->res->text + ", @function\n"
          "" + tac->res->text + ":\n"
          "\tendbr64\n"
          "\tpushq	%rbp\n"
          "\tmovq	%rsp, %rbp\n\n"
          ;
        break;
      case TAC_ENDFUN:
        fout << 
          //"\tleave\n"
          //"\tret\n"
          ""
          ;
        break;
      case TAC_RET:
        if (tac->res->type == 0)  {
         fout <<
          "\tmovl	-4(%rbp), %eax\n"
          "\tleave\n"
          "\tret\n"
          ;
        } else {
          fout <<
          "\tmovl	$"+tac->res->text+", %eax\n"
          "\tpopq	%rbp\n"
          "\tret\n"
          ;         
        }
        break;
      case TAC_PRINT:
        if (tac->res->type == SYMBOL_LIT_STRING) {
          fout <<
            "\tleaq	_s"+std::to_string(lit_string_print)+"(%rip), %rax\n"
            "\tmovq	%rax, %rdi\n"
            "\tcall	printf@PLT\n"
          ;
          lit_string_print++;
        } else {
          fout <<
            "\tmovl	_"+tac->res->text+"(%rip), %eax\n"
            "\tmovl	%eax, %esi\n"
            "\tleaq	_"+tac->res->text+"(%rip), %rax\n"
            "\tmovq	%rax, %rdi\n"
            "\tmovl	$0, %eax\n"
            "\tcall	printf@PLT\n"
          ;
        }
        TAC_PRINT_count++;
        break;
      case TAC_MOVE:
        break;
      default:
        break;
    }
  }

  fout.close();
}
