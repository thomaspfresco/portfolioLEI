/*Miguel Ferreira, nº 2019214567 */
/*Thomas Fresco, nº 2019219057 */

%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "functions.h"
    
    node* root = NULL; 
    node* child = NULL; 
    node* aux = NULL;
    node* aux2 = NULL;

    //int yydebug = 1;

    extern int line;
    extern int column;

    int yylex (void);
    void yyerror (const char *s);
%}

%union {
char* token;
struct node* node;
}

%token SEMICOLON COMMA BLANKID ASSIGN STAR DIV MINUS PLUS GT LBRACE LPAR LSQ LT MOD NOT RBRACE RPAR RSQ

%token EQ GE LE NE AND OR

%token <token> PACKAGE RETURN ELSE FOR IF VAR INT FLOAT32 BOOL STRING PRINT PARSEINT FUNC CMDARGS

%token RESERVED

/*LITs*/
%token <token> ID REALLIT INTLIT STRLIT

/*simbolos nao terminais*/
%type <node> Program
%type <node> Declarations
%type <node> VarDeclaration
%type <node> VarSpec
%type <node> Type
%type <node> FuncDeclaration
//%type <node> CommaIdType
%type <node> Parameters
%type <node> FuncBody
%type <node> VarsAndStatements
%type <node> Statement
%type <node> StatementSemicolon
%type <node> ParseArgs
%type <node> CommaExpr
%type <node> FuncInvocation
%type <node> Expr
%type <node> ParametersAux
%type <node> CommaId

/*precedencias*/
%left COMMA
%right ASSIGN
%left OR
%left AND
%left LT LE GT GE EQ NE
%left PLUS MINUS
%left STAR DIV MOD
%right NOT
%left LPAR RPAR LSQ RSQ

%nonassoc ELSE IF

%%       

Program:                PACKAGE ID SEMICOLON Declarations           {root = createNode("Program", NULL,line,column); 
                                                                    $$ = root;    
                                                                    insertChild(root, $4);
                                                                    }
                ;
 
Declarations:           Declarations VarDeclaration SEMICOLON       {$$=$1; insertBrother($1, $2);}  /*recursirvidade -> adicionar irmao*/
                |       Declarations FuncDeclaration SEMICOLON      {$$=$1; insertBrother($1, $2);}
                |                                                   {$$=createNode(NULL,NULL,line,column);}
                ;

VarDeclaration:         VAR VarSpec                                 {$$ = $2; /*passa para VarSpec*/}
                |       VAR LPAR VarSpec SEMICOLON RPAR             {$$ = $3; /*passa para VarSpec*/}  
                ;

CommaId:             COMMA ID CommaId                               {$$ = createNode("VarDecl", NULL,line,column);
                                                                    aux=createNode("loopVarSpec",NULL,line,column);
                                                                    insertBrother($$,$3);
                                                                    insertChild($$,aux);
                                                                    insertBrother(aux,createNode("Id",$2,line,column));
                                                                    }

                |       COMMA ID                                    {$$ = createNode("VarDecl", NULL,line,column);
                                                                    aux=createNode("loopVarSpec",NULL,line,column);
                                                                    insertChild($$,aux);
                                                                    insertBrother(aux,createNode("Id",$2,line,column));
                                                                    }
                ;

VarSpec:                ID Type                                     {$$ = createNode("VarDecl", NULL,line,column);
                                                                    insertChild($$, $2); //Type torna-se filho do VarDecl
                                                                    insertBrother($2, createNode("Id", $1,line,column)); //passa para CommaId
                                                                    }  

                |       ID CommaId Type                             {$$ = createNode("VarDecl", NULL,line,column);
                                                                    insertChild($$, $3); //Type torna-se filho do VarDecl
                                                                    insertBrother($3, createNode("Id", $1,line,column)); //passa para CommaId
                                                                    insertBrother($$, $2); //passa para CommaId
                                                                    aux=$$->brother;
                                                                    loopVarSpec(aux,$3); //faz todos no final
                                                                    }  
                ;

Type:                   INT                                         {$$ = createNode("Int", NULL,line,column);}
                |       FLOAT32                                     {$$ = createNode("Float32", NULL,line,column);}
                |       BOOL                                        {$$ = createNode("Bool", NULL,line,column);}
                |       STRING                                      {$$ = createNode("String", NULL,line,column);}
                ;

FuncDeclaration:        FUNC ID LPAR RPAR FuncBody                  {$$ = createNode("FuncDecl", NULL,line,column); 
                                                                    aux = createNode("FuncHeader", NULL,line,column); 
                                                                    
                                                                    insertChild($$, aux); //FuncHeader torna-se filho do FuncDecl

                                                                    child = createNode("Id", $2,line,column);
                                                                    insertChild(aux, child); //ID torna-se filho do FuncHeader

                                                                    insertBrother(child,createNode("FuncParams", NULL,line,column)); //FuncParams torna-se filho do FuncHeader

                                                                    insertBrother(aux, $5); //FuncBody torna-se filho do FuncDecl
                                                                    } 

                |       FUNC ID LPAR Parameters RPAR FuncBody       {$$ = createNode("FuncDecl", NULL,line,column); 
                                                                    aux = createNode("FuncHeader", NULL,line,column); 
                                                                    
                                                                    insertChild($$, aux); //FuncHeader torna-se filho do FuncDecl

                                                                    child = createNode("Id", $2,line,column);
                                                                    insertChild(aux, child); //ID torna-se filho do FuncHeader
                                                                    
                                                                    insertBrother(child, $4); //Parameters torna-se filho do FuncHeader

                                                                    insertBrother(aux, $6); //FuncBody torna-se filho do FuncDecl
                                                                    }
                                                                
                |       FUNC ID LPAR RPAR Type FuncBody             {$$ = createNode("FuncDecl", NULL,line,column); 
                                                                    aux = createNode("FuncHeader", NULL,line,column);
                                                                    
                                                                    insertChild($$, aux); //FuncHeader torna-se filho do FuncDecl

                                                                    child = createNode("Id", $2,line,column);
                                                                    insertChild(aux, child); //ID torna-se filho do FuncHeader
                                                                    
                                                                    insertBrother(child, $5); //Type torna-se filho do FuncHeader
                                                                    insertBrother($5,createNode("FuncParams", NULL,line,column)); //FuncParams torna-se filho do FuncHeader

                                                                    insertBrother(aux, $6); //FuncBody torna-se filho do FuncDecl
                                                                    } 
                                                                    
                |       FUNC ID LPAR Parameters RPAR Type FuncBody  {$$ = createNode("FuncDecl", NULL,line,column); 
                                                                    aux = createNode("FuncHeader", NULL,line,column); 
                                                                    
                                                                    insertChild($$, aux); //FuncHeader torna-se filho do FuncDecl

                                                                    child = createNode("Id", $2,line,column);
                                                                    insertChild(aux, child); //ID torna-se filho do FuncHeader
                                                                    
                                                                    insertBrother(child, $6); //Type torna-se filho do FuncHeader
                                                                    insertBrother($6, $4); //Parameters torna-se filho do FuncHeader

                                                                    insertBrother(aux, $7); //FuncBody torna-se filho do FuncDecl
                                                                    }
                ;

Parameters:             ParametersAux                               {$$=createNode("FuncParams",NULL,line,column);
                                                                    insertChild($$,$1);
                                                                    } 
                ;

ParametersAux:          ID Type COMMA ParametersAux                 {$$=createNode("ParamDecl",NULL,line,column); 
                                                                    insertChild($$,$2);
                                                                    insertBrother($2,createNode("Id", $1,line,column));
                                                                    insertBrother($$,$4);
                                                                    }

                |       ID Type                                     {$$=createNode("ParamDecl",NULL,line,column); 
                                                                    insertChild($$,$2);
                                                                    insertBrother($2,createNode("Id", $1,line,column));
                                                                    }
                ;

FuncBody:               LBRACE VarsAndStatements RBRACE             {$$ = createNode("FuncBody", NULL,line,column);
                                                                    insertChild($$, $2); //VarsAndStatements torna-se filho do FuncBody
                                                                    }         
                ;
                
VarsAndStatements:      VarsAndStatements VarDeclaration SEMICOLON  {$$=$1; insertBrother($1, $2); /*recursividade*/}
                |       VarsAndStatements Statement SEMICOLON       {$$=$1; insertBrother($1, $2); /*recursividade*/}
                |       VarsAndStatements SEMICOLON                 {$$=$1;}
                |                                                   {$$=createNode(NULL,NULL,line,column);}
                ;

Statement:              ID ASSIGN Expr                              {$$ = createNode("Assign",NULL,line,column);
                                                                    aux = createNode("Id",$1,line,column);
                                                                    insertChild($$,aux);
                                                                    insertBrother(aux,$3);
                                                                    }

                |       LBRACE StatementSemicolon RBRACE            {if(validBlock($2) >= 2) {
                                                                    $$ = createNode("Block",NULL,line,column);
                                                                    insertChild($$,$2);
                                                                    } else $$ = $2;
                                                                    }

                |       IF Expr LBRACE StatementSemicolon RBRACE ELSE LBRACE StatementSemicolon RBRACE  {$$ = createNode("If",NULL,line,column);
                                                                                                        insertChild($$,$2); //Expr torna-se filho do If
                                                                                                        aux = createNode("Block",NULL,line,column);
                                                                                                        insertBrother($2,aux); //Block torna-se filho do If
                                                                                                        insertChild(aux,$4); //StatementSemicolon torna-se filho do Block
                                                                                                        aux2 = createNode("Block",NULL,line,column);
                                                                                                        insertBrother(aux,aux2); //fechar bloco
                                                                                                        insertChild(aux2,$8); //2º StatementSemicolon torna-se filho do 2º Block
                                                                                                        }

                |       IF Expr LBRACE StatementSemicolon RBRACE    {$$ = createNode("If",NULL,line,column);
                                                                    insertChild($$,$2); //Expr torna-se filho do If
                                                                    aux = createNode("Block",NULL,line,column);
                                                                    insertBrother($2,aux); //Block torna-se filho do If
                                                                    insertChild(aux,$4); //StatementSemicolon torna-se filho do Block
                                                                    insertBrother(aux,createNode("Block",NULL,line,column)); //fechar bloco
                                                                    } 

                |       FOR Expr LBRACE StatementSemicolon RBRACE   {$$ = createNode("For",NULL,line,column);
                                                                    insertChild($$,$2); //Expr torna-se filho do For
                                                                    aux = createNode("Block",NULL,line,column);
                                                                    insertBrother($2,aux); //Block torna-se filho do For
                                                                    insertChild(aux,$4); //StatementSemicolon torna-se filho do Block
                                                                    } 

                |       FOR LBRACE StatementSemicolon RBRACE        {$$ = createNode("For",NULL,line,column);
                                                                    aux = createNode("Block",NULL,line,column);
                                                                    insertChild($$,aux); //Block torna-se filho do For
                                                                    insertChild(aux,$3); //StatementSemicolon torna-se filho do Block
                                                                    } 

                |       RETURN Expr                                 {$$ = createNode("Return",NULL,line,column);
                                                                    insertChild($$,$2);
                                                                    }

                |       RETURN                                      {$$ = createNode("Return",NULL,line,column);}
                |       FuncInvocation                              {$$ = createNode("Call",NULL,line,column);
                                                                    insertChild($$,$1);
                                                                    }

                |       ParseArgs                                   {$$=$1;}

                |       PRINT LPAR STRLIT RPAR                    	{$$ = createNode("Print",NULL,line,column);
                                                                    aux = createNode("StrLit",$3,line,column);
                                                                    insertChild($$,aux);
                                                                    }

                |       PRINT LPAR Expr RPAR                        {$$ = createNode("Print",NULL,line,column); 
                                                                    insertChild($$,$3);
                                                                    }

                |       error                                       {$$=createNode(NULL,NULL,line,column);}
                ;

StatementSemicolon:     StatementSemicolon Statement SEMICOLON      {$$=$1; insertBrother($1,$2);}
                |                                                   {$$=createNode(NULL,NULL,line,column);}
                ;

ParseArgs:              ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ Expr RSQ RPAR     {$$=createNode("ParseArgs",NULL,line,column);
                                                                                            aux = createNode("Id", $1,line,column);
                                                                                            insertChild($$,aux); //ID torna-se filho do FuncBody
                                                                                            insertBrother(aux,$9); //Expr torna-se filho do FuncBody
                                                                                            }

                |       ID COMMA BLANKID ASSIGN PARSEINT LPAR error RPAR    {$$=createNode("ParseArgs",NULL,line,column);
                                                                            aux = createNode("Id", $1,line,column);
                                                                            insertChild($$,aux); //ID torna-se filho do FuncBody
                                                                            insertBrother(aux,createNode(NULL,NULL,line,column));
                                                                            }
                ;

CommaExpr:              CommaExpr COMMA Expr                        {$$=$1; insertBrother($1,$3);}
                |                                                   {$$=createNode(NULL,NULL,line,column);}
                ;

FuncInvocation:         ID LPAR Expr CommaExpr RPAR                 {$$ = createNode("Id",$1,line,column);
                                                                    insertBrother($$,$3);
                                                                    insertBrother($3,$4);
                                                                    }
                                                                    
                |       ID LPAR RPAR                                {$$ = createNode("Id",$1,line,column);}
                |       ID LPAR error RPAR                          {$$=createNode(NULL,NULL,line,column);}
                ;
                
Expr:                   Expr AND Expr                                   {$$ = createNode("And",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr OR Expr                                    {$$ = createNode("Or",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr LT Expr                                    {$$ = createNode("Lt",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr GT Expr                                    {$$ = createNode("Gt",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr EQ Expr                                    {$$ = createNode("Eq",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr NE Expr                                    {$$ = createNode("Ne",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr LE Expr                                    {$$ = createNode("Le",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr GE Expr                                    {$$ = createNode("Ge",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr PLUS Expr                                  {$$ = createNode("Add",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr MINUS Expr                                 {$$ = createNode("Sub",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr STAR Expr                                  {$$ = createNode("Mul",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr DIV Expr                                   {$$ = createNode("Div",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       Expr MOD Expr                                   {$$ = createNode("Mod",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        insertBrother($1,$3);
                                                                        }

                |       NOT Expr                                        {$$ = createNode("Not",NULL,line,column);
                                                                        insertChild($$,$2);
                                                                        }

                |       MINUS Expr %prec NOT                            {$$ = createNode("Minus",NULL,line,column);
                                                                        insertChild($$,$2);
                                                                        }

                |       PLUS Expr %prec NOT                             {$$ = createNode("Plus",NULL,line,column);
                                                                        insertChild($$,$2);
                                                                        }

                |       INTLIT                                          {$$ = createNode("IntLit",$1,line,column);}
                |       REALLIT                                         {$$ = createNode("RealLit",$1,line,column);}
                |       ID                                              {$$ = createNode("Id", $1,line,column);}

                |       FuncInvocation                                  {$$ = createNode("Call",NULL,line,column);
                                                                        insertChild($$,$1);
                                                                        }
                
                |       LPAR Expr RPAR                                  {$$ = $2;}
                |       LPAR error RPAR                                 {$$ = createNode(NULL,NULL,line,column);}
                ;

%%