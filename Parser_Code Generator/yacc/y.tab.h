
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
     FOR = 265,
     DO = 266,
     CONTINUE = 267,
     GOTO = 268,
     CASE = 269,
     STRING = 270,
     NUM = 271,
     CHAR = 272,
     PRINTF = 273,
     SCANF = 274,
     INT = 275,
     FLOAT = 276,
     VOID = 277,
     MAIN = 278,
     IVAR = 279,
     CVAR = 280,
     FUNC = 281,
     FLOAT_N = 282,
     PLUS = 283,
     MINUS = 284,
     MULT = 285,
     DIV = 286,
     ASSIGN = 287,
     LT = 288,
     GT = 289,
     LE = 290,
     GE = 291,
     EQ = 292,
     NE = 293,
     COMMA = 294,
     SCOLON = 295,
     OBRAC = 296,
     CBRAC = 297,
     OCBRAC = 298,
     CCBRAC = 299,
     OLBRAC = 300,
     CLBRAC = 301
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
#define FOR 265
#define DO 266
#define CONTINUE 267
#define GOTO 268
#define CASE 269
#define STRING 270
#define NUM 271
#define CHAR 272
#define PRINTF 273
#define SCANF 274
#define INT 275
#define FLOAT 276
#define VOID 277
#define MAIN 278
#define IVAR 279
#define CVAR 280
#define FUNC 281
#define FLOAT_N 282
#define PLUS 283
#define MINUS 284
#define MULT 285
#define DIV 286
#define ASSIGN 287
#define LT 288
#define GT 289
#define LE 290
#define GE 291
#define EQ 292
#define NE 293
#define COMMA 294
#define SCOLON 295
#define OBRAC 296
#define CBRAC 297
#define OCBRAC 298
#define CCBRAC 299
#define OLBRAC 300
#define CLBRAC 301




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


