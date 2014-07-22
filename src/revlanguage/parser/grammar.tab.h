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
     RBNULL = 262,
     FALSE = 263,
     TRUE = 264,
     FUNCTION = 265,
     CLASS = 266,
     FOR = 267,
     IN = 268,
     IF = 269,
     ELSE = 270,
     WHILE = 271,
     NEXT = 272,
     BREAK = 273,
     RETURN = 274,
     MOD_CONST = 275,
     MOD_DYNAMIC = 276,
     MOD_STOCHASTIC = 277,
     MOD_DETERMINISTIC = 278,
     MOD_MUTABLE = 279,
     ARROW_ASSIGN = 280,
     TILDE_ASSIGN = 281,
     EQUATION_ASSIGN = 282,
     CONTROL_ASSIGN = 283,
     REFERENCE_ASSIGN = 284,
     ADDITION_ASSIGN = 285,
     SUBTRACTION_ASSIGN = 286,
     MULTIPLICATION_ASSIGN = 287,
     DIVISION_ASSIGN = 288,
     DECREMENT = 289,
     INCREMENT = 290,
     EQUAL = 291,
     AND = 292,
     OR = 293,
     AND2 = 294,
     OR2 = 295,
     GT = 296,
     GE = 297,
     LT = 298,
     LE = 299,
     EQ = 300,
     NE = 301,
     END_OF_INPUT = 302,
     UNOT = 303,
     UPLUS = 304,
     UMINUS = 305,
     UAND = 306
   };
#endif
/* Tokens.  */
#define REAL 258
#define INT 259
#define NAME 260
#define STRING 261
#define RBNULL 262
#define FALSE 263
#define TRUE 264
#define FUNCTION 265
#define CLASS 266
#define FOR 267
#define IN 268
#define IF 269
#define ELSE 270
#define WHILE 271
#define NEXT 272
#define BREAK 273
#define RETURN 274
#define MOD_CONST 275
#define MOD_DYNAMIC 276
#define MOD_STOCHASTIC 277
#define MOD_DETERMINISTIC 278
#define MOD_MUTABLE 279
#define ARROW_ASSIGN 280
#define TILDE_ASSIGN 281
#define EQUATION_ASSIGN 282
#define CONTROL_ASSIGN 283
#define REFERENCE_ASSIGN 284
#define ADDITION_ASSIGN 285
#define SUBTRACTION_ASSIGN 286
#define MULTIPLICATION_ASSIGN 287
#define DIVISION_ASSIGN 288
#define DECREMENT 289
#define INCREMENT 290
#define EQUAL 291
#define AND 292
#define OR 293
#define AND2 294
#define OR2 295
#define GT 296
#define GE 297
#define LT 298
#define LE 299
#define EQ 300
#define NE 301
#define END_OF_INPUT 302
#define UNOT 303
#define UPLUS 304
#define UMINUS 305
#define UAND 306




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 78 "./grammar.y"
{
    char*                                           c_string;
    std::string*                                    string;
    double                                          realValue;
    int                                             intValue;
    bool                                            boolValue;
    RevLanguage::SyntaxElement*                     syntaxElement;
    RevLanguage::SyntaxVariable*                    syntaxVariable;
    RevLanguage::SyntaxFunctionCall*                syntaxFunctionCall;
    RevLanguage::SyntaxLabeledExpr*                 syntaxLabeledExpr;
    RevLanguage::SyntaxFormal*                      syntaxFormal;
    std::list<RevLanguage::SyntaxElement*>*         syntaxElementList;
    std::list<RevLanguage::SyntaxLabeledExpr*>*     argumentList;
    std::list<RevLanguage::SyntaxFormal*>*          formalList;
}
/* Line 1529 of yacc.c.  */
#line 167 "./grammar.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
