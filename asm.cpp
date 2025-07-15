#include <cstdio>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <vector>
#include "asm.hpp" 
#include "tacs.hpp"

void generateASM(TAC *head) {
  int TAC_PRINT_count = 0;
  int lit_string_print = 0;
  std::ofstream fout("res.s");

  // init
  fout << "\t.file \"res.c\"\n\t.text\n\n";

  // Hash Table
  printAsm(fout, head);
  std::vector<std::string> registers;
  registers.push_back("%ecx");
  registers.push_back("%edx");
  registers.push_back("%esi");
  registers.push_back("%edi");
  int k=0;
  int n_params =0;
  int z=1;

  // each TAC
  TAC *tac;
  TAC *aux;
  for(tac = head; tac; tac = tac->next){
    aux = tac;
    switch (tac->type) {
      case TAC_BEGINFUN:
        fout <<
          "\t.globl " + tac->res->text + "\n"
          "\t.type  " + tac->res->text + ", @function\n"
          "" + tac->res->text + ":\n"
          "\tendbr64\n"
          "\tpushq	%rbp\n"
          "\tmovq	%rsp, %rbp\n\n";
        while (aux->next->type == TAC_VARIABLE) {
          n_params++;
          aux = aux->next;
        }
        for (int i = 0; i < n_params; i++) {
          fout <<
            "\tmovl "+registers.at(n_params-1-i)+", -"+std::to_string(z*4)+"(%rbp)\n"
            ;
          z++;
        }
        n_params=0;
        z=0;
        break;
      case TAC_ENDFUN:
        fout << 
          //"\tleave\n"
          //"\tret\n"
          ""
          ;
        break;
      case TAC_CALL:
        fout << "\tsubq %16, %rsp\n";
        aux = tac->prev;
        while (aux->type == TAC_ARG) {
          //printf("aux prv: %s\n", aux->prev->res->text.c_str());
          //tacPrintSingle(aux);
          fout << "\tmovl $"+aux->res->text+", "+registers.at(k)+"\n";
          k++;
          aux = aux->prev;
          aux = aux->prev;
          //printf("current aux: %s\n", aux->res->text.c_str());
          //printf("2 aux prv: %s\n", aux->res->text.c_str());
          //tacPrintSingle(aux);
          //printf("pq nao printa: %d\n", aux ? aux->type : 1000100);
        }
        k=0;
        fout << "\tcall "+tac->op1->text+"\n";
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
        } else if (tac->res->type == SYMBOL_LIT_INT) {
          fout <<
            "\tmovl $"+tac->res->text+", %esi\n"
            "\tleaq	.LC0(%rip), %rax\n"
            "\tmovq	%rax, %rdi\n"
            "\tmovl	$0, %eax\n"
            "\tcall	printf@PLT\n"
          ;
        } else {
          fout <<
            "\tmovl	_"+tac->res->text+"(%rip), %eax\n"
            "\tmovl	%eax, %esi\n"
            // only suporting int rn
            "\tleaq	.LC0(%rip), %rax\n"
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
