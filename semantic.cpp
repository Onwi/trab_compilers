#include "semantic.hpp"
#include <cstdio>
#include <string.h>

int SemanticErrors = 0;


FUNC_PARAMS* auxFP;
int countFP;
int countExp;
int vecSize;
int count;
AST* auxNode;
AST* auxExp;
std::string auxString;
char* auxAux;

bool CheckDTIsIntByte(int datatype) {
  return datatype == DATATYPE_BYTE || datatype == DATATYPE_INT;
}

bool CheckSiblingsDT(AST* node, int datatype) {
  return node->child[0]->dataType == datatype || node->child[1]->dataType == datatype;
}

bool CheckSameDT(int dt1, int dt2) {
  return (CheckDTIsIntByte(dt1) && CheckDTIsIntByte(dt2)) || dt1 == dt2;
}

int GetNumberParamsExp(AST* node) {
  int count = 0;
  while(node != NULL) {
    node = node->child[1];
    count++;
  }
  return count;
}

// copied from https://www.geeksforgeeks.org/reverse-string-in-c/
char* ReverseString(char* string) {
  int left = 0;
  int right = strlen(string) - 1;
  char aux;

  while (left < right) {
    aux           = string[left];
    string[left]  = string[right];
    string[right] = aux;

    left++;
    right--;
  }
  return string;
}

std::string AstTypesNames2[] = {
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

void check_and_set_declarations(AST *node) {
  if (!node) return;


  if(node->type == AST_VARIABLES|| node->type == AST_VARIABLES_VEC || node->type == AST_FUNCTION) {
  switch (node->type) {
    case AST_VARIABLES:
      handleTypes(node, SYMBOL_VARIABLE);
      break;
    case AST_VARIABLES_VEC:
      handleTypes(node, SYMBOL_VEC);
      break;
    case AST_FUNCTION: 
      handleTypes(node, SYMBOL_FUNCTION);
      SetFunctionParams(node->child.at(1), node->symbol);
      break;
  }
  }

  for (auto s : node->child) {
    check_and_set_declarations(s);
  }
}

void handleTypes(AST *node, SymbolType type) {
  if (node->symbol) {
    validateDeclaration(node->symbol);
    node->symbol->type = type;
    assignDataType(node);
  }
}

void assignDataType(AST *node) {
  if (!node->child.at(0)) {
    fprintf(stderr, "Unable to find child for node %s\n", node->symbol->text.c_str());
    return;
  }

  switch (node->child.at(0)->type) {
    case AST_INT:
      node->symbol->dataType = DATATYPE_INT;
      break;
    case AST_BYTE:
      node->symbol->dataType = DATATYPE_BYTE;
      break;
    case AST_REAL:
      node->symbol->dataType = DATATYPE_REAL;
      break;
  }
}

void validateDeclaration(Symbol *symbol) {
  if (symbol->type != SYMBOL_IDENTIFIER) {
    fprintf(stderr, "SEMANTIC ERROR: %s already declared\n", symbol->text.c_str());
    ++SemanticErrors;
  }
}

void semantic_check_undeclared() {
  SemanticErrors += check_undeclared();
}

void checkOperands(AST *node, SYMBOL *function) {
  if (!node) return;

  if (node->type == AST_FUNCTION) {
      function = node->symbol;
  } else if(node->type == AST_DECL) {
      function = NULL;
  }
  // gotta go botton up
  for(int i = 0 ; i < (int)node->child.size() ; i++) {
      checkOperands(node->child[i], function);
  }

  switch (node->type) {
    case AST_UNKNOWN: 
		  fprintf(stderr, "UNKNOWN TOKEN");
			break;

	  // SYMBOLS and TYPES
	  case AST_SYMBOL_LIT_CHAR:
		case AST_CHAR:
      node->dataType = DATATYPE_CHAR;
      break;
    case AST_SYMBOL_LIT_INT:
    case AST_INT:
      node->dataType = DATATYPE_INT;
      break;
    case AST_SYMBOL_LIT_REAL:
    case AST_REAL:
      node->dataType = DATATYPE_REAL;
      break;
    case AST_STRING: 
      node->dataType = JUST_FOR_SKIP;
      break;
    case AST_LIT: 
      node->dataType = node->child[0]->dataType;
      break;

    case AST_SYMBOL_ID:
      node->dataType = node->symbol->dataType;
      //printf("\n\n nodeeeee: %s,  %d\n\n", AstTypesNames2[node->type].c_str(), node->symbol->type);

      if(node->symbol->type != SYMBOL_VARIABLE) {
        fprintf(stderr, "Semantic Error: Using a Non-Scalar Expression as a Scalar Expression: \"%s\".\n", node->symbol->text.c_str());
        ++SemanticErrors;
      }
      break;

    // VECTOR ID
    case AST_SYMBOL_VEC:
      node->dataType = node->symbol->dataType;
      if(node->symbol->type != SYMBOL_VEC) {
        fprintf(stderr, "Semantic Error: Using a Non-Vector Expression as a Vector Expression: \"%s\".\n", node->symbol->text.c_str());
        ++SemanticErrors;
      } else if(!
          (
           node->child[0]->dataType == DATATYPE_BYTE ||
           node->child[0]->dataType == DATATYPE_INT
          )
        ) {
        fprintf(stderr, "Semantic Error: Trying to Access Vector with a non integer expression: \"%s\".\n", node->symbol->text.c_str());
        ++SemanticErrors;
      }
			break;

    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
      node->dataType = node->child[0]->dataType;
      if(CheckSiblingsDT(node, DATATYPE_BOOL) || CheckSiblingsDT(node, JUST_FOR_SKIP)) {
        fprintf(stderr, "Semantic Error: Cannot use Operators +,-,*,/ with Boolean Datatype.\n");
        ++SemanticErrors;
      } else if(!CheckSameDT(node->child[0]->dataType, node->child[1]->dataType)) {
        fprintf(stderr, "Semantic Error: Both Sides of an Operation with +,-,*,/ Don't Have Compatible Datatypes.\n");
        ++SemanticErrors;
      }
      break;

    case AST_AND:
    case AST_OR:
      node->dataType = DATATYPE_BOOL;
      if(!CheckSiblingsDT(node, DATATYPE_BOOL)) {
        fprintf(stderr, "Semantic Error: Cannot use Operators &,| without Boolean Datatype.\n");
        ++SemanticErrors;
      }
      break;
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_DIF:
    case AST_LSR:
    case AST_GTR:
      node->dataType = DATATYPE_BOOL;
        if(CheckSiblingsDT(node, DATATYPE_BOOL) || CheckSiblingsDT(node, JUST_FOR_SKIP)) {
          fprintf(stderr, "Semantic Error: Boolean type invalid for <,>,==,!=,<=,>= operators.\n");
          ++SemanticErrors;
        } else if(!CheckSameDT(node->child[0]->dataType, node->child[1]->dataType)) {
          fprintf(stderr, "Semantic Error: left hand side and right hand side don't match for <,>,==,!=,<=,>=.\n");
          ++SemanticErrors;
        }
      break;

    case AST_NOT:
      node->dataType = DATATYPE_BOOL;
      if(node->child[0]->dataType != DATATYPE_BOOL) {
        fprintf(stderr, "Semantic Error: Operator ~ only allowed without Boolean Datatype.\n");
        ++SemanticErrors;
      }
		  break;
    
    // FUNCTION CALL
    case AST_SYMBOL_FUNCTION:
				node->dataType = node->symbol->dataType;
        if(node->symbol->type != SYMBOL_FUNCTION) {
          fprintf(stderr, "Semantic Error: Using a Non-Function Expression as a Function Expression: \"%s(...)\".\n", node->symbol->text.c_str());
          ++SemanticErrors;
        }

        auxFP = node->symbol->funcParams;
        countFP = GetNumberParamsFP(auxFP);
        countExp = 0;
        if(node->child.size()) {
          countExp = GetNumberParamsExp(node->child.at(0));
        }

        if(countFP == countExp) {
          if(countExp != 0) {
            auxExp = node->child[0];
            while(auxFP != NULL) {
              if(!CheckSameDT(auxExp->child[0]->dataType, auxFP->symbol->dataType)) {
                fprintf(stderr, "Semantic Error: Argument from Function \"%s(...)\" has the wrong type.\n", node->symbol->text.c_str());
                ++SemanticErrors;
              }
              auxExp = auxExp->child[1];
              auxFP = auxFP->nextParam;
            }
          }
        }
        else {
          fprintf(stderr, "Semantic Error: Argument Number of Function Call Expression Mismatch: \"%s(...)\".\n", node->symbol->text.c_str());
          ++SemanticErrors;
        }
        break;

      /////////////////////////////////////////////////////////////////////////////////

      case AST_PRTSS:
        node->dataType = node->child[0]->dataType;
        break;

      // ATTRIBUTIONS:
      case AST_ATTB:
        node->dataType = node->symbol->dataType;
        if(node->symbol->type != SYMBOL_VARIABLE) {
          fprintf(stderr, "Semantic Error attb: Using a Non-Scalar Expression as a Scalar Expression: \"%s\".\n", node->symbol->text.c_str());
          ++SemanticErrors;
        } else if(!CheckSameDT(node->child[0]->dataType, node->symbol->dataType)) {
          fprintf(stderr, "Semantic Error: Attempted Attribution Expression to Variable, Datatype Mismatch.\n");
          ++SemanticErrors;
        }
      	break;
        
      case AST_ATTB_VEC:
        node->dataType = node->symbol->dataType;
        if(node->symbol->type != SYMBOL_VEC) {
          fprintf(stderr, "Semantic Error: Using a Non-Vector Expression as a Vector Expression: \"%s\".\n", node->symbol->text.c_str());
          ++SemanticErrors;
        } else if(!CheckSameDT(node->child[1]->dataType, node->symbol->dataType)) {
          fprintf(stderr, "Semantic Error: Attempted Attribution Expression to Variable, Datatype Mismatch.\n");
          ++SemanticErrors;
        } else if(!CheckDTIsIntByte(node->child[0]->dataType)) {
          fprintf(stderr, "Semantic Error: Attempted Attribution Expression to Variable, Datatype Mismatch.\n");
          ++SemanticErrors;
        }
        break;

			// FLOW CONTROL:
      case AST_IF:
      case AST_IF_ELSE:
      case AST_WHILE:
        if(!CheckSameDT(node->child[0]->dataType, DATATYPE_BOOL)) {
          fprintf(stderr, "Semantic Error: Flow Control Expression doesn't Have Boolean Datatype.\n");
          ++SemanticErrors;
        }
        break;

      case AST_DO_WHILE:
        if(!CheckSameDT(node->child[1]->dataType, DATATYPE_BOOL)) {
          fprintf(stderr, "Semantic Error: Flow Control Expression doesn't Have Boolean Datatype.\n");
          ++SemanticErrors;
        }
				break;

			// OTHER COMMANDS:
			case AST_READ:
				if(node->symbol->type != SYMBOL_VARIABLE) {
          fprintf(stderr, "Semantic Error read: Using a Non-Scalar Expression as a Scalar Expression: \"%s\".\n", node->symbol->text.c_str());
          ++SemanticErrors;
        }
				break;

      case AST_PRINT:
			case AST_PRINT_REST:
				break;

			case AST_RETURN:
        if(function == NULL) {
          fprintf(stderr, "Semantic Error: Return Outside a function.\n");
          ++SemanticErrors;
        } else if(!CheckSameDT(function->dataType, node->child[0]->dataType)) {
          fprintf(stderr, "Semantic Error: Return Value Mismatch with Function Datatype.\n");
          ++SemanticErrors;
        }
        break;

      // DATATYPELESS COMMANDS
			case AST_EMPTY_CMD:
			case AST_CMDBLK:
			case AST_EMPTY_CMDBLK:
			case AST_CMDL:
				break;


      // FUNCTION DECLARATIONS:
			case AST_FUNCTION:
				if(node->symbol->type != SYMBOL_FUNCTION) {
          fprintf(stderr, "Semantic Error: Using a Non-Function Expression as a Function Expression: \"%s\".\n", node->symbol->text.c_str());
          ++SemanticErrors;
        }
				break;

      case AST_FUNCTION_ARGS:
      case AST_FUNCTION_ARG:
				break;
      /////////////////////////////////////////////////////////////////////////////////
      // VARIABLE DECLARATIONS:
			case AST_VARIABLES:
				if(node->symbol->dataType != node->child[1]->dataType) {
          fprintf(stderr, "Semantic Error: Declaration Datatype Mismatch.\n");
          ++SemanticErrors;
        }
				break;
			
			case AST_VARIABLES_VEC:
        if(!CheckDTIsIntByte(node->child[1]->dataType)) {
          fprintf(stderr, "Semantic Error: Trying to Declare Vector With a non Integer Expression: \"%s[%d]\".\n", node->symbol->text.c_str(), node->child[0]->dataType);
        }
        auxString = node->child[1]->symbol->text;
        auxAux = ReverseString((char*)auxString.c_str());
        vecSize = atoi(auxAux);
        count = 0;
        auxNode = node->child[2];

        while(auxNode != NULL) {
          if(node->symbol->dataType != auxNode->child[0]->dataType) {
            fprintf(stderr, "Semantic Error: Declaration Datatype Mismatch \"%s\".\n", auxNode->child[0]->symbol->text.c_str());
          ++SemanticErrors;
          }
          count++;
          auxNode = auxNode->child[1];
        }

        if(vecSize != count && count != 0) {
          fprintf(stderr, "Semantic Error: Vector Declaration Size Mismatch.\n");
          ++SemanticErrors;
        }
				break;

      case AST_VARIABLES_ARG:
        break;

     // TYPES
			case AST_BYTE:
        node->dataType = DATATYPE_BYTE;
        break;
     //

			case AST_DECL:
			case AST_PROGRAM:
				break;

			default: 
				break;
    }
  }

void validateOperandsTypes(int operation, AST *left, AST *right) {
  //printf("passing left node: %s\n", AstTypesNames2[left->type].c_str());
  if (!isValidOperand(left->type, left)) {
    fprintf(stderr, "SEMANTIC ERROR: invalid left operand %s for %s\n", AstTypesNames2[left->type].c_str(), AstTypesNames2[operation].c_str());
    ++SemanticErrors;
  }

  //printf("passing right node: %s\n", AstTypesNames2[right->type].c_str());
  if (!isValidOperand(right->type, right)) {
    fprintf(stderr, "SEMANTIC ERROR: invalid right operand %s for %s\n", AstTypesNames2[right->type].c_str(), AstTypesNames2[operation].c_str());
    ++SemanticErrors;
  }
}

bool isValidOperand(int type, AST *node) {
  if (type == AST_ADD ||
      type == AST_SUB ||
      type == AST_MUL ||
      type == AST_DIV ||
      type == AST_AND ||
      type == AST_OR  ||
      type == AST_LE  ||
      type == AST_GE  ||
      type == AST_EQ  ||
      type == AST_DIF ||
      type == AST_LSR ||
      type == AST_GTR ||
      type == AST_NOT
    ) return true;

  if (type == AST_LIT) {
    // std::cout << "dataType: " << node->child.front()->symbol->dataType << std::endl;
    // std::cout << "symbol: " << node->child.front()->symbol->text << std::endl;
    // std::cout << "child symbol type: " << node->child.front()->symbol->type << std::endl;
    return (
          (node->child.front()->symbol->type == SYMBOL_LIT_INT 
           && node->child.front()->symbol->dataType == DATATYPE_INT ) ||

          (node->child.front()->symbol->type == SYMBOL_LIT_REAL
           && node->child.front()->symbol->dataType == DATATYPE_REAL) || 

          (node->child.front()->symbol->type == SYMBOL_LIT_BYTE
           && node->child.front()->symbol->dataType == DATATYPE_BYTE) 
        );
  }

  if (type == AST_SYMBOL_ID) {
    printf("type: %s, \n", AstTypesNames2[node->type].c_str() );
    return ( 
          node->symbol->dataType == DATATYPE_INT ||
          node->symbol->dataType == DATATYPE_REAL ||
          node->symbol->dataType == DATATYPE_BYTE
        );
  }

  if (type == AST_SYMBOL_FUNCTION) {
    printf("symbol type: %d\n", node->symbol->dataType);
        return (
        node->symbol->dataType == DATATYPE_INT  ||
        node->symbol->dataType == DATATYPE_REAL ||
        node->symbol->dataType == DATATYPE_BYTE 
    );
  }

  return false;
}


void validateFunctionParams(AST *node) {

}

void SetFunctionParams(AST* node, SYMBOL* func) {
  if (!node) return;

  if(node->type == AST_FUNCTION_ARGS) {
    if(node->child[0]->symbol != NULL) {
      if(node->child[0]->symbol->type == SYMBOL_IDENTIFIER) {
        node->child[0]->symbol->type = SYMBOL_VARIABLE;
        assignDataType(node->child[0]); }
      else {
        fprintf(stderr, "Semantic Error: Redeclared Variable %s\n", node->child[0]->symbol->text.c_str());
        ++SemanticErrors;
      }
    }
    else {
      fprintf(stderr, "NUCLEAR LEVEL ERROR, TOTAL COMPILER MELTDOWN");
      exit(-1);
    }
    FUNC_PARAMS* fp;
    if(func->funcParams == NULL) {
      func->funcParams = new FUNC_PARAMS(NULL, node->child[0]->symbol);
    }
    else {
      fp = GetLastFP(func);
      fp->nextParam = new FUNC_PARAMS(NULL, node->child[0]->symbol);
    }
    
    SetFunctionParams(node->child[1], func);
  }
}
