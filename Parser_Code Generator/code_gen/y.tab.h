
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     START = 258,
     END = 259,
     IF = 260,
     ELSE = 261,
     ELSIF = 262,
     WHILE = 263,
     RETURN = 264,
     PRINTF = 265,
     SCANF = 266,
     STRING = 267,
     NUM = 268,
     CHAR = 269,
     IVAR = 270,
     CVAR = 271,
     FUNC = 272,
     PLUS = 273,
     MINUS = 274,
     MULT = 275,
     DIV = 276,
     ASSIGN = 277,
     LT = 278,
     GT = 279,
     LE = 280,
     GE = 281,
     EQ = 282,
     NE = 283,
     COMMA = 284,
     SCOLON = 285,
     OBRAC = 286,
     CBRAC = 287,
     CCBRAC = 288,
     OCBRAC = 289
   };
#endif
/* Tokens.  */
#define START 258
#define END 259
#define IF 260
#define ELSE 261
#define ELSIF 262
#define WHILE 263
#define RETURN 264
#define PRINTF 265
#define SCANF 266
#define STRING 267
#define NUM 268
#define CHAR 269
#define IVAR 270
#define CVAR 271
#define FUNC 272
#define PLUS 273
#define MINUS 274
#define MULT 275
#define DIV 276
#define ASSIGN 277
#define LT 278
#define GT 279
#define LE 280
#define GE 281
#define EQ 282
#define NE 283
#define COMMA 284
#define SCOLON 285
#define OBRAC 286
#define CBRAC 287
#define CCBRAC 288
#define OCBRAC 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 36 "prog.y"

      	int number;
	char code[1024];
      	struct attribute4 *attr4Ptr;
	struct attribute2 *attr2Ptr;



/* Line 1676 of yacc.c  */
#line 129 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


