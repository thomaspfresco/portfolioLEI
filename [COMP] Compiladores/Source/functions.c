/*Miguel Ferreira, nº 2019214567 */
/*Thomas Fresco, nº 2019219057 */

#include "functions.h"

nodeTab* tableRoot = NULL;
nodeTab* currentFunc = NULL;

int semanticErrors = 0;

char *toLower(char *str, size_t len) {
    char *str_l = calloc(len+1, sizeof(char));
    for (size_t i = 0; i < len; ++i) str_l[i] = tolower((unsigned char)str[i]);
    return str_l;
}

// Arvore de sintaxe abstrata ---------------------------------------------------
//-------------------------------------------------------------------------------

//criar novo nó
node* createNode(char* type, char* token, int line, int column) {
	node* newNode = malloc(sizeof(struct node));

	newNode->note = NULL;
	newNode->line = line;
	newNode->column = column;

	if (token == NULL) newNode->token = token;
	else newNode->token = strdup(token);

	if (type == NULL) newNode->type = type;
	else newNode->type = strdup(type);
	
	newNode->child = NULL;
	newNode->brother = NULL;

	return newNode;
}

//inserir nó filho
void insertChild(node* parent, node* child) {
	parent->note = NULL;
	child->note = NULL;
	if((parent != NULL) && (parent->child == NULL)) parent->child = child;
}

//inserir nó irmao
void insertBrother(node* current, node* new) {
	current->note=NULL;
	new->note=NULL;
	if(current != NULL) {
		while (current->brother != NULL) {
			current = current->brother;
		}
		current->brother = new;
	}
}

//imprime a árvore
void printTree(node* current, int pontos) {
	
	if(current == NULL) return;

	if(current->token != NULL) {
		for(int i=0; i < pontos; i++) printf(".");
		printf("%s(%s)", current->type, current->token);

	}
	else if(current->type != NULL) {
		for(int i=0; i < pontos; i++) printf(".");
	    printf("%s", current->type);
	}

	if (current->note != NULL) {
		printf(" - %s\n",current->note);
	} 
	else if(current->note == NULL && current->type != NULL) printf("\n"); 

	printTree(current->child, pontos+2);
	printTree(current->brother, pontos);
}

//verificacao de blocks com < 2 statements
int validBlock(node* no) {
		int counter=0;

		while(no != NULL) {
			if(no->type != NULL) {
				counter++;
			}
			no = no->brother;
		}

		return counter;
}

//loop para assign do tipo a todas as variaveis
void loopVarSpec(node* no, node* tipo) {

	while(no != NULL) {                                                                                                  
    	if (strcmp(no->child->type,"loopVarSpec")==0) strcpy(no->child->type,tipo->type);
    	
		no=no->brother;
    }
}

//libertar memoria alocada para a arvore
void freeTree(node *current) {
   
    if (current==NULL) return;

    if (current->type != NULL) free(current->type);
 
    if (current->token != NULL) free(current->token);

    freeTree(current->child);
    freeTree(current->brother);
    free(current);
}

// Tabela de Simbolos -----------------------------------------------------------
//-------------------------------------------------------------------------------

//insere novo no na tabela/cria tabela
nodeTab* insertTable(char* label, char* varType, char* varName, nodeTab * funcao){
    
	//cabecalho
	if (strcmp(label,"Global")==0) {
		nodeTab* noTab = malloc(sizeof(struct nodeTab));
		noTab->label = label;
		noTab->next = NULL;
		noTab->varType = NULL;
		noTab->varName = NULL;
		if (tableRoot == NULL) return noTab;

		nodeTab* current = tableRoot;

		while (current->next != NULL) current = current->next;

		current->next = noTab;

    	return noTab;
	}

	//variaveis globais
	else if (funcao == NULL && strcmp(label,"Global")!=0) {
		nodeTab* noTab = malloc(sizeof(struct nodeTab));
		noTab->label = label;
		noTab->next = NULL;
		noTab->varType = varType;
		noTab->varName = varName;
		if (tableRoot == NULL) return noTab;

		nodeTab* current = tableRoot;

		while (current->next != NULL) current = current->next;

		current->next = noTab;

    	return noTab;
	} 
	//funcoes
	else {
		if (tableRoot == NULL) return funcao;
		nodeTab* current = tableRoot;
		while (current->next != NULL) current = current->next;
		current->next = funcao;
		return funcao;
	}
}

//verifica a presenca de uma variavel na tabela
//da return do type se existir, null se não existir
char* verificaVar(char* varName, nodeTab *funcao, int line) {
	//considerar parametros como variaveis dentro da funcao!!!!!!!!!!!!!!!!!!
	nodeTab* current = tableRoot;
	var* auxVar = NULL;

	//salta cabeçalho
	current = current->next;
	
	//dentro de funcao
	if (funcao != NULL) {
		auxVar = funcao->rootVars;

		while(auxVar != NULL) {
			//comparacao com variaveis locais
			if (strcmp(varName,auxVar->varName) == 0) if(auxVar->line < line) return auxVar->varType;
			auxVar=auxVar->nextVar;
		}
	}

	//fora de funcao	
	while(current != NULL) {
		//comparacao com variaveis globais
		if (current->varName != NULL) {
			if (strcmp(varName,current->varName) == 0) return current->varType;
		} 
		else break;
		current = current->next;
	}
			
	
	return NULL;
}

//verifica a presenca de uma funcao na tabela
//da return do returnType se existir, null se não existir
char* verificaFunc(char* funcName) {
	nodeTab* current = tableRoot;
	
	while(current != NULL) {
		if (current->varName == NULL && strcmp(current->label,"Global") != 0) {
			if (strcmp(funcName,current->funcName) == 0) return current->returnType;
		} 
		current = current->next;
	}

	return NULL;
}

//verifica a presenca de um parametro de uma funcao na tabela
//da return do returnType se existir, null se não existir
char* verificaParams(char* paramName, nodeTab* funcao) {
	param* auxParams = NULL;

	if (funcao != NULL) {
		auxParams = funcao->rootParams;

		while(auxParams != NULL) {
			//tenta encontrar o nome do parametro
			if (strcmp(paramName,auxParams->paramName) == 0) return auxParams->paramType;
			auxParams=auxParams->nextParam;
		}
	}

	return NULL;
}

//inserir declaracao de variaveis
int insertVarDecl(node* no) {
	node* aux = NULL;

	if (no->child == NULL) aux = no->child->brother;
	else aux = no->child;

	//verificar se a variavel ja foi delcarada
	//if (verificaVar(aux->brother->token,NULL, current->line) != NULL) {
	//	printf("Line <linha>, column <coluna>: Symbol %s already defined\n",aux->brother->token);
	//	semanticErrors = 1;
	//}
	//else 
	insertTable("Variavel",toLower(aux->type,strlen(aux->type)),aux->brother->token,NULL);

	return 0;
}

//insere parametros de funcoes
void insertParams(nodeTab *funcao, char *paramType, char *paramName) {

	param* parametro = malloc(sizeof(struct param));
	
	parametro->paramType = paramType;
	parametro->paramName = paramName;
	parametro->nextParam = NULL;

	if (funcao->rootParams == NULL) funcao->rootParams = parametro;

	else {
		param* current = funcao->rootParams;

	while (current->nextParam != NULL) current = current->nextParam;

	current->nextParam = parametro;
	}
}

//encontra parametros de funcoes
int findParams(nodeTab *funcao,node *no) {

	node *aux;

	//a funcao nao tem parametros
	if (no->child == NULL) return 1;

	//a funcao tem parametros
	else {
		aux = no->child;
		while (aux != NULL) {
			insertParams(funcao,toLower(aux->child->type,strlen(aux->child->type)),aux->child->brother->token);
			aux = aux->brother;
		}
	}
	return 0;
}

void insertVars(nodeTab *funcao, char *varType, char *varName, int line) {

	var* variavel = malloc(sizeof(struct var));
	
	variavel->varType = varType;
	variavel->varName = varName;
	variavel->nextVar = NULL;
	variavel->line = line;

	if (funcao->rootVars == NULL) funcao->rootVars = variavel;

	else {
		var* current = funcao->rootVars;

	while (current->nextVar != NULL) current = current->nextVar;

	current->nextVar = variavel;
	}
}

int findVars(nodeTab *funcao,node *no) {
	node *aux;

	if (no == NULL) return 1;

	if (no->type != NULL) {
		if (strcmp(no->type,"FuncDecl") == 0) return 1;


		if(strcmp(no->type, "VarDecl") == 0) {
			if (no->child == NULL) aux = no->child->brother;
			else aux = no->child;
			
			//verifica se a variavel já foi definida globalmente ou na propria funcao
			//if (verificaVar(aux->brother->token,currentFunc, current->line) != NULL) {
			//	printf("Line <linha>, column <coluna>: Symbol %s already defined\n",aux->brother->token);
			//	semanticErrors = 1;
			//}
			//else 
			insertVars(funcao,toLower(aux->type,strlen(aux->type)),aux->brother->token, aux->line);

		}	
	}

	findVars(funcao,no->child);
	findVars(funcao,no->brother);
	return 0;
}

int insertFuncDecl(node* no) {
	node* aux = NULL;
	char *funcName;
	char *returnType;

	nodeTab *funcao = malloc(sizeof(struct nodeTab));

	funcao->label = "Funcao";
	funcao->varName = NULL;
	funcao->varType = NULL;

	//iguala "FuncHeader" ao nó funcHeader
	node* funcHeader = no->child;
	aux = no->child->child;
	funcName = aux->token;
	
	//a funcao tem return "none", passa para FuncParams
	if (strcmp(aux->brother->type,"FuncParams")==0) {
		returnType = "none";
		aux = aux->brother;
	}
	//a funcao tem return type, passa para FuncParams
	else {
		returnType = toLower(aux->brother->type,strlen(aux->brother->type));
		aux = aux->brother->brother;
	}

	funcao->funcName = funcName;
	funcao->returnType = returnType;
	funcao->rootParams = NULL;
	funcao->rootVars = NULL;

	//varivel global para saber em que funcao vai
	currentFunc = funcao;

	//verificar se a funcao ja foi delcarada
	//if (verificaFunc(funcName) != NULL) {
	//	printf("Line <linha>, column <coluna>: Symbol %s already defined\n",funcName);
	//	semanticErrors = 1;
	//} 
	//else {

	//inserir os parametros da funcao
	findParams(funcao,aux);
	//inserir as variaveis declaradas dentro da funcao
	findVars(funcao,funcHeader);

	insertTable("Function",NULL,NULL,funcao);
	//}

	return 0;
}

nodeTab* findFunc(char* funcName) {
	nodeTab* current = tableRoot;
	
	while(current != NULL) {
		if (current->varName == NULL && strcmp(current->label,"Global") != 0) {
			if (strcmp(funcName,current->funcName) == 0) return current;
		} 
		current = current->next;
	}

	return NULL;
}

//vai encontrar as funcoes na arvore, argumento é a root
void startAnot(node* no) {
	no = no->child; 
	node* aux; 

	while(no != NULL) {
		if(no->type != NULL) {
			if(strcmp(no->type, "FuncDecl") == 0) {
				aux = no->child->brother->child; //filho do funcBody
				nodeTab* func = findFunc(no->child->child->token); //ponteiro da funcao na tabela

				anotateTree(aux, func);
				
			}
		}
		
		no = no->brother;
	}
	
	 
}

//anota a Arvore de sintaxe abstrata
char* anotateTree(node* current, nodeTab* tableRef) {
	if (current == NULL) return NULL;

	anotateTree(current->brother, tableRef);

	if(current->type != NULL) {
		
		//ignora o VarDecl
		if(strcmp(current->type, "VarDecl") == 0) {
			return NULL;
		}

		//anota o parseArgs
		else if(strcmp(current->type, "ParseArgs") == 0) {
			char* type = anotateTree(current->child, tableRef);
			current->note = type;
			return current->note;
		}
	
		//anota o return type da funcao
		else if(strcmp(current->type, "Call") == 0) {
			char* type = anotateTree(current->child, tableRef);
			if (strcmp(type, "none") != 0) current->note = type;
		
			return current->note;
		}

		//anotacao em return
		else if(strcmp(current->type, "Return") == 0) {

		}

		//anotacao em if
		else if(strcmp(current->type, "If") == 0) {
			//verifica se o filho e bool, se nao for da erro
			
		}

		else if(strcmp(current->type, "And") == 0 || strcmp(current->type, "Or") == 0 || strcmp(current->type, "Lt") == 0 || strcmp(current->type, "Gt") == 0 || strcmp(current->type, "Eq") == 0 || strcmp(current->type, "Ne") == 0 || strcmp(current->type, "Le") == 0 || strcmp(current->type, "Ge") == 0) {
			anotateTree(current->child, tableRef);
			anotateTree(current->child->brother, tableRef);
			
			current->note = "bool";
			return current->note;
		}

		else if(strcmp(current->type, "Not") == 0) {
			anotateTree(current->child, tableRef);
			
			current->note = "bool";
			return current->note;
		}

		else if(strcmp(current->type,"Minus")==0||strcmp(current->type,"Plus")==0) { 
			char *typeOp1;
			//verificar se as variaveis foram declaradas!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			typeOp1 = anotateTree(current->child, tableRef);
			
			current->note = typeOp1;
		
			return current->note;
		}

		//anotacao em for
		else if(strcmp(current->type, "For") == 0) {
			//verificar se os filhos sao bool
		}

		else if(strcmp(current->type, "IntLit") == 0) {
			current->note = "int";
			return "int";
		}

		else if(strcmp(current->type, "RealLit") == 0) {
			current->note = "float32";
			return "float32";
		}

		else if(strcmp(current->type, "Id") == 0) {
			//verificar tabela de vars e de funcs
			if (verificaParams(current->token, tableRef) != NULL) {
				current->note = verificaParams(current->token, tableRef);
				return current->note;
			}
			else if (verificaFunc(current->token) != NULL) { //funcs
				char* retType = verificaFunc(current->token);
				param *auxParams = findFunc(current->token)->rootParams;
				char auxNote[1000] = "(";	
				while (auxParams != NULL) {
					strcat(auxNote,auxParams->paramType);
					if (auxParams->nextParam != NULL) strcat(auxNote,",");
					auxParams=auxParams->nextParam;
				}
				strcat(auxNote,")");
				current->note = strdup(auxNote);

				return retType;
			}
			else if (verificaVar(current->token, tableRef, current->line) != NULL) { //vars locais
				current->note = verificaVar(current->token, tableRef, current->line);
				return current->note;
			}
			else {
				
				current->note = "undef";
				return current->note;
				//ERRO!!!!!!!!!
			}
		}

		else if(strcmp(current->type,"Add")==0 || strcmp(current->type,"Sub")==0 || strcmp(current->type,"Mul")==0 || strcmp(current->type,"Div")==0 || strcmp(current->type,"Mod")==0) {
			char *typeOp1;
			//char *typeOp2;
			//verificar se as variaveis foram declaradas!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			typeOp1 = anotateTree(current->child, tableRef);
			anotateTree(current->child->brother, tableRef);
			
			//printf("type1: %s	type2: %s\n", current->child->type, current->child->brother->type);
			
			// (2 bools, strings ou undef) ou tipos diferentes
			/*if (((strcmp(typeOp1,"bool")==0 && strcmp(typeOp2,"bool")==0) || (strcmp(typeOp1,"string")==0 && strcmp(typeOp2,"string")==0) || (strcmp(typeOp1,"undef")==0 && strcmp(typeOp2,"undef")==0)) || (strcmp(typeOp1,typeOp2) != 0)) {
				
				/if(strcmp(current->type,"Add")==0) {
					printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n",current->line,current->column,"+",typeOp1,typeOp2);             
				}
				else if(strcmp(current->type,"Sub")==0) {
					printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n",current->line,current->column,"-",typeOp1,typeOp2);              
				}
				else if(strcmp(current->type,"Mul")==0) {
					printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n",current->line,current->column,"*",typeOp1,typeOp2);             
				}
				else if(strcmp(current->type,"Div")==0) {
					printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n",current->line,current->column,"/",typeOp1,typeOp2);            
				}
				else if(strcmp(current->type,"Mod")==0) {
					printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n",current->line,current->column,"%",typeOp1,typeOp2);              
				}
				semanticErrors = 1;
				current->note = "undef";
				return "undef";
			}
			//operandos do mesmo tipo
			else if(strcmp(typeOp1,typeOp2) == 0) {
				current->note = typeOp1;
			}*/
			
			current->note = typeOp1;
			return current->note;
		}
		//anotacao em assigns
		else if(strcmp(current->type, "Assign") == 0) {
			//printf("teste1\n");
			char *typeOp1;
			//char *typeOp2;
			//verificar se as variaveis foram declaradas!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			typeOp1 = anotateTree(current->child, tableRef);
			anotateTree(current->child->brother, tableRef);

			//verificar se var existe na tabela (ja foi declarada)!!!!!!!!!!!!!!!!!!!!!!!!!!
			
			//verificar se os tipos correspondem declaracao/assign!!!!!!!!!!!!!!!!!!!!!!!!!!

			
			/*if(strcmp(typeOp1,typeOp2) == 0) {
				current->note = typeOp1;
			}*/
			current->note = typeOp1;
			return current->note;
		}

	}

	anotateTree(current->child, tableRef);
	//anotateTree(current->brother, tableRef);
	
	return NULL;
}

//constroi a tabela de simbolos percorrendo a Arvore de sintaxe abstrata
void buildTab(node* currentNode) {

	if (currentNode->type != NULL) {
		if(strcmp(currentNode->type, "Program") == 0) tableRoot = insertTable("Global",NULL,NULL,NULL);
	}
	else return;

	//passa da raiz para o primeiro filho (globais)
	currentNode = currentNode->child;

	while(currentNode != NULL) {

		if (currentNode->type != NULL) {
			if((strcmp(currentNode->type, "VarDecl") == 0)) insertVarDecl(currentNode);
			else if(strcmp(currentNode->type, "FuncDecl") == 0) insertFuncDecl(currentNode);
		}
		currentNode = currentNode->brother;
	}
}


//imprime a tabela
void printTab(nodeTab* current) {
	param* auxParam = NULL;
	var* auxVars = NULL;

	//global
	while(current != NULL) {
		
		//program
		if(strcmp(current->label,"Global") == 0) printf("===== %s Symbol Table =====\n", current->label);

		//declaracao de variaveis
		if(strcmp(current->label,"Variavel") == 0) printf("%s\t\t%s\n", current->varName,current->varType);

		//declaracao de funcoes
		if(strcmp(current->label,"Funcao") == 0) {
			
			printf("%s\t(", current->funcName);
			
			//nao tem parametros
			if (current->rootParams != NULL) {
				auxParam = current->rootParams;

				while (auxParam != NULL) {
					printf("%s",auxParam->paramType);
					if (auxParam->nextParam != NULL) printf(",");
					auxParam = auxParam->nextParam;
				}
			}
			printf(")\t%s\n",current->returnType);
		}

		current = current->next;
	}

	current = tableRoot;
	//funcoes
	while(current != NULL) {
		//declaracao de funcoes
		if(strcmp(current->label,"Funcao") == 0) {
			printf("\n");
			if (current->rootParams == NULL) printf("===== Function %s() Symbol Table =====\n", current->funcName);
			
			else {
				auxParam = current->rootParams;
				printf("===== Function %s(", current->funcName);

				while (auxParam != NULL) {
					printf("%s",auxParam->paramType);
					if (auxParam->nextParam != NULL) printf(",");
					auxParam = auxParam->nextParam;
				}
				printf(") Symbol Table =====\n");
			}

			printf("return\t\t%s\n", current->returnType);

			auxParam = current->rootParams;
			while (auxParam != NULL) {
				printf("%s\t\t%s\tparam\n",auxParam->paramName,auxParam->paramType);
				auxParam = auxParam->nextParam;
			}
			auxVars = current->rootVars;
			while (auxVars != NULL) {
				printf("%s\t\t%s\n",auxVars->varName,auxVars->varType);
				auxVars = auxVars->nextVar;
			}

		}

		current = current->next;
	}
}