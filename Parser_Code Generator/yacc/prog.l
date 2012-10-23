%{
#include <stdio.h>
#include <stdlib.h>			// For atoi and atof functions
#include "symtable.h"			// Data structure for symbol table
#include "y.tab.h"
node* head = NULL;			// Pointer to symbol table
extern int row1, col1;			// To keep track of row1 and col1umn number

%}

ALPHA	[a-zA-Z_]
DIGIT	[0-9]
WS	[ \t]
ID	({ALPHA})({ALPHA}|{DIGIT})*
INT	"$"{ID}
CHAR	"#"{ID}
COMMENT "//"*(ID/DIGIT/WS/COMMENT)
%%

{WS}+					{ col1+= strlen(yytext); 	// Neglect whitespaces
					}
COMMENT				        {
					 
					}
\/\/.*[\n]        {row1++;}
\/\*[\n.]*\*\/    {}



start					{ col1+= 5;			// Keyword start
					  install_token(yytext,row1,col1,"KEYWORD");
					  return(START);
					}

end					{ col1+= 3;			// Keyword end
                                          install_token(yytext,row1,col1,"KEYWORD");
                                          return(END);
                                        }

if					{ col1+= 2;			// Keyword if
                                          install_token(yytext,row1,col1,"KEYWORD");
                                          return(IF);
                                        }

elseif					{ col1+= 6;			// Keyword elseif
                                          install_token(yytext,row1,col1,"KEYWORD");
                                          return(ELSIF);
                                        }

else					{ col1+= 4;			// Keyword else
                                          install_token(yytext,row1,col1,"KEYWORD");
                                          return(ELSE);
                                        }

while					{ col1+= 5;			// Keyword while
                                          install_token(yytext,row1,col1,"KEYWORD");
                                          return(WHILE);
                                        }

for					{ col1+= 3;			// Keyword for
                                          install_token(yytext,row1,col1,"KEYWORD");
                                          return(FOR);
                                        }
                                                                      
return					{ col1+= 6;			// Keyword return
                                          install_token(yytext,row1,col1,"KEYWORD");
                                          return(RETURN);
                                        }

int 					{col1+= 3;
					 install_token(yytext,row1,col1,"KEYWORD");
					 return(INT);
					}
char 					{col1+= 4;
					 install_token(yytext,row1,col1,"KEYWORD");
					 return(CHAR);
					}
float					{col1+= 5;
					 install_token(yytext,row1,col1,"KEYWORD");
					 return(FLOAT);
					}
case					{col1+= 4;
					 install_token(yytext,row1,col1,"KEYWORD");
					 return(CASE);
					}				
	
printf					{col1+= 6;
					 install_token(yytext,row1,col1,"KEYWORD");
					 return(PRINTF);
					}
scanf					{col1+= 5;
					 install_token(yytext,row1,col1,"KEYWORD");
					 return(SCANF);
					}
void					{col1+= 4;
					 install_token(yytext,row1,col1,"KEYWORD");
					 return(VOID);
					}
main					{col1+= 5;
					 install_token(yytext,row1,col1,"KEYWORD");
					 return(MAIN);
					}										
\"([^\"\\]*|\\\")*\"			{				// To match string constants
					  col1+= strlen(yytext);
					  install_token(yytext,row1,col1,"LITERAL");
					  return(STRING);
					}

{INT}					{ col1+= strlen(yytext); 	// Integer Variables
					  install_token(
					  yytext,row1,col1,"IDENTIFIER");
					  return(IVAR);
					}

{CHAR}					{ col1+= strlen(yytext); 	// Character Variables
					  install_token(yytext,row1,col1,"IDENTIFIER"); 
					  return(CVAR);
					}

({DIGIT})+				{ col1+= strlen(yytext);		// Integer Numbers
                                         //yytext,atoi(yytext);
                                         install_token(yytext,row1,col1,"NUMBER"); 
					 return(NUM);
					}
					

{DIGIT}+"."{DIGIT}+((E|e)(\+|\-)?{DIGIT}+)?	{ col1+= strlen(yytext);		// Real Numbers
                                          	  //printf("NUM   , %-15s , %f\n",yytext,atof(yytext));          
                                          	  install_token(yytext,row1,col1,"NUMBER"); 
                                          	  return(FLOAT_N);
                                        	}			

{ID}					{ col1+= strlen(yytext);	  	// Function Names
                                          install_token(yytext,row1,col1,"IDENTIFIER");
                                          return(FUNC);
                                        }

"="					{ col1+=1; 			// Assignment Operator
					  install_token(yytext,row1,col1,"RELOP"); 
					  return(ASSIGN);
					}

"*"					{ col1+=1;			// Multiplication
                                          install_token(yytext,row1,col1,"OPERATOR");
                                          return(MULT);
                                        }

"+"                                     { col1+=1;			// Addition
                                          install_token(yytext,row1,col1,"OPERATOR");
                                          return(PLUS);
                                        }

"-"                                     { col1+=1;			// Subtraction
                                          install_token(yytext,row1,col1,"OPERATOR");
                                          return(MINUS);
                                        }

"/"                                     { col1+=1;			// Division
                                          install_token(yytext,row1,col1,"OPERATOR");
                                          return(DIV);
                                        }

"<"                                     { col1+=1;			// Less Than
                                          install_token(yytext,row1,col1,"RELOP");
                                          return(LT);
                                        }

">"                                     { col1+=1;			// Greater Than
                                          install_token(yytext,row1,col1,"RELOP");
                                          return(GT);
                                        }

"<="                                    { col1+=2;			// Less Than or Equal
                                          install_token(yytext,row1,col1,"RELOP");
                                          return(LE);
                                        }

">="                                    { col1+=2;			// Greater Than or Equal
                                          install_token(yytext,row1,col1,"RELOP");
                                          return(GE);
                                        }

"=="                                    { col1+=2;			// Equal
                                          install_token(yytext,row1,col1,"RELOP");
                                          return(EQ);
                                        }

"!="                                    { col1+=2;			// Not Equal
                                          install_token(yytext,row1,col1,"RELOP");
                                          return(NE);
                                        }

","					{ /*col1+=1;			// Comma
                                          install_token(yytext,row1,col1);*/
                                          return(COMMA);
                                        }

";"                                     { /*col1+=1;			// Semicol1on
                                          install_token(yytext,row1,col1);*/
                                          return(SCOLON);
                                          
                                        }

"("					{ col1+=1;			// Opening Round Bracket
					  install_token(yytext,row1,col1,"Parenthesis");
					  return(OBRAC);
					}

")"                                     { col1+=1;			// Closing Round Bracket
                                          install_token(yytext,row1,col1,"Parenthesis");
                                          return(CBRAC);
                                        }

"{"                                     { col1+=1;			// Opening Curly Bracket
                                          install_token(yytext,row1,col1,"Parenthesis");
                                          return(OCBRAC);
                                        }

"}"                                     { col1+=1;			// Closing Curly Bracket
                                          install_token(yytext,row1,col1,"Parenthesis");
                                          return(CCBRAC);
                                        }

"["                                     { col1+=1;			// Opening Square Bracket
                                          install_token(yytext,row1,col1,"Parenthesis");
                                          return(OLBRAC);
                                        }

"]"                                     { col1+=1;			// Closing Square Bracket
                                          install_token(yytext,row1,col1,"Parenthesis");
                                          return(CLBRAC);
                                        }
"//"					{

					}                               

"\n"					{ row1++; col1=0;  }		// Newline

.					{ printf("ERROR: Line:%d, col1: %d Pattern: %s\n", row1, col1,yytext); 
					  //continue;
                   			  col1+=strlen(yytext); 
                   			  return( *yytext);
					}

%%

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
