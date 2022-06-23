/*Miguel Ferreira, nº 2019214567 */
/*Thomas Fresco, nº 2019219057 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Arvore de sintaxe abstrata ---------------------------------------------------
//-------------------------------------------------------------------------------

typedef struct node {
	char* type;
	char* token;
	struct node* child;
	struct node* brother;

	//arvore anotada
	char* note;

	//linhas e colunas para erros
	int line;
	int column;
} node;

node* createNode(char* type, char* token, int line, int column);
void insertChild(node* child, node* parent);
void insertBrother(node* current, node* new);
void printTree(node* current, int dots);
void freeTree(node* current);
int validBlock(node* node);
void loopVarSpec(node* no, node* tipo);
void startAnot(node* no);

// Tabela de Simbolos -----------------------------------------------------------
//-------------------------------------------------------------------------------

typedef struct param {
	char *paramType;
	char *paramName;
	struct param* nextParam;
} param;

typedef struct var {
	char *varName;
	char *varType;
	struct var* nextVar;

	//linha
	int line;
} var;

typedef struct nodeTab {

//categoria (global,Function,...)
char *label;

//variaveis
char *varName;
char *varType;

//funcoes
char *funcName;
char *returnType;
struct param* rootParams;
struct var* rootVars;

struct nodeTab* next;


} nodeTab;

void buildTab(node* current);
void printTab(nodeTab* current);

//Anotacao da arvore
char* anotateTree(node* current, nodeTab* tableRef);