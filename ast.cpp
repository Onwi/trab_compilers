#include <cstdio>
#include "ast.hpp"

std::string AstTypesNames[] = {
  "AST_UNKNOWN", "AST_SYMBOL", "AST_SYMBOL_LIT",
  // types
  "AST_INT", "AST_CHAR", "AST_REAL", "AST_STRING", "AST_BYTE", "AST_LIT",
  // operations
  "AST_ADD", "AST_SUB", "AST_MUL", "AST_DIV", "AST_AND", "AST_OR",
  "AST_LE", "AST_GE", "AST_EQ", "AST_DIF", "AST_LSR", "AST_GTR", "AST_NOT",
  // ATTRIBUTIONs
  "AST_ATTB", "AST_ATTB_VEC",
  // FLOW 
  "AST_IF", "AST_IF_ELSE", "AST_WHILE", "AST_DO_WHILE",
  // COMMANDS
  "AST_READ", "AST_PRINT", "AST_PRINT_REST", "AST_RETURN", "AST_EMPTY_CMD",
  // COMMAND BLOCKS
  "AST_CMDBLK", "AST_EMPTY_CMDBLK", "AST_CMDL", 
  "AST_FUNCTION", "AST_FUNCTION_ARG", "AST_FUNCTION_ARGS", 
  "AST_VARIABLES", "AST_VARIABLES_VEC", "AST_VARIABLES_ARG",
  // BACKBONE
  "AST_DECL", "AST_PROGRAM", "AST_SYMBOL_ID", "AST_SYMBOL_VEC", "AST_PRTSS",
  "AST_SYMBOL_FUNCTION", "AST_SYMBOL_FUNCTION_ARGS"
};

void printAST(AST *node, int level) {
  if (!node) return;

  for (int i = 0; i < level; ++i) 
    fprintf(stderr, "    ");

  fprintf(stderr, "AST: %s, %s \n", AstTypesNames[node->type].c_str(), 
          node->symbol ? node->symbol->text.c_str() : "");

  for (auto s : node->child) {
    printAST(s, level+1);
  }
}

const char *operations[] = {
	"+", "-", "*", "/",
	"&", "|", "<=", ">=", "==", "!=", "<", ">", "~"
};

void astWrite(AST* node, FILE* file) {
	if(node != NULL) {
		switch(node->type) {

			// UNKNOWN:
			case AST_UNKNOWN: 
				fprintf(stderr, "UNKNOWN TOKEN ERROR!");
				fprintf(file, "UNKNOWN TOKEN ERROR!!");
				break;

			// SYMBOLS:
			case AST_SYMBOL_LIT:
			case AST_SYMBOL_ID:
			case AST_STRING: 
				fprintf(file, "%s", node->symbol->text.c_str());
				break;

			case AST_SYMBOL_VEC:
				fprintf(file, "%s[", node->symbol->text.c_str());
				astWrite(node->child[0], file);
				fprintf(file, "]");
				break;

			case AST_SYMBOL_FUNCTION:
				fprintf(file, "%s(", node->symbol->text.c_str());
				if(node->child.size()) {
					astWrite(node->child[0], file);
				}
				fprintf(file, ")");
				break;

			case AST_SYMBOL_FUNCTION_ARGS:
				astWrite(node->child[0], file);
				if(node->child[1]) {
					fprintf(file, ", ");
					astWrite(node->child[1], file);
				}
				break;
				
			// LITERAL FORWARD:
			case AST_LIT: 
				astWrite(node->child[0], file);
				break;

			// TYPES
			case AST_BYTE:
				fprintf(file, "byte ");
				break;

			case AST_INT:
				fprintf(file, "int ");
				break;

			case AST_REAL:
				fprintf(file, "real ");
				break;

			// EXPRESSIONS OPERATORS:
			case AST_ADD:
			case AST_SUB:
			case AST_MUL:
			case AST_DIV:
			case AST_AND:
			case AST_OR:
			case AST_LE:
			case AST_GE:
			case AST_EQ:
			case AST_DIF:
			case AST_LSR:
			case AST_GTR:
			case AST_NOT:
				astWrite(node->child[0], file);
				fprintf(file, "%s", operations[node->type - 9]);
				astWrite(node->child[1], file);
				break;

			case AST_PRTSS:
				fprintf(file, "(");
				astWrite(node->child[0], file);
				fprintf(file, ")");
				break;

			// ATTRIBUTIONS:
			case AST_ATTB:
				fprintf(file, "%s=", node->symbol->text.c_str());
				astWrite(node->child[0], file);
				fprintf(file, ";\n");
				break;

			case AST_ATTB_VEC:
				fprintf(file, "%s[", node->symbol->text.c_str());
				astWrite(node->child[0], file);
				fprintf(file, "]=\n");
				astWrite(node->child[1], file);
				fprintf(file, ";\n");
				break;

			// FLOW CONTROL:
			case AST_IF:
				fprintf(file, "if(");
				astWrite(node->child[0], file);
				fprintf(file, ")\n");
				astWrite(node->child[1], file);
				break;

			case AST_IF_ELSE:
				fprintf(file, "if(");
				astWrite(node->child[0], file);
				fprintf(file, ")\n");
				astWrite(node->child[1], file);
				fprintf(file, "else\n");
				astWrite(node->child[2], file);
				break;

			case AST_WHILE:
				fprintf(file, "while ");
				astWrite(node->child[0], file);
				fprintf(file, " do\n");
				astWrite(node->child[1], file);
				break;

			case AST_DO_WHILE:
				fprintf(file, "do ");
				astWrite(node->child[0], file);
				fprintf(file, "while ");
				astWrite(node->child[1], file);
				fprintf(file, ";\n");
				break;

			// OTHER COMMANDS:
			case AST_READ:
				fprintf(file, "read %s;\n", node->symbol->text.c_str());
				break;

			case AST_PRINT:
				fprintf(file, "print ");
				astWrite(node->child[0], file);
				if(node->child[1]) {
					fprintf(file, " ");
					astWrite(node->child[1], file);
				}
				fprintf(file, ";\n");
				break;

			case AST_PRINT_REST:
				astWrite(node->child[0], file);
				if(node->child[1]) {
					fprintf(file, " ");
					astWrite(node->child[1], file);
				}
				break;

			case AST_RETURN:
				fprintf(file, "return ");
				astWrite(node->child[0], file);
				fprintf(file, ";\n");
				break;
			
			case AST_EMPTY_CMD:
				fprintf(file, ";\n");
				break;

			// COMMAND BLOCKS:
			case AST_CMDBLK:
				fprintf(file, "{\n");
				astWrite(node->child[0], file);
				fprintf(file, "}\n");
				break;

			case AST_EMPTY_CMDBLK:
				fprintf(file, "{}\n");
				break;
			
			case AST_CMDL:
				astWrite(node->child[0], file);
				if(node->child[1]) {
					astWrite(node->child[1], file);
				}
				break;

			// FUNCTION DECLARATIONS:
			case AST_FUNCTION:
				astWrite(node->child[0], file);
				fprintf(file, "%s(", node->symbol->text.c_str());
				if(node->child[1]) {
					astWrite(node->child[1], file);
				}
				fprintf(file, ")");
				astWrite(node->child[2], file);
				break;

			case AST_FUNCTION_ARGS:
				astWrite(node->child[0], file);
				if(node->child[1]) {
					fprintf(file, ", ");
					astWrite(node->child[1], file);
				}
				break;
			
			case AST_FUNCTION_ARG:
				astWrite(node->child[0], file);
				fprintf(file, "%s", node->symbol->text.c_str());
				break;

			// VARIABLE DECLARATIONS:
			case AST_VARIABLES:
				astWrite(node->child[0], file);
				fprintf(file, "%s=", node->symbol->text.c_str());
				astWrite(node->child[1], file);
				fprintf(file, ";\n");
				break;
			
			case AST_VARIABLES_VEC:
				astWrite(node->child[0], file);
				fprintf(file, "%s[", node->symbol->text.c_str());
				astWrite(node->child[1], file);
				fprintf(file, "]");
				if(node->child[2]) {
					fprintf(file, "=");
					astWrite(node->child[2], file);
				}
				fprintf(file, ";\n");
				break;
			
			case AST_VARIABLES_ARG:
				astWrite(node->child[0], file);
				if(node->child[1]) {
					fprintf(file, ", ");
					astWrite(node->child[1], file);
				}
				break;

			// BACKBONE:
			case AST_DECL:
				astWrite(node->child[0], file);
				astWrite(node->child[1], file);
				break;

			case AST_PROGRAM:
				astWrite(node->child[0], file);
				break;

			default: 
				fprintf(stderr, "ERRO! call 911!");
				fprintf(file, "ERRO! call 911!");
				break;
		}	
	}
}


