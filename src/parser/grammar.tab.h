/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     REAL = 258,
     INT = 259,
     NAME = 260,
     STRING = 261,
     NULL = 262,
     FALSE = 263,
     TRUE = 264,
     COMMENT = 265,
     FUNCTION = 266,
     FOR = 267,
     IN = 268,
     IF = 269,
     ELSE = 270,
     WHILE = 271,
     NEXT = 272,
     BREAK = 273,
     LEFT_ASSIGN = 274,
     TILDE_ASSIGN = 275,
     EQUAL_ASSIGN = 276,
     EQUAL = 277,
     AND = 278,
     OR = 279,
     AND2 = 280,
     OR2 = 281,
     GT = 282,
     GE = 283,
     LT = 284,
     LE = 285,
     EQ = 286,
     NE = 287,
     END_OF_INPUT = 288,
     LOW = 289,
     ARROW_ASSIGN = 290,
     UNOT = 291,
     UPLUS = 292,
     UMINUS = 293
   };
#endif
/* Tokens.  */
#define REAL 258
#define INT 259
#define NAME 260
#define STRING 261
#define NULL 262
#define FALSE 263
#define TRUE 264
#define COMMENT 265
#define FUNCTION 266
#define FOR 267
#define IN 268
#define IF 269
#define ELSE 270
#define WHILE 271
#define NEXT 272
#define BREAK 273
#define LEFT_ASSIGN 274
#define TILDE_ASSIGN 275
#define EQUAL_ASSIGN 276
#define EQUAL 277
#define AND 278
#define OR 279
#define AND2 280
#define OR2 281
#define GT 282
#define GE 283
#define LT 284
#define LE 285
#define EQ 286
#define NE 287
#define END_OF_INPUT 288
#define LOW 289
#define ARROW_ASSIGN 290
#define UNOT 291
#define UPLUS 292
#define UMINUS 293




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 76 "grammar.y"
{
    char*                  string;
    double                 realValue;
    int                    intValue;
    bool                   boolValue;
    SyntaxElement*         syntaxElement;
    list<SyntaxElement *>* syntaxElementList;
}
/* Line 1529 of yacc.c.  */
#line 134 "grammar.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

