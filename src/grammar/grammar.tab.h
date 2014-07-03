/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
     ARROW_ASSIGN = 275,
     TILDE_ASSIGN = 276,
     EQUATION_ASSIGN = 277,
     ADDITION_ASSIGN = 278,
     SUBTRACTION_ASSIGN = 279,
     MULTIPLICATION_ASSIGN = 280,
     DIVISION_ASSIGN = 281,
     DECREMENT = 282,
     INCREMENT = 283,
     EQUAL = 284,
     AND = 285,
     OR = 286,
     AND2 = 287,
     OR2 = 288,
     GT = 289,
     GE = 290,
     LT = 291,
     LE = 292,
     EQ = 293,
     NE = 294,
     END_OF_INPUT = 295,
     UNOT = 296,
     UPLUS = 297,
     UMINUS = 298,
     UAND = 299
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
#define ARROW_ASSIGN 275
#define TILDE_ASSIGN 276
#define EQUATION_ASSIGN 277
#define ADDITION_ASSIGN 278
#define SUBTRACTION_ASSIGN 279
#define MULTIPLICATION_ASSIGN 280
#define DIVISION_ASSIGN 281
#define DECREMENT 282
#define INCREMENT 283
#define EQUAL 284
#define AND 285
#define OR 286
#define AND2 287
#define OR2 288
#define GT 289
#define GE 290
#define LT 291
#define LE 292
#define EQ 293
#define NE 294
#define END_OF_INPUT 295
#define UNOT 296
#define UPLUS 297
#define UMINUS 298
#define UAND 299




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 79 "./grammar.y"

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



/* Line 2068 of yacc.c  */
#line 156 "./grammar.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
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

