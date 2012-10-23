%{
/****************************************************************************************

             

****************************************************************************************/

#include <stdio.h>			
#include <string.h>			

int row=1, col=1, errcount = 0;		
extern int wrap();			// To show the symbol table	
extern int yyleng;			// To keep track of colomn number
extern char* yytext;			// To show erroneous pattern

int g_temp = 1, g_label = 1;		
int is_main = 0;			

struct attribute4			// Structure for attributes of statements
{
	char code[1024];		// Output code
	char place[8];			// Name that holds the value
	char begin[8];			// Begin marker
	char end[8];			// End marker
};

struct attribute2			// Structure for attributes of expressions
{
	char code[1024];		// Output Code
	char place[8];			// Name that holds the value
};
char* newlabel(char* newlabelvar);
struct attribute2* makeAttr1(struct attribute2* op1, struct attribute2* op2, char* str, int type);
struct attribute2* makeAttr2(char* str);
struct attribute4* makeAttr3(struct attribute2* op1,char* str, int type);


%}

%union
{
      	int number;
	char code[1024];
      	struct attribute4 *attr4Ptr;
	struct attribute2 *attr2Ptr;
};

%token START END IF ELSE ELSIF WHILE RETURN PRINTF SCANF
%token <code> STRING NUM CHAR
%token <code> IVAR CVAR FUNC
%token <code> PLUS MINUS MULT DIV ASSIGN
%token <code> LT GT LE GE EQ NE
%token COMMA SCOLON OBRAC CBRAC OCBRAC CCBRAC

%right ASSIGN
%right LT GT LE GE EQ NE
%left PLUS MINUS
%left MULT DIV

%type <code> start program function fbody comp_stmt stmtlist 
%type <attr4Ptr> stmt loop condition functioncall assignment elsifpart return
%type <attr2Ptr> expr intexpr charexpr clause term factor group 
%type <code> var varlist arg arglist relop

%%

start : program 	{ printf("\n Result: 0 syntax errors. Input Accepted.\n");
			  strcpy($$,"");
			  if(is_main)
				sprintf($$,"goto main:\n");
			  strcat($$,$1);
			  printf("\n****3 Address Code*****\n%s",$$);	
			}

program : function SCOLON  program	{ char newcode[1024] = "";
					  strcpy(newcode,$1);
					  strcat(newcode,$3);
					  strcpy($$,newcode);
					}
	| function SCOLON		{ strcpy($$,$1); }
	| function			{ strcpy($$,$1); }
	;

function : FUNC OBRAC varlist CBRAC fbody { char newcode[1024]="";
					    sprintf(newcode,"\n%s:\n%s\n",$1,$3);
				            strcpy($$,newcode);
					    strcat($$,$5);
					    sprintf(newcode,"return (0)\n");
                                  	    strcat($$,newcode);		  
					    if(!strcmp($1,"main"))
						is_main = 1;  
					  }
	 |  FUNC OBRAC CBRAC fbody     	{ char newcode[1024]="";
                                          sprintf(newcode,"\n%s: \n",$1);
                                          strcpy($$,newcode);
                                          strcat($$,$4);
                                          sprintf(newcode,"return (0)\n");
                                          strcat($$,newcode);
					  if(!strcmp($1,"main"))
                                                is_main = 1;
					}
	 ;

var : IVAR 				{ strcpy($$,$1); }
    | CVAR				{ strcpy($$,$1); }
    ;

varlist : var COMMA varlist    	 	{ char newcode[1024]="";
					  sprintf(newcode,"ACTUAL %s\n%s",$1,$3);  
					  strcpy($$,newcode);
					}
	| var				{ char newcode[1024]="";
                                          sprintf(newcode,"ACTUAL %s",$1);
                                          strcpy($$,newcode);
                                        }
	;	

fbody : START stmtlist END 		{ strcpy($$,$2); }
      | OCBRAC stmtlist CCBRAC		{ strcpy($$,$2);}

comp_stmt : START stmtlist END		{ strcpy($$,$2); }
	  | OCBRAC stmtlist CCBRAC	{strcpy($$,$2);}
	  | START END          		{ strcpy($$,""); }

stmtlist : stmt stmtlist		{ struct attribute4 *q = $1;
					  char newcode[1024]="";
                                          strcpy(newcode, q->code);
                                          strcat(newcode, $2);
                                          strcpy($$,newcode);
					}
	 |				{ strcpy($$,""); }
	 ;

stmt : loop				{ $$ = $1; } 
	| condition 			{ $$ = $1; }
	| functioncall 			{ $$ = $1; }
	| assignment 			{ $$ = $1; }
	| return			{ $$ = $1; }	
     	| SCOLON			{ struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
                                          strcpy(p->begin,"");
                                          strcpy(p->end,"");
                                          strcpy(p->place,"");
                                          strcpy(p->code,"");
                                          $$ = p;
					}
	| error				{printf("\n\n Error at Line: %d Col: %d (somewhere before the pattern: %s)\n", row, col-yyleng, yytext );}
	;	

loop : WHILE expr comp_stmt		{ struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
					  struct attribute2 *q = $2;
					  char tac_newlabel[8] = "_l";
					  strcpy(p->begin,newlabel(tac_newlabel));
					  strcpy(tac_newlabel,"_l");
					  strcpy(p->end,newlabel(tac_newlabel));
					  strcpy(p->place,"");
					  char newcode[1024]="";
					  sprintf(newcode,"%s: \n%s", p->begin, q->code);
					  strcpy(p->code,newcode);
					  sprintf(newcode,"if %s == 0 goto %s\n",q->place,p->end);
					  strcat(p->code,newcode);
					  strcat(p->code,$3);
					  sprintf(newcode,"goto %s \n%s:\n",p->begin,p->end);
                                          strcat(p->code,newcode);
					  $$ = p;
					}

condition : IF expr comp_stmt		{ struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
                                          struct attribute2 *q = $2;
             				  char tac_newlabel[8] = "_l";
                                          strcpy(p->end,newlabel(tac_newlabel));
					  strcpy(p->place,"");
                                          char newcode[1024]="";
					  strcpy(p->code,q->code);
					  sprintf(newcode,"if %s == 0 goto %s\n",q->place,p->end);
					  strcat(p->code,newcode);
                                          strcat(p->code,$3);
					  sprintf(newcode,"%s:\n",p->end);
                                          strcat(p->code,newcode);
					  $$ = p;
					}
	  | IF expr comp_stmt elsifpart ELSE comp_stmt		
					{ struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
                                          struct attribute2 *q = $2;
                                          struct attribute4 *s = $4;
					  char tac_newlabel[8] = "_l";
                                          strcpy(p->end,newlabel(tac_newlabel));
					  strcpy(tac_newlabel,"_l");
                                          strcpy(p->place,"");
                                          char newcode[1024]="";
                                          strcpy(p->code,q->code);
                                          sprintf(newcode,"if %s == 0 goto %s\n",q->place,p->end);
                                          strcat(p->code,newcode);
                                          strcat(p->code,$3);
					  sprintf(newcode,"goto %s\n",s->begin);
                                          strcat(p->code,newcode);
                                          sprintf(newcode,"%s:\n",p->end);
                                          strcat(p->code,newcode);
					  strcat(p->code,s->code);
					  strcat(p->code,$6);
					  sprintf(newcode,"%s:\n",s->begin);
					  strcat(p->code,newcode);
                                          $$ = p;
					}
	  ;

elsifpart : ELSIF expr comp_stmt elsifpart	
					{ struct attribute2 *q = $2;
                                          struct attribute4 *p = $4;
                                          char tac_newlabel[8] = "_l";
                                          strcpy(p->end,newlabel(tac_newlabel));
                                          strcpy(p->place,"");
                                          char newcode1[1024]="";
					  char newcode2[1024]="";
                                          strcpy(newcode1,q->code);
                                          sprintf(newcode2,"if %s == 0 goto %s\n",q->place,p->end);
                                          strcat(newcode1,newcode2);
                                          strcat(newcode1,$3);
					  sprintf(newcode2,"goto %s\n",p->begin);
                                          strcat(newcode1,newcode2);
                                          sprintf(newcode2,"%s:\n",p->end);
                                          strcat(newcode1,newcode2);
					  strcat(newcode1,p->code);
					  strcpy(p->code,newcode1);
                                          $$ = p;
					}
	  |				{ struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
					  strcpy(p->code,"");
					  strcpy(p->place,"");
					  char tac_newlabel[8] = "_l";
					  strcpy(p->begin,newlabel(tac_newlabel));
					  strcpy(p->end,"");
					  $$ = p;
					}
          ;

functioncall : PRINTF OBRAC STRING COMMA arglist CBRAC { struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
					     char newcode[1024]="";
                                             sprintf(newcode,"PARAM %s\n",$3);
					     strcpy(p->code,newcode);
                                             strcat(p->code,$5);
                                             sprintf(newcode,"CALL printf\n");
                                             strcat(p->code,newcode);
                                             strcpy(p->begin,"");
                                             strcpy(p->end,"");
                                             strcpy(p->place,"");
                                             $$ = p;		
					   }
 | PRINTF OBRAC STRING CBRAC		   { struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
                                             char newcode[1024]="";
                                             sprintf(newcode,"PARAM %s\n",$3);
                                             strcpy(p->code,newcode);
                                             sprintf(newcode,"CALL printf\n");
                                             strcat(p->code,newcode);
                                             strcpy(p->begin,"");
                                             strcpy(p->end,"");
                                             strcpy(p->place,"");
                                             $$ = p;
					   }
 | SCANF OBRAC STRING COMMA arglist CBRAC  { struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
                                             char newcode[1024]="";
                                             sprintf(newcode,"PARAM %s\n",$3);
                                             strcpy(p->code,newcode);
                                             strcat(p->code,$5);
                                             sprintf(newcode,"CALL scanf\n");
                                             strcat(p->code,newcode);
                                             strcpy(p->begin,"");
                                             strcpy(p->end,"");
                                             strcpy(p->place,"");
                                             $$ = p;
					   }
 ;

arglist : arg COMMA arglist		{ char newcode[1024]="";
                                          sprintf(newcode,"PARAM %s\n",$1);
                                          strcat(newcode, $3);
                                          strcpy($$,newcode);
					}
	| arg				{ char newcode[1024]="";
                                          sprintf(newcode,"PARAM %s\n",$1);
                                          strcpy($$,newcode);
					}
	;

arg : var                      		{ strcpy($$,$1); } 
    | NUM                       	{ strcpy($$,$1); }
    | CHAR				{ strcpy($$,$1); }
    ;

assignment : CVAR ASSIGN charexpr	{ $$ = makeAttr3($3,$1,2); }
	   | IVAR ASSIGN intexpr	{ $$ = makeAttr3($3,$1,1); }
	   ;

return : RETURN intexpr			{ struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
				          struct attribute2 *q = $2;
        				  char newcode[32];
        				  strcpy(p->place,"");
        				  strcpy(p->code,q->code);
        				  sprintf(newcode,"return (%s)\n", q->place);
        				  strcat(p->code,newcode);
        				  strcpy(p->begin,"");
          				  strcpy(p->end,"");
					  $$ = p;
					}
	;

expr : intexpr | charexpr 		{ $$ = $1; }
     ;

intexpr : intexpr relop clause		{ $$ = makeAttr1($1,$3,$2,1); }
     	| clause			{ $$ = $1; }
	| FUNC OBRAC arglist CBRAC      { struct attribute2 *p = (struct attribute2 *)malloc(sizeof(struct attribute2));
                                          strcpy(p->code,$3);
                                          char newcode[1024]="";
					  //char tac_newtemp[8] = "$_t";
					  //strcpy(p->place,newtemp(tac_newtemp));
                                          strcpy(p->place,$1);
					  sprintf(newcode,"CALL %s\n",$1,p->place);
                                          strcat(p->code,newcode);
                                          $$ = p;
                                        }
 	| FUNC OBRAC CBRAC         	{ struct attribute2 *p = (struct attribute2 *)malloc(sizeof(struct attribute2));
                                          char newcode[1024]="";
					  //char tac_newtemp[8] = "$_t";
                                          //strcpy(p->place,newtemp(tac_newtemp));
					  strcpy(p->place,$1);
					  sprintf(newcode,"CALL %s\n",$1,p->place);
                                          strcpy(p->code,newcode);
					  $$ = p;
                                        }
     	;

clause : clause PLUS term		{ $$ = makeAttr1($1,$3,"+",1); }
     	| clause MINUS term		{ $$ = makeAttr1($1,$3,"-",1); }
     	| term				{ $$ = $1; }
     	;

term : term MULT factor			{ $$ = makeAttr1($1,$3,"*",1); }
     | term DIV factor			{ $$ = makeAttr1($1,$3,"/",1); }
     | factor				{ $$ = $1; }
     ;

factor : OBRAC intexpr CBRAC		{ $$ = $2; }
       | NUM 				{ $$ = makeAttr2($1); }
       | IVAR				{ $$ = makeAttr2($1); }
       ;

charexpr : charexpr relop group		{ $$ = makeAttr1($1,$3,$2,2); } 
	 | group			{ $$ = $1; }
	 ;

group : OBRAC charexpr CBRAC		{ $$ = $2; }
      | CHAR 				{ $$ = makeAttr2($1); }
      | CVAR				{ $$ = makeAttr2($1); }
      ;

relop : LT				{ strcpy($$,$1); } 
	| GT 				{ strcpy($$,$1); }
	| LE 				{ strcpy($$,$1); }	
	| GE 				{ strcpy($$,$1); }
	| EQ 				{ strcpy($$,$1); }
	| NE 				{ strcpy($$,$1); }

%%

main (int argc, char *argv[])
{
	FILE *fp;
	fp = fopen(argv[1],"r");
   	yyparse();			// Start parsing
	if(errcount)			// If error occured
                printf("\n Result: %d syntax errors. Input rejected \n",errcount);
   	wrap();				// Print symbol table
	printf("\n");
	fclose(fp);	
}

int yyerror()
{
	errcount++;			// Increase error count
	printf("\n\n Error at Line: %d Col: %d (somewhere before the pattern: %s)\n", row, col-yyleng, yytext );
}

/***********************************************************************************/

char* newtemp(char* newtempvar)		// Function to return a new temporary variable
{					// Returns variables of the form _t1, _t2, .... 
	char tempnum[5];				
	sprintf (tempnum, "%d", g_temp++);
	strcat(newtempvar,tempnum);
	install_token(newtempvar);	// Insert it in the symbol table
	return(newtempvar);		// Return the variable
}

char* newlabel(char* newlabelvar)	// Function to return a new label
{					//  Returns labels of the form _l1, _l2, ....
        char labelnum[5];
        sprintf (labelnum, "%d", g_label++);
        strcat(newlabelvar,labelnum);
        return(newlabelvar);		// Return the Label
}


/* Following function constructs attributes of an expression from attributes 
   of two sub-expressions and returns a ptr to it.
*/
struct attribute2* makeAttr1(struct attribute2* op1, struct attribute2* op2, char* str, int type)
{
	struct attribute2 *p = (struct attribute2 *)malloc(sizeof(struct attribute2));
	char tac_newtemp[8];
	if(type == 1)					// Return $_t.... for integer expr
        	strcpy(tac_newtemp,"$_t");
	else 						// Return #_t.... for character expr
		strcpy(tac_newtemp,"#_t");	
        char newcode[32];
        strcpy(p->place,newtemp(tac_newtemp));		// place = new temp variable
        strcpy(p->code,op1->code);			// code = op1.code || op2.code
        strcat(p->code,op2->code);
        sprintf(newcode,"%s := %s %s %s\n", p->place, op1->place, str, op2->place);
        strcat(p->code,newcode);			// code = code || op1.place || operator || op2.place
	return p;
}

/* Following function constructs attributes of an expression from a string */
struct attribute2* makeAttr2(char* str)
{
	struct attribute2 *p = (struct attribute2 *)malloc(sizeof(struct attribute2));
        strcpy(p->place,str);				// place = string
        strcpy(p->code,"");				// code = NULL
        return p;
}
/* Following function constructs attributes of statements from attributes of sub-statements */
struct attribute4* makeAttr3(struct attribute2* op1,char* str, int type)
{
	struct attribute4 *p = (struct attribute4 *)malloc(sizeof(struct attribute4));
        char tac_newtemp[8];
        if(type == 1)
                strcpy(tac_newtemp,"$_t");		// Return $_t.... for integer expr
        else
                strcpy(tac_newtemp,"#_t");		// Return #_t.... for character expr
        char newcode[32];				
        strcpy(p->place,str);				// place = string
        strcpy(p->code,op1->code);			// code = op1.code
        sprintf(newcode,"%s := %s\n",p->place,op1->place);
        strcat(p->code,newcode);			// code = code || newtemp || ':=' || op1.place
	strcpy(p->begin,"");				// begin = NULL
	strcpy(p->end,"");				// end = NUL
        return p;
}
