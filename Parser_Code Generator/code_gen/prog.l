%{
/****************************************************************************************
           

****************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>			// For atoi and atof functions
#include "SymTable.h"
#include "y.tab.h"			// Data structure for symbol table
node* head = NULL;			// Pointer to symbol table
extern int row, col;			// To keep track of row and column number

/**************************************************************************************

 * The following code defines all the regular expressions for token.
 
**************************************************************************************/

%}

ALPHA	[a-zA-Z_]
DIGIT	[0-9]
WS	[ \t]
ID	({ALPHA})({ALPHA}|{DIGIT})*
IVAR	"$"{ID}
CVAR	"#"{ID}

%%

{WS}+					{ col+= strlen(yytext); 	// Neglect whitespaces
					}

start					{ col+= 5;			// Keyword start
					  return(START);
					}

end					{ col+= 3;			// Keyword end
                                          return(END);
					}
									

if					{ col+= 2;			// Keyword if
                                          return(IF);
					}

elsif					{ col+= 6;			// Keyword elseif
                                          return(ELSIF);
					}

else					{ col+= 4;			// Keyword else
                                          return(ELSE);
					}

while					{ col+= 5;			// Keyword while
                                          return(WHILE);
					}

return					{ col+= 6;			// Keyword return
                                          return(RETURN);
					}

printf                                  { col+= 6;                    // Keyword return
                                          return(PRINTF);
                                        }

scanf                                  	{ col+= 5;                    // Keyword return
                                          return(SCANF);
                                        }

\"([^\"\\]*|\\\")*\"			{				// To match string constants
					  col+= strlen(yytext);
					  install_token(yytext,row,col,"literal");
                                          strcpy(yylval.code,yytext);
					  return(STRING);
					}

{IVAR}					{ col+= strlen(yytext); 	// Integer Variables
                                          strcpy(yylval.code,yytext);					  
					  install_token(yytext,row,col,"Variable");
					  return(IVAR);
					}

{CVAR}					{ col+= strlen(yytext); 	// Character Variables
					  install_token(yytext,row,col,"char");
					  strcpy(yylval.code,yytext);
					  return(CVAR);
					}

({DIGIT})+				{ col+= strlen(yytext);		// Integer Numbers
                                          strcpy(yylval.code,yytext);
					  return(NUM);
					}

\'[a-zA-Z]\'				{ col+=3;
                                          strcpy(yylval.code,yytext);
					  return(CHAR);
					}			

{ID}					{ col+= strlen(yytext);	  	// Function Names
                                          strcpy(yylval.code,yytext);
                                          return(FUNC);
					}

=					{ col+=1; 			// Assignment Operator
					  return(ASSIGN);
					}

"*"					{ col+=1;			// Multiplication
                                          return(MULT);
					}

"+"                                     { col+=1;			// Addition
                                          return(PLUS);
					}

"-"                                     { col+=1;			// Subtraction
                                          return(MINUS);
					}

"/"                                     { col+=1;			// Division
                                          return(DIV);
					}

"<"                                     { col+=1;			// Less Than
					  strcpy(yylval.code,yytext);
                                          return(LT);
					}

">"                                     { col+=1;			// Greater Than
					  strcpy(yylval.code,yytext);
                                          return(GT);
					}

"<="                                    { col+=2;			// Less Than or Equal
					  strcpy(yylval.code,yytext);
                                          return(LE);
					}

">="                                    { col+=2;			// Greater Than or Equal
					  strcpy(yylval.code,yytext);
                                          return(GE);
					}

"=="                                    { col+=2;			// Equal
					  strcpy(yylval.code,yytext);
                                          return(EQ);
					}

"<>"                                    { col+=2;			// Not Equal
					  strcpy(yylval.code,yytext);
                                          return(NE);
					}

","					{ col+=1;			// Comma
                                          return(COMMA);
					}

";"                                     { col+=1;			// Semicolon
                                          return(SCOLON);
					}

"("					{ col+=1;			// Opening Round Bracket
					  return(OBRAC);
					}

")"                                     { col+=1;			// Closing Round Bracket
                                          return(CBRAC);					}

"{"					{ col+=1;
					  return(OCBRAC);
					 }
"}"					{col+=1;
					 return(CCBRAC);
					 }					 
					                                            

"\n"					{ row++; 
					  //printf("\n line change");
					  col=1;  
					}

.					{ printf("\n ERROR: Line:%d, Col: %d Pattern: %s\n", row, col,yytext); 
					  col+=strlen(yytext); 
					  return(* yytext);
					}

%%

int yywrap()
{
	return 1;
}

int wrap()
{
    printf("\n SYMBOL TABLE");
    showTable();
    printf("\n");	
    return 1;
}
