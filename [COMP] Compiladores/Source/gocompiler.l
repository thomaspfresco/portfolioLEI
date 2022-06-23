/*Miguel Ferreira, nº 2019214567 */
/*Thomas Fresco, nº 2019219057 */

%{
	#include "y.tab.h"
	#include "functions.h"

	#include <stdio.h>
	#include <stdbool.h>
	#include <string.h>

	extern node* root;
	extern nodeTab* tableRoot;

	int yylex_destroy(void);

	int flag_l = 0;

	int line = 1;
	int column = 1;
	int flag_t = 0;

	int syntaxerror = 0;
	extern int semanticErrors;

	bool semicolon = false;
	bool printstr = false;

	int current_line = 1;
	int current_column = 1;

	char auxstr[1000];
%}

numeros	[0-9]

/*ID*/
letrasID	_|[a-z]|[A-Z]
ID {letrasID}({numeros}|{letrasID})*

/*INTLIT*/
octal	[0-7]
hexa	[a-fA-F]
INTLIT	[1-9]{numeros}*|0{octal}*|("0x"|"0X")({numeros}|{hexa})+

/*erro octal*/
INVOCT	0{octal}*[89]{numeros}*

/*REALLIT*/
expoente	(E|e)("+"|"-")?{numeros}+
REALLIT		({numeros}+"."{numeros}*{expoente}?)|("."{numeros}+{expoente}?)|{numeros}+{expoente}

/*STRLIT*/
STRLIT		\"([^\n\r\\\"]|{ESCAPE})*\" 
ESCAPE		(\\n|\\f|\\r|\\t|\t|\\\\|\\\")

/*metacaracteres*/
SEMICOLON	";"
COMMA		","
BLANKID		"_"
ASSIGN		"="
STAR		"*"
DIV			"/"
MINUS		"-"
PLUS		"+"
EQ			"=="
GE			">="
GT			">"
LBRACE		"{"
LE			"<="
LPAR		"("
LSQ			"["
LT			"<"
MOD			"%"
NE			"!="
NOT			"!"
AND			"&&"
OR			"||"
RBRACE		"}"
RPAR		")"
RSQ			"]"

/*outros*/
PACKAGE		package
RETURN		return
ELSE		else
FOR			for
IF			if
VAR			var
INT			int
FLOAT32		float32
BOOL 		bool
STRING  	string
PRINT 		fmt.Println	
PARSEINT 	strconv.Atoi
FUNC 		func
CMDARGS 	os.Args

/*newline*/
NEWLINE     \r\n|\n|\r

/*comment*/
SINGLE		"//"
BLOCK		"/*"

/*reserved*/
RESERVED	break|default|interface|select|case|defer|go|map|struct|chan|goto|switch|const|fallthrough|range|type|continue|import|"++"|"--"

/*estados*/
%x SINGLECOMMENT BLOCKCOMMENT STR

%%

{STRLIT}     {semicolon = true; if(flag_l == 1) printf("STRLIT(%s)\n",yytext); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t) return STRLIT;}

\" {BEGIN STR; current_line = line; current_column = column; column+=yyleng; semicolon = false;}
<STR>\" {semicolon=true; if(flag_l == 1) printf("STRLIT(%s)\n",yytext); column+=yyleng; BEGIN 0;}
<STR>{NEWLINE}  {BEGIN 0; printf("Line %d, column %d: unterminated string literal\n", current_line, current_column); column = 1; line++;}
<STR>{ESCAPE}|[^\\\r\n\"]   {column+=yyleng;} 
<STR>\\[^\n\r\"]|\\|\f|\t {printf("Line %d, column %d: invalid escape sequence (%s)\n", current_line, column, yytext); column+=yyleng; semicolon = false;}
<STR><<EOF>> {BEGIN 0; printf("Line %d, column %d: unterminated string literal\n", current_line, current_column);} 

{RESERVED}		{semicolon = false; if(flag_l == 1) printf("RESERVED(%s)\n", yytext); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return RESERVED;}

{SEMICOLON}		{semicolon = false; if(flag_l == 1) printf("SEMICOLON\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return SEMICOLON;}
{COMMA}	    	{semicolon = false; if(flag_l == 1) printf("COMMA\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return COMMA;}
{BLANKID}		{semicolon = false; if(flag_l == 1) printf("BLANKID\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return BLANKID;}
{ASSIGN}		{semicolon = false; if(flag_l == 1) printf("ASSIGN\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return ASSIGN;}
{STAR}		    {semicolon = false; if(flag_l == 1) printf("STAR\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return STAR;}
{DIV}			{semicolon = false; if(flag_l == 1) printf("DIV\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return DIV;}
{MINUS}		    {semicolon = false; if(flag_l == 1) printf("MINUS\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return MINUS;}
{PLUS}		    {semicolon = false; if(flag_l == 1) printf("PLUS\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return PLUS;}
{EQ}			{semicolon = false; if(flag_l == 1) printf("EQ\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return EQ;}
{GE}			{semicolon = false; if(flag_l == 1) printf("GE\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return GE;}
{GT}			{semicolon = false; if(flag_l == 1) printf("GT\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return GT;}
{LBRACE}		{semicolon = false; if(flag_l == 1) printf("LBRACE\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return LBRACE;}
{LE}			{semicolon = false; if(flag_l == 1) printf("LE\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return LE;}
{LPAR}		    {semicolon = false; if(flag_l == 1) printf("LPAR\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return LPAR;}
{LSQ}			{semicolon = false; if(flag_l == 1) printf("LSQ\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return LSQ;}
{LT}			{semicolon = false; if(flag_l == 1) printf("LT\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return LT;}
{MOD}			{semicolon = false; if(flag_l == 1) printf("MOD\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return MOD;}
{NE}			{semicolon = false; if(flag_l == 1) printf("NE\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return NE;}
{NOT}			{semicolon = false; if(flag_l == 1) printf("NOT\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return NOT;}
{AND}			{semicolon = false; if(flag_l == 1) printf("AND\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return AND;}
{OR}			{semicolon = false; if(flag_l == 1) printf("OR\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return OR;}
{RBRACE}		{semicolon = true; if(flag_l == 1) printf("RBRACE\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return RBRACE;}
{RPAR}		    {semicolon = true; if(flag_l == 1) printf("RPAR\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return RPAR;}
{RSQ}			{semicolon = true; if(flag_l == 1) printf("RSQ\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return RSQ;}

{PACKAGE}		{semicolon = false; if(flag_l == 1) printf("PACKAGE\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return PACKAGE;}
{RETURN}		{semicolon = true; if(flag_l == 1) printf("RETURN\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return RETURN;}
{ELSE}		    {semicolon = false; if(flag_l == 1) printf("ELSE\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return ELSE;}
{FOR}			{semicolon = false; if(flag_l == 1) printf("FOR\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return FOR;}
{IF}			{semicolon = false; if(flag_l == 1) printf("IF\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return IF;}
{VAR}			{semicolon = false; if(flag_l == 1) printf("VAR\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return VAR;}
{INT}			{semicolon = false; if(flag_l == 1) printf("INT\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return INT;}
{FLOAT32}		{semicolon = false; if(flag_l == 1) printf("FLOAT32\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return FLOAT32;}
{BOOL} 		    {semicolon = false; if(flag_l == 1) printf("BOOL\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return BOOL;}
{STRING}  	    {semicolon = false; if(flag_l == 1) printf("STRING\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return STRING;}
{PRINT} 	    {semicolon = false; if(flag_l == 1) printf("PRINT\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return PRINT;}
{PARSEINT} 	    {semicolon = false; if(flag_l == 1) printf("PARSEINT\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return PARSEINT;}
{FUNC} 		    {semicolon = false; if(flag_l == 1) printf("FUNC\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return FUNC;}
{CMDARGS}       {semicolon = false; if(flag_l == 1) printf("CMDARGS\n"); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return CMDARGS;}

{REALLIT}    	{semicolon = true; if(flag_l == 1) printf("REALLIT(%s)\n", yytext); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return REALLIT;}

{INVOCT}		{semicolon = false; printf("Line %d, column %d: invalid octal constant (%s)\n",line, column, yytext); column+= yyleng;}
{INTLIT}    	{semicolon = true; if(flag_l == 1) printf("INTLIT(%s)\n", yytext); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return INTLIT;}

{ID}			{semicolon = true; if(flag_l == 1) printf("ID(%s)\n", yytext); column+= yyleng; yylval.token = (char*)strdup(yytext); if(flag_t == 1) return ID;} 

{SINGLE}						{BEGIN SINGLECOMMENT;}
<SINGLECOMMENT>{NEWLINE}		{BEGIN 0; line++; column=1;}
<SINGLECOMMENT>.				{column+=yyleng;}

{BLOCK}							{current_line = line; current_column = column; BEGIN BLOCKCOMMENT; column+=yyleng;}
<BLOCKCOMMENT>{NEWLINE}			{line++; column=1;}
<BLOCKCOMMENT><<EOF>>			{printf("Line %d, column %d: unterminated comment\n", current_line, current_column); yyterminate();}
<BLOCKCOMMENT>.					{column+=yyleng;}
<BLOCKCOMMENT>"*/"				{BEGIN 0; column+=yyleng;}

{NEWLINE}		{line++; 
				column=1;
				if (semicolon == true) {
					if (flag_l == 1) {
						printf("SEMICOLON\n"); //semicolon automatico
					}
					else if (flag_t == 1) {
						semicolon = false;

						return SEMICOLON; //semicolon automatico
					}

					semicolon = false; }}

" "		{column+=yyleng;}
\t		{column+=yyleng;}

<<EOF>>			{if (semicolon == true) {
					semicolon = false;
					if (flag_l == 1) {
					printf("SEMICOLON\n"); 
					}
					
					if (flag_t == 1) {

						//yyterminate();
						return SEMICOLON;
					}
					
					}
					yyterminate();}

.				{printf("Line %d, column %d: illegal character (%s)\n",line, column, yytext); column+=yyleng;}

%%

void yyerror (char *s) {

	syntaxerror = 1;

	int aux = 0;
	
	//inicio da linha
	if (column == 1) aux = column;

	//generico
	else aux = (int)(column-yyleng);

	//strings
	//if ((yytext[0]=='\"') && (yytext[yyleng]=='\"')) aux = column - yyleng;

	printf ("Line %d, column %d: %s: %s\n", line, aux, s, yytext);
}	

int main(int argc, char* argv[]) {


if (argc == 2) {
	
	if (strcmp(argv[1], "-l") == 0) {
		flag_l = 1;
		yylex();
		//yylex_destroy();
        //return 0;
	} 
	
	else if (strcmp(argv[1], "-t") == 0) {
		flag_t = 1;
		yyparse();
		if(syntaxerror == 0) printTree(root, 0);
		//yylex_destroy();
	}

	else if (strcmp(argv[1], "-s") == 0) {
		flag_t = 1;
		yyparse();
		if (syntaxerror == 0) {
			buildTab(root);
			startAnot(root);
			if (semanticErrors == 0) {
				printTab(tableRoot);
				printf("\n");
				printTree(root, 0);
			}
		} 
		//yylex_destroy();
	}
} 

else if (argc == 1) {
	flag_t = 1;
	yyparse();
}

freeTree(root);

return 0;
}

int yywrap() {
    return 1;
}
