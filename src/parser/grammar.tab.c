/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "grammar.y"

/*
 * \file
 * Grammar specification in bison format for REvBayes, an R-like environment 
 * for Evolutionary analysis using Bayesian inference. REvBayes uses EvoMoDeL,
 * Evolutionary Model Description Language, to describe evolutionary models
 * used in Bayesian MCMC inference, so the EvoMoDeL grammar is a subset of the
 * REvBayes grammar.
 *
 * The grammar borrows heavily from the R grammar specification in the gram.y 
 * file of the R source code. EvoMoDeL itself is inspired by the language used  
 * by BUGS to describe models. Unlike BUGS, and similar programs such as JAGS,  
 * REvBayes allows models to be built in an interpreted environment.
 *
 * \brief Grammar specification in bison format
 *
 * \author Fredrik Ronquist
 * \date Last modified: $Date$
 *
 * $Id$
 */

/* The following statements go into the resulting C code */
#include <iostream>
#include <list>
#include <string>

#include "config.h"

#include "RbDataType.h"
#include "RbObject.h"
#include "Parser.h"
#include "SyntaxElement.h"

extern int yyerror(char *);
extern int yylex(void);
extern char *yytext;
extern Parser parser;

#ifdef YYDEBUG
#warning "Compiled with parser debug option"
#define YYERROR_VERBOSE
#endif

#ifdef YACC_VERBOSE
#define PRINTF printf
#else
#define PRINTF(...)
#endif

#define YY_NEVER_INTERACTIVE

/*!
 * \todo Add functionality for locality determination
 *       in parser. The code below is just a stub.
*/

#if def 0
/* struct holding source location for accurate error reports */
typedef struct yyltype
{
    int first_line;
    int first_column;
    int first_byte;

    int last_line;
    int last_column;
    int last_byte;
}

#define YYLTYPE yyltype
#endif



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 77 "grammar.y"
{
    char                  *string;
    double                 realValue;
    int                    intValue;
    bool                   boolValue;
    RbObject              *object;
    SyntaxElement         *syntaxElement;
    list<SyntaxElement *> *syntaxElementList;
}
/* Line 193 of yacc.c.  */
#line 257 "grammar.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 270 "grammar.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  52
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   465

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNRULES -- Number of states.  */
#define YYNSTATES  157

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      49,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    51,     2,     2,    46,     2,     2,     2,
      47,    50,    40,    38,    53,    39,     2,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    42,    56,
       2,     2,     2,    34,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    48,     2,    52,    45,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    54,     2,    55,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    35,
      36,    37,    43,    44
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    16,    18,    20,
      24,    27,    30,    33,    36,    40,    44,    48,    52,    56,
      60,    64,    68,    72,    76,    80,    84,    88,    92,    96,
     100,   102,   104,   106,   108,   110,   112,   116,   120,   124,
     128,   132,   135,   136,   140,   141,   143,   147,   148,   150,
     155,   158,   159,   163,   164,   166,   170,   176,   179,   180,
     184,   185,   187,   191,   195,   197,   200,   201,   205,   209,
     210,   212,   214,   216,   218,   220,   223,   226,   228,   232,
     238,   242,   246,   252,   256,   258,   260,   262,   264,   266,
     268,   270,   272
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      58,     0,    -1,    33,    -1,    49,    -1,    10,    49,    -1,
      79,    49,    -1,    59,    49,    -1,     1,    -1,    88,    -1,
      47,    59,    50,    -1,    39,    59,    -1,    38,    59,    -1,
      51,    59,    -1,    34,    87,    -1,    59,    42,    59,    -1,
      59,    38,    59,    -1,    59,    39,    59,    -1,    59,    40,
      59,    -1,    59,    41,    59,    -1,    59,    45,    59,    -1,
      59,    29,    59,    -1,    59,    30,    59,    -1,    59,    31,
      59,    -1,    59,    32,    59,    -1,    59,    28,    59,    -1,
      59,    27,    59,    -1,    59,    23,    59,    -1,    59,    24,
      59,    -1,    59,    25,    59,    -1,    59,    26,    59,    -1,
      60,    -1,    62,    -1,    61,    -1,    72,    -1,    68,    -1,
      63,    -1,    63,    36,    59,    -1,    68,    36,    59,    -1,
      63,    20,    68,    -1,    63,    21,    59,    -1,    87,    64,
      65,    -1,    46,    87,    -1,    -1,    48,    66,    52,    -1,
      -1,    67,    -1,    66,    53,    67,    -1,    -1,    59,    -1,
      87,    47,    69,    50,    -1,    71,    70,    -1,    -1,    53,
      71,    70,    -1,    -1,    59,    -1,    87,    22,    59,    -1,
      11,    47,    73,    50,    76,    -1,    75,    74,    -1,    -1,
      53,    75,    74,    -1,    -1,    87,    -1,    87,    22,    59,
      -1,    54,    77,    55,    -1,    79,    -1,    79,    78,    -1,
      -1,    56,    79,    78,    -1,    49,    79,    78,    -1,    -1,
      80,    -1,    82,    -1,    84,    -1,    85,    -1,    86,    -1,
      59,    49,    -1,    59,    56,    -1,    56,    -1,    14,    81,
      76,    -1,    14,    81,    76,    15,    76,    -1,    47,    59,
      50,    -1,    12,    83,    76,    -1,    47,    87,    13,    59,
      50,    -1,    16,    81,    76,    -1,    17,    -1,    18,    -1,
       5,    -1,     8,    -1,     9,    -1,     7,    -1,     4,    -1,
       6,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   159,   159,   160,   161,   162,   163,   164,   167,   169,
     171,   172,   173,   174,   176,   178,   179,   180,   181,   182,
     184,   185,   186,   187,   188,   189,   191,   192,   193,   194,
     196,   197,   198,   200,   201,   203,   206,   211,   218,   225,
     232,   239,   240,   243,   244,   247,   248,   251,   252,   255,
     262,   263,   266,   267,   270,   275,   282,   289,   290,   293,
     294,   297,   302,   309,   310,   313,   314,   317,   318,   319,
     322,   323,   324,   325,   326,   327,   328,   329,   332,   333,
     335,   338,   341,   344,   347,   350,   353,   356,   357,   358,
     359,   360,   361
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "REAL", "INT", "NAME", "STRING", "NULL",
  "FALSE", "TRUE", "COMMENT", "FUNCTION", "FOR", "IN", "IF", "ELSE",
  "WHILE", "NEXT", "BREAK", "LEFT_ASSIGN", "TILDE_ASSIGN", "EQUAL_ASSIGN",
  "EQUAL", "AND", "OR", "AND2", "OR2", "GT", "GE", "LT", "LE", "EQ", "NE",
  "END_OF_INPUT", "'?'", "LOW", "ARROW_ASSIGN", "UNOT", "'+'", "'-'",
  "'*'", "'/'", "':'", "UPLUS", "UMINUS", "'^'", "'$'", "'('", "'['",
  "'\\n'", "')'", "'!'", "']'", "','", "'{'", "'}'", "';'", "$accept",
  "prog", "expression", "arrowAssign", "tildeAssign", "equalAssign",
  "variable", "optAttribute", "optElement", "subList", "sub",
  "functionCall", "argumentList", "optArguments", "argument",
  "functionDef", "formalList", "optFormals", "formal", "stmts", "stmtList",
  "optStatements", "statement", "ifStatement", "cond", "forStatement",
  "forCond", "whileStatement", "nextStatement", "breakStatement",
  "identifier", "constant", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,    63,   289,   290,   291,    43,    45,
      42,    47,    58,   292,   293,    94,    36,    40,    91,    10,
      41,    33,    93,    44,   123,   125,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    57,    58,    58,    58,    58,    58,    58,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    60,    60,    61,    62,
      63,    64,    64,    65,    65,    66,    66,    67,    67,    68,
      69,    69,    70,    70,    71,    71,    72,    73,    73,    74,
      74,    75,    75,    76,    76,    77,    77,    78,    78,    78,
      79,    79,    79,    79,    79,    79,    79,    79,    80,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    88,    88,
      88,    88,    88
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     1,     1,     3,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     2,     0,     3,     0,     1,     3,     0,     1,     4,
       2,     0,     3,     0,     1,     3,     5,     2,     0,     3,
       0,     1,     3,     3,     1,     2,     0,     3,     3,     0,
       1,     1,     1,     1,     1,     2,     2,     1,     3,     5,
       3,     3,     5,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     7,    92,    90,    86,    91,    89,    87,    88,     0,
       0,     0,     0,     0,    84,    85,     2,     0,     0,     0,
       0,     3,     0,    77,     0,     0,    30,    32,    31,    35,
      34,    33,     0,    70,    71,    72,    73,    74,    42,     8,
       4,    58,     0,     0,     0,     0,     0,    13,    11,    10,
       0,    12,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     6,
      76,     0,     0,     0,     0,     5,     0,    51,    44,     0,
      60,    61,     0,    66,     0,    81,    64,     0,    78,    83,
       9,    26,    27,    28,    29,    25,    24,    20,    21,    22,
      23,    15,    16,    17,    18,    14,    19,    38,     0,    39,
      36,    37,    41,    54,     0,    53,    42,    47,    40,     0,
       0,    57,     0,     0,     0,    69,    75,    80,     0,    49,
       0,    50,     0,    48,     0,    45,    56,    60,    62,     0,
      63,     0,     0,    65,    79,    53,    55,    43,    47,    59,
      82,    69,    69,    52,    46,    68,    67
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    24,    84,    26,    27,    28,    29,    78,   118,   134,
     135,    30,   114,   131,   115,    31,    79,   121,    80,    85,
     124,   143,    86,    33,    45,    34,    43,    35,    36,    37,
      38,    39
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -110
static const yytype_int16 yypact[] =
{
     146,  -110,  -110,  -110,  -110,  -110,  -110,  -110,  -110,   -38,
     -32,   -26,     3,     3,  -110,  -110,  -110,    46,    86,    86,
      86,  -110,    86,  -110,    69,   253,  -110,  -110,  -110,   -13,
      35,  -110,    30,  -110,  -110,  -110,  -110,  -110,   -22,  -110,
    -110,    46,    46,   200,    86,   200,   200,  -110,    36,    36,
     321,   401,  -110,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    31,
    -110,    46,    86,    86,    86,  -110,    46,    86,    34,    33,
      32,    64,    75,   219,   287,  -110,  -110,   349,    84,  -110,
    -110,   401,    73,   401,    73,   420,   420,   420,   420,   420,
     420,   -28,   -28,   -36,   -36,    36,    36,  -110,    59,     7,
       7,     7,  -110,     7,    57,    55,   -20,    86,  -110,   200,
      46,  -110,    86,    86,    54,   -46,  -110,  -110,   200,  -110,
      86,  -110,    86,     7,   -24,  -110,  -110,    32,     7,   377,
    -110,   219,   219,  -110,  -110,    55,     7,  -110,    86,  -110,
    -110,   -46,   -46,  -110,  -110,  -110,  -110
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -110,  -110,     0,  -110,  -110,  -110,  -110,  -110,  -110,  -110,
     -27,    39,  -110,   -29,    -4,  -110,  -110,   -10,     8,   -41,
    -110,  -109,     1,  -110,   118,  -110,  -110,  -110,  -110,  -110,
      -1,  -110
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -76
static const yytype_int16 yytable[] =
{
      25,    32,   132,   141,    88,    89,    67,    71,    72,    68,
     142,    40,    65,    66,    67,    41,    47,    68,    48,    49,
      50,    42,    51,    73,    76,    77,    76,    77,   147,   148,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      81,    82,   155,   156,    87,    63,    64,    65,    66,    67,
      44,     4,    68,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,    52,
     108,    74,   109,   110,   111,   112,   116,   113,   136,    75,
     -75,    68,   117,   119,   125,   120,   122,   144,   123,     2,
       3,     4,     5,     6,     7,     8,    53,    10,    55,   128,
      57,    58,    59,    60,    61,    62,    77,   129,   130,   140,
     107,    63,    64,    65,    66,    67,   153,   133,    68,    81,
      17,   154,   138,   139,    18,    19,   145,   149,   137,   116,
     113,    46,   146,    20,     0,     0,     0,    22,     0,     0,
       0,     0,   151,   152,     0,     0,     0,     1,   133,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,     0,
      12,     0,    13,    14,    15,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
      17,     0,     0,     0,    18,    19,     0,     0,     0,     0,
       0,     0,     0,    20,     0,    21,     0,    22,     0,     0,
       0,     0,    23,     2,     3,     4,     5,     6,     7,     8,
       0,    10,    11,     0,    12,     0,    13,    14,    15,     0,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     0,
      10,    11,     0,    12,    17,    13,    14,    15,    18,    19,
       0,     0,     0,     0,     0,     0,     0,    20,     0,     0,
       0,    22,     0,    17,    83,     0,    23,    18,    19,     0,
       0,     0,     0,     0,     0,     0,    20,     0,     0,     0,
      22,     0,     0,     0,     0,    23,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,     0,     0,    68,     0,
       0,     0,    69,     0,     0,     0,     0,     0,     0,    70,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
       0,     0,    68,     0,     0,     0,   126,     0,     0,     0,
       0,     0,     0,    70,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,     0,     0,    68,     0,     0,     0,
       0,    90,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,     0,     0,    68,     0,     0,     0,     0,   127,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
       0,     0,    68,     0,     0,     0,     0,   150,    57,    58,
      59,    60,    61,    62,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,     0,     0,    68,   -76,   -76,   -76,
     -76,   -76,   -76,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,     0,     0,    68
};

static const yytype_int16 yycheck[] =
{
       0,     0,    22,    49,    45,    46,    42,    20,    21,    45,
      56,    49,    40,    41,    42,    47,    17,    45,    18,    19,
      20,    47,    22,    36,    46,    47,    46,    47,    52,    53,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      41,    42,   151,   152,    44,    38,    39,    40,    41,    42,
      47,     5,    45,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      71,    36,    72,    73,    74,    76,    77,    77,   119,    49,
      49,    45,    48,    50,    83,    53,    22,   128,    13,     3,
       4,     5,     6,     7,     8,     9,    23,    11,    25,    15,
      27,    28,    29,    30,    31,    32,    47,    50,    53,    55,
      71,    38,    39,    40,    41,    42,   145,   117,    45,   120,
      34,   148,   122,   123,    38,    39,   130,   137,   120,   130,
     130,    13,   132,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,     1,   148,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    -1,
      14,    -1,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    49,    -1,    51,    -1,    -1,
      -1,    -1,    56,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    12,    -1,    14,    -1,    16,    17,    18,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    12,    -1,    14,    34,    16,    17,    18,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    34,    54,    -1,    56,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    39,    40,    41,    42,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    50,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      41,    42,    -1,    -1,    45,    -1,    -1,    -1,    -1,    50,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,
      -1,    -1,    45,    -1,    -1,    -1,    -1,    50,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    -1,    -1,    45,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      40,    41,    42,    -1,    -1,    45
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    14,    16,    17,    18,    33,    34,    38,    39,
      47,    49,    51,    56,    58,    59,    60,    61,    62,    63,
      68,    72,    79,    80,    82,    84,    85,    86,    87,    88,
      49,    47,    47,    83,    47,    81,    81,    87,    59,    59,
      59,    59,     0,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    38,    39,    40,    41,    42,    45,    49,
      56,    20,    21,    36,    36,    49,    46,    47,    64,    73,
      75,    87,    87,    54,    59,    76,    79,    59,    76,    76,
      50,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    68,    87,    59,
      59,    59,    87,    59,    69,    71,    87,    48,    65,    50,
      53,    74,    22,    13,    77,    79,    49,    50,    15,    50,
      53,    70,    22,    59,    66,    67,    76,    75,    59,    59,
      55,    49,    56,    78,    76,    71,    59,    52,    53,    74,
      50,    79,    79,    70,    67,    78,    78
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 159 "grammar.y"
    { return 0; }
    break;

  case 3:
#line 160 "grammar.y"
    { return 0; }
    break;

  case 4:
#line 161 "grammar.y"
    { return 0;}
    break;

  case 5:
#line 162 "grammar.y"
    { return parser.execute((yyvsp[(1) - (2)].syntaxElement)); }
    break;

  case 6:
#line 163 "grammar.y"
    { return parser.execute((yyvsp[(1) - (2)].syntaxElement)); }
    break;

  case 7:
#line 164 "grammar.y"
    { YYABORT; }
    break;

  case 8:
#line 167 "grammar.y"
    { (yyval.syntaxElement) = new Constant((yyvsp[(1) - (1)].object)); }
    break;

  case 9:
#line 169 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 10:
#line 171 "grammar.y"
    { (yyval.syntaxElement) = new UnaryExpr(UnaryExpr::UMINUS, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 11:
#line 172 "grammar.y"
    { (yyval.syntaxElement) = new UnaryExpr(UnaryExpr::UPLUS, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 12:
#line 173 "grammar.y"
    { (yyval.syntaxElement) = new UnaryExpr(UnaryExpr::UNOT, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 13:
#line 174 "grammar.y"
    { (yyval.syntaxElement) = new HelpExpr((yyvsp[(2) - (2)].object)); }
    break;

  case 14:
#line 176 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::RANGE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 15:
#line 178 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::ADD, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 16:
#line 179 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::SUB, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 17:
#line 180 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::MUL, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 18:
#line 181 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::DIV, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 19:
#line 182 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::EXP, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 20:
#line 184 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::LT, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 21:
#line 185 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::LE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 22:
#line 186 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::EQ, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 23:
#line 187 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::NE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 24:
#line 188 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::GE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 25:
#line 189 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::GT, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 26:
#line 191 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::AND, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 27:
#line 192 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::OR, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 28:
#line 193 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::AND, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 29:
#line 194 "grammar.y"
    { (yyval.syntaxElement) = new BinaryExpr(BinaryExpr::OR, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 30:
#line 196 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 31:
#line 197 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 32:
#line 198 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 33:
#line 200 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 34:
#line 201 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 35:
#line 203 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 36:
#line 207 "grammar.y"
    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new AssignExpr(AssignExpr::ARROW_ASSIGN, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 37:
#line 212 "grammar.y"
    {
                        PRINTF("Parser inserting attribute assignment (ATTRIBUTE_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new AssignExpr(AssignExpr::ATTRIBUTE_ASSIGN, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 38:
#line 219 "grammar.y"
    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new AssignExpr(AssignExpr::TILDE_ASSIGN, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 39:
#line 226 "grammar.y"
    {
                        PRINTF("Parser inserting equation assignment (EQUAL_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new AssignExpr(AssignExpr::EQUAL_ASSIGN, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 40:
#line 233 "grammar.y"
    {
                    PRINTF("Parser inserting variable (VARIABLE) in syntax tree\n");
                    (yyval.syntaxElement) = new Variable((yyvsp[(1) - (3)].object), (yyvsp[(2) - (3)].object), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 41:
#line 239 "grammar.y"
    { (yyval.object) = (yyvsp[(2) - (2)].object); }
    break;

  case 42:
#line 240 "grammar.y"
    { (yyval.object) = NULL; }
    break;

  case 43:
#line 243 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 44:
#line 244 "grammar.y"
    { (yyval.syntaxElement) = NULL; }
    break;

  case 45:
#line 247 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement>((yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 46:
#line 248 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 47:
#line 251 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxElement(); }
    break;

  case 48:
#line 252 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 49:
#line 256 "grammar.y"
    { 
                        PRINTF("Parser inserting function call (FUNCTION_CALL) in syntax tree\n");
                        (yyval.syntaxElement) = new FunctionCall((yyvsp[(1) - (4)].object), (yyvsp[(3) - (4)].syntaxElementList));
                    }
    break;

  case 50:
#line 262 "grammar.y"
    { (yyvsp[(2) - (2)].syntaxElementList)->push_front((yyvsp[(1) - (2)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(2) - (2)].syntaxElementList); }
    break;

  case 51:
#line 263 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 52:
#line 266 "grammar.y"
    { (yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(3) - (3)].syntaxElementList); }
    break;

  case 53:
#line 267 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 54:
#line 271 "grammar.y"
    {
                    PRINTF("Parser inserting unlabeled argument (ARGUMENT) in syntax tree\n");
                    (yyval.syntaxElement) = new LabeledExpr(LabeledExpr::ARGUMENT, new RbDataType<string>(), (yyvsp[(1) - (1)].syntaxElement));
                }
    break;

  case 55:
#line 276 "grammar.y"
    { 
                    PRINTF("Parser inserting labeled argument (ARGUMENT) in syntax tree\n");
                    (yyval.syntaxElement) = new LabeledExpr(LabeledExpr::ARGUMENT, (yyvsp[(1) - (3)].object), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 56:
#line 283 "grammar.y"
    {
                PRINTF("Parser inserting function definition (FUNCTION_DEF) in syntax tree\n");
                (yyval.syntaxElement) = new FunctionDef((yyvsp[(3) - (5)].syntaxElementList), (yyvsp[(5) - (5)].syntaxElementList));
                }
    break;

  case 57:
#line 289 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(2) - (2)].syntaxElementList)->push_front((yyvsp[(1) - (2)].syntaxElement))); }
    break;

  case 58:
#line 290 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 59:
#line 293 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement))); }
    break;

  case 60:
#line 294 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 61:
#line 298 "grammar.y"
    {
                    PRINTF("Parser inserting unlabeled formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new LabeledExpr(LabeledExpr::FORMAL, new RbDataType<string>(), (yyvsp[(1) - (1)].object));
                }
    break;

  case 62:
#line 303 "grammar.y"
    { 
                    PRINTF("Parser inserting labeled formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new LabeledExpr(LabeledExpr::FORMAL, (yyvsp[(1) - (3)].object), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 63:
#line 309 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 64:
#line 310 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 65:
#line 313 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(2) - (2)].syntaxElementList)->push_front((yyvsp[(1) - (2)].syntaxElement))); }
    break;

  case 66:
#line 314 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 67:
#line 317 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement))); }
    break;

  case 68:
#line 318 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement))); }
    break;

  case 69:
#line 319 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 70:
#line 322 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 71:
#line 323 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 72:
#line 324 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 73:
#line 325 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 74:
#line 326 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 75:
#line 327 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (2)].syntaxElement); }
    break;

  case 76:
#line 328 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (2)].syntaxElement); }
    break;

  case 77:
#line 329 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxElement(); }
    break;

  case 78:
#line 332 "grammar.y"
    { (yyval.syntaxElement) = new Statement(Statement::IF, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 79:
#line 333 "grammar.y"
    { (yyval.syntaxElement) = new Statement(Statement::IF_ELSE, (yyvsp[(2) - (5)].syntaxElement), (yyvsp[(3) - (5)].syntaxElementList), (yyvsp[(5) - (5)].syntaxElementList)); }
    break;

  case 80:
#line 335 "grammar.y"
    { (yyval.syntaxElement) = new Condition((yyvsp[(2) - (3)].syntaxElement)); }
    break;

  case 81:
#line 338 "grammar.y"
    { (yyval.syntaxElement) = new Statement(Statement::FOR, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 82:
#line 341 "grammar.y"
    { (yyval.syntaxElement) = new ForCondition((yyvsp[(2) - (5)].object), (yyvsp[(4) - (5)].syntaxElement)); }
    break;

  case 83:
#line 344 "grammar.y"
    { (yyval.syntaxElement) = new Statement(Statement::WHILE, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 84:
#line 347 "grammar.y"
    { (yyval.syntaxElement) = new Statement(Statement::NEXT); }
    break;

  case 85:
#line 350 "grammar.y"
    { (yyval.syntaxElement) = new Statement(Statement::BREAK); }
    break;

  case 86:
#line 353 "grammar.y"
    { (yyval.object) = new RbDataType<string>((yyvsp[(1) - (1)].string)); }
    break;

  case 87:
#line 356 "grammar.y"
    { (yyval.object) = new RbDataType<bool>(false); }
    break;

  case 88:
#line 357 "grammar.y"
    { (yyval.object) = new RbDataType<bool>(true); }
    break;

  case 89:
#line 358 "grammar.y"
    { (yyval.object) = new RbDataType<int>(0); }
    break;

  case 90:
#line 359 "grammar.y"
    { (yyval.object) = new RbDataType<int>((yyvsp[(1) - (1)].intValue)); }
    break;

  case 91:
#line 360 "grammar.y"
    { (yyval.object) = new RbDataType<string>((yyvsp[(1) - (1)].string)); }
    break;

  case 92:
#line 362 "grammar.y"
    {
                    /* This code records and preserves input format of the real */
                    int prec;
                    bool sci;
                    int i=0;
                    while (yytext[i]!='\0') {
                    if (yytext[i]=='E' || yytext[i]=='e')
                        break;
                        i++;
                    }
                    if (yytext[i]!='\0') {
                        sci = true;
                        prec = i;
                        for (i=0; yytext[i]!='\0'; i++) {
                            if (yytext[i]=='.') {
                                prec--;
                            break;
                            }
                        }
                    }
                    else {
                        sci = false;
                        for (i=0; yytext[i]!='\0'; i++) {
                            if (yytext[i]=='.') {
                                break;
                            }
                        }
                        prec = strlen(yytext) - 1 - i;
                    }
                    RbDataType<double> *real = new RbDataType<double>((yyvsp[(1) - (1)].realValue));
                    real->setPrecision(prec);
                    real->setScientific(sci);
                    (yyval.object) = real;
                }
    break;


/* Line 1267 of yacc.c.  */
#line 2197 "grammar.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 398 "grammar.y"



/* Definition of yyerror. */
int yyerror(char *msg)
{
    PRINTF("Bison code said: %s\n", msg);
    return 0;     // or return 1 ??
}

