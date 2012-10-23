%{
#include <stdio.h>
#include <stdlib.h>			// For atoi and atof functions
#include "SymTable.h"			// Data structure for symbol table
node* head = NULL;			// Pointer to symbol table
int row=1, col=0;			// To keep track of row and column number

%}

ALPHA	[a-zA-Z_]
DIGIT	[0-9]
WS	[ \t]
ID	({ALPHA})({ALPHA}|{DIGIT})*
INT	"$"{ID}
CHAR	"#"{ID}
COMMENT "//"*(ID/DIGIT/WS/COMMENT)
%%

{WS}+					{ col+= strlen(yytext); 	// Neglect whitespaces
					}
COMMENT				        {
					 
					}
\/\/.*[\n]        {row++;}
\/\*[\n.]*\*\/    {}



start					{ col+= 5;			// Keyword start
					  install_token(yytext,row,col,"KEYWORD");
					}

end					{ col+= 3;			// Keyword end
                                          install_token(yytext,row,col,"KEYWORD");
                                        }

if					{ col+= 2;			// Keyword if
                                          install_token(yytext,row,col,"KEYWORD");
                                        }

elseif					{ col+= 6;			// Keyword elseif
                                          install_token(yytext,row,col,"KEYWORD");
                                        }

else					{ col+= 4;			// Keyword else
                                          install_token(yytext,row,col,"KEYWORD");
                                        }

while					{ col+= 5;			// Keyword while
                                          install_token(yytext,row,col,"KEYWORD");
                                        }

for					{ col+= 3;			// Keyword for
                                          install_token(yytext,row,col,"KEYWORD");
                                        }
                                       
do 					{col+= 2;			// Keyword do
					 install_token(yytext,row,col,"KEYWORD");					
					}                                 
return					{ col+= 6;			// Keyword return
                                          install_token(yytext,row,col,"KEYWORD");
                                        }

continue				{col+= 8;
					 install_token(yytext,row,col,"KEYWORD");
					}
int 					{col+= 3;
					 install_token(yytext,row,col,"KEYWORD");
					}
char 					{col+= 4;
					 install_token(yytext,row,col,"KEYWORD");
					}
float					{col+= 5;
					 install_token(yytext,row,col,"KEYWORD");
					}
case					{col+= 4;
					 install_token(yytext,row,col,"KEYWORD");
					}				
break					{col+= 5;
					 install_token(yytext,row,col,"KEYWORD");
					}	
printf					{col+= 6;
					 install_token(yytext,row,col,"KEYWORD");
					}
scanf					{col+= 5;
					 install_token(yytext,row,col,"KEYWORD");
					}
goto					{col+= 4;
					 install_token(yytext,row,col,"KEYWORD");
					}
void					{col+= 4;
					 install_token(yytext,row,col,"KEYWORD");
					}
main					{col+= 5;
					 install_token(yytext,row,col,"KEYWORD");
					}										
\"([^\"\\]*|\\\")*\"			{				// To match string constants
					  col+= strlen(yytext);
					  install_token(yytext,row,col,"LITERAL");
					}

{INT}					{ col+= strlen(yytext); 	// Integer Variables
					  install_token(yytext,row,col,"IDENTIFIER");
					}

{CHAR}					{ col+= strlen(yytext); 	// Character Variables
					  install_token(yytext,row,col,"IDENTIFIER"); 
					}

({DIGIT})+				{ col+= strlen(yytext);		// Integer Numbers
                                         //yytext,atoi(yytext);
                                         install_token(yytext,row,col,"NUMBER"); 
					}

{DIGIT}+"."{DIGIT}+((E|e)(\+|\-)?{DIGIT}+)?	{ col+= strlen(yytext);		// Real Numbers
                                          	  //printf("NUM   , %-15s , %f\n",yytext,atof(yytext));          
                                          	  install_token(yytext,row,col,"NUMBER"); 
                                        	}			

{ID}					{ col+= strlen(yytext);	  	// Function Names
                                          install_token(yytext,row,col,"IDENTIFIER");
                                        }

"="					{ col+=1; 			// Assignment Operator
					  install_token(yytext,row,col,"RELOP"); 
					}

"*"					{ col+=1;			// Multiplication
                                          install_token(yytext,row,col,"OPERATOR");
                                        }

"+"                                     { col+=1;			// Addition
                                          install_token(yytext,row,col,"OPERATOR");
                                        }

"-"                                     { col+=1;			// Subtraction
                                          install_token(yytext,row,col,"OPERATOR");
                                        }

"/"                                     { col+=1;			// Division
                                          install_token(yytext,row,col,"OPERATOR");
                                        }

"<"                                     { col+=1;			// Less Than
                                          install_token(yytext,row,col,"RELOP");
                                        }

">"                                     { col+=1;			// Greater Than
                                          install_token(yytext,row,col,"RELOP");
                                        }

"<="                                    { col+=2;			// Less Than or Equal
                                          install_token(yytext,row,col,"RELOP");
                                        }

">="                                    { col+=2;			// Greater Than or Equal
                                          install_token(yytext,row,col,"RELOP");
                                        }

"=="                                    { col+=2;			// Equal
                                          install_token(yytext,row,col,"RELOP");
                                        }

"!="                                    { col+=2;			// Not Equal
                                          install_token(yytext,row,col,"RELOP");
                                        }

","					{ /*col+=1;			// Comma
                                          install_token(yytext,row,col);*/
                                        }

";"                                     { /*col+=1;			// Semicolon
                                          install_token(yytext,row,col);*/
                                          
                                        }

"("					{ col+=1;			// Opening Round Bracket
					  install_token(yytext,row,col,"Parenthesis");
					}

")"                                     { col+=1;			// Closing Round Bracket
                                          install_token(yytext,row,col,"Parenthesis");
                                        }

"{"                                     { col+=1;			// Opening Curly Bracket
                                          install_token(yytext,row,col,"Parenthesis");
                                        }

"}"                                     { col+=1;			// Closing Curly Bracket
                                          install_token(yytext,row,col,"Parenthesis");
                                        }

"["                                     { col+=1;			// Opening Square Bracket
                                          install_token(yytext,row,col,"Parenthesis");
                                        }

"]"                                     { col+=1;			// Closing Square Bracket
                                          install_token(yytext,row,col,"Parenthesis");
                                        }
"//"					{

					}                               

"\n"					{ row++; col=0;  }		// Newline

.					{ printf("ERROR: Line:%d, Col: %d Pattern: %s\n", row, col,yytext); 
					  //continue;
                   			  col+=strlen(yytext); 
					}

%%

main ( argc, argv )
    int argc;
    char **argv;
{
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 ) 
		yyin = fopen( argv[0], "r" );
	else
		yyin = stdin;
	yylex();
}

int yywrap()
{
    /*printf("\n SYMBOL TABLE");
    printf("\n--------------------------------------------\n");
    printf(" S.No.%20s%20s%20s\n","Token","Type","Positions");*/
    printf("----------------------------------------------");
    showTable();
    printf("\n");	
    return 1;
}
