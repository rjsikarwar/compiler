%{


#include <stdio.h>
int row1=1, col1=1, errcount = 0;	// To keep track of row and column number
extern int wrap();			// To show the symbol table	
extern int yyleng;			// To keep track of colomn number
extern char* yytext;			// To show erroneous pattern


%}

%token START END IF ELSE ELSIF WHILE RETURN FOR DO CONTINUE GOTO CASE
%token STRING NUM CHAR PRINTF SCANF INT FLOAT VOID MAIN
%token IVAR CVAR FUNC FLOAT_N
%token PLUS MINUS MULT DIV ASSIGN
%token LT GT LE GE EQ NE
%token COMMA SCOLON OBRAC CBRAC OCBRAC CCBRAC OLBRAC CLBRAC

%right ASSIGN
%right LT GT LE GE EQ NE
%left PLUS MINUS
%left MULT DIV

%%

start : program 	{ printf("\n reduce( start -> program) \n\n Result: %d syntax errors.\n",errcount); }

program : function SCOLON  program	
	| function SCOLON		
	| function			
	;

function : pre FUNC OBRAC varlist CBRAC fbody 	
	 | pre FUNC OBRAC CBRAC fbody       	
	 | VOID MAIN OBRAC CBRAC fbody
	 | INT MAIN OBRAC CBRAC fbody
	 ;

pre : VOID 			
    | INT 			
    | CHAR 			
    | FLOAT 			
    ;

var : INT IVAR 				
    | FLOAT IVAR
    | CHAR CVAR				
    | IVAR
    | CVAR
    | INT IVAR ASSIGN NUM SCOLON
    | FLOAT IVAR ASSIGN FLOAT_N
    | CHAR CVAR ASSIGN 
    | error 			{printf("\n\n Error at Line: %d Col: %d before the pattern: '%s'\n", row1, col1-yyleng, yytext );
    				 errcount++;
    				}
    ;

varlist : var COMMA varlist    	 	
	| var				
	;	

fbody : START stmtlist END 		
      | OCBRAC stmtlist CCBRAC

comp_stmt : OCBRAC stmtlist CCBRAC		
	  | stmt          		
	  |
	  ;
stmtlist : stmt stmtlist		
         |                              
	 ;

stmt : loop 	   			
     | condition			
     | functioncall			
     | assignment			
     | return				
     | varlist SCOLON
     | SCOLON				
     ;

loop : WHILE expr comp_stmt		
     | FOR OBRAC var ASSIGN NUM SCOLON expr SCOLON expr CBRAC has	
     ;
     
has : OCBRAC stmtlist CCBRAC
    | stmt
    ;     
condition : IF expr comp_stmt		
	  | IF expr comp_stmt elsifpart ELSE comp_stmt		
	  ;

elsifpart : ELSIF expr comp_stmt elsifpart	
	  |					
          ;

functioncall : PRINTF OBRAC STRING COMMA arglist CBRAC 		
	     | PRINTF OBRAC STRING CBRAC		   	
	     | SCANF OBRAC STRING COMMA arglist CBRAC  		
	     ;

arglist : arg COMMA arglist		
	| arg				
	;				

arg : var				
    | NUM				
    | CHAR				
    | FLOAT
    ;

assignment : IVAR ASSIGN intexpr	
	   | CVAR ASSIGN charexpr	
	   ;

return : RETURN intexpr			

expr : intexpr 				
     | charexpr               		
     ;

intexpr : intexpr relop clause			
        | clause				
     	| FUNC OBRAC arglist CBRAC         	
	| FUNC OBRAC CBRAC                 	
     	;
	
	  

clause : clause PLUS term			
     	| clause MINUS term			
     	| term					
     	;

term : term MULT factor			
     | term DIV factor			
     | factor				
     ;

factor : OBRAC intexpr CBRAC		
       | NUM				
       | IVAR				
       | FLOAT_N
       ;

charexpr : charexpr relop group         
         | group                       	
         ;

group : OBRAC charexpr CBRAC            
      | CHAR                            
      | CVAR                            
      ;

relop : LT | GT | LE | GE | EQ | NE 	

%%


extern int  yyparse();
extern FILE *yyin;
FILE *outFile_p;
main(int argc,char *argv[])
{
 if(argc<2)
  {
   printf("Please specify the input file & output file\n");
 
  }
 FILE *fp=fopen(argv[1],"r");
 if(!fp)
 {
  printf("couldn't open file for reading\n");
 
 }
  
  yyin=fp;
  yyparse();
 fclose(fp);
 
 

}

int yyerror()
{
	int i;
	errcount++;			// Increase error count
	fflush(stdout);
//	for(i=0;i<errcount;i++){
		 
		//printf("\n\n Error at Line: %d Col: %d 'before the pattern: %s'\n", row1, col1-yyleng, yytext );
//	}	
}
