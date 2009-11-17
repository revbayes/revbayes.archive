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
     CLASS = 267,
     FOR = 268,
     IN = 269,
     IF = 270,
     ELSE = 271,
     WHILE = 272,
     NEXT = 273,
     BREAK = 274,
     RETURN = 275,
     LEFT_ASSIGN = 276,
     TILDE_ASSIGN = 277,
     EQUATION_ASSIGN = 278,
     EQUAL = 279,
     AND = 280,
     OR = 281,
     AND2 = 282,
     OR2 = 283,
     GT = 284,
     GE = 285,
     LT = 286,
     LE = 287,
     EQ = 288,
     NE = 289,
     END_OF_INPUT = 290,
     LOW = 291,
     ARROW_ASSIGN = 292,
     UNOT = 293,
     UPLUS = 294,
     UMINUS = 295
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
#define CLASS 267
#define FOR 268
#define IN 269
#define IF 270
#define ELSE 271
#define WHILE 272
#define NEXT 273
#define BREAK 274
#define RETURN 275
#define LEFT_ASSIGN 276
#define TILDE_ASSIGN 277
#define EQUATION_ASSIGN 278
#define EQUAL 279
#define AND 280
#define OR 281
#define AND2 282
#define OR2 283
#define GT 284
#define GE 285
#define LT 286
#define LE 287
#define EQ 288
#define NE 289
#define END_OF_INPUT 290
#define LOW 291
#define ARROW_ASSIGN 292
#define UNOT 293
#define UPLUS 294
#define UMINUS 295




/* Copy the first part of user declarations.  */
#line 1 "grammar.y"

/*
 * \file
 * Grammar specification in bison format for RevBayes, a computing environment 
 * for evolutionary analysis using Bayesian inference. RevBayes uses EvoMoDeL,
 * Evolutionary Model Description Language, to describe evolutionary models
 * used in Bayesian MCMC inference, so the EvoMoDeL grammar is a subset of the
 * RevBayes grammar.
 *
 * The grammar borrows heavily from the R grammar specification in the gram.y 
 * file of the R source code but deviates significantly in many respects, being
 * more similar to object-oriented languages like C++ or Java. EvoMoDeL itself
 * is inspired by the language used by BUGS to describe models. Unlike BUGS,
 * and similar programs such as JAGS, REvBayes allows models to be built in an
 * interpreted environment.
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

#include "Scalar.h"
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

#ifdef YY_VERBOSE
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
#line 78 "grammar.y"
{
    char*                  string;
    double                 realValue;
    int                    intValue;
    bool                   boolValue;
    SyntaxElement*         syntaxElement;
    list<SyntaxElement *>* syntaxElementList;
}
/* Line 193 of yacc.c.  */
#line 261 "grammar.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 274 "grammar.tab.c"

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
#define YYFINAL  59
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   597

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  101
/* YYNRULES -- Number of states.  */
#define YYNSTATES  186

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      51,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    53,     2,     2,     2,     2,     2,     2,
      49,    52,    42,    40,    55,    41,    48,    43,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    44,    58,
       2,     2,     2,    36,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    54,    47,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    56,     2,    57,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    37,    38,    39,    45,    46
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    16,    19,    21,
      23,    27,    30,    33,    36,    39,    43,    47,    51,    55,
      59,    63,    67,    71,    75,    79,    83,    87,    91,    95,
      99,   103,   105,   107,   109,   111,   113,   117,   124,   128,
     131,   136,   141,   142,   147,   154,   157,   158,   162,   163,
     165,   169,   176,   184,   187,   188,   192,   193,   195,   199,
     202,   207,   211,   213,   216,   217,   221,   225,   226,   228,
     230,   232,   234,   236,   238,   241,   244,   246,   248,   250,
     252,   256,   260,   262,   264,   272,   276,   282,   286,   290,
     296,   300,   302,   304,   306,   309,   311,   313,   315,   317,
     319,   321
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      60,     0,    -1,    35,    -1,    51,    -1,    10,    51,    -1,
      78,    51,    -1,    79,    51,    -1,    61,    51,    -1,     1,
      -1,    92,    -1,    49,    61,    52,    -1,    41,    61,    -1,
      40,    61,    -1,    53,    61,    -1,    36,    91,    -1,    61,
      44,    61,    -1,    61,    40,    61,    -1,    61,    41,    61,
      -1,    61,    42,    61,    -1,    61,    43,    61,    -1,    61,
      47,    61,    -1,    61,    31,    61,    -1,    61,    32,    61,
      -1,    61,    33,    61,    -1,    61,    34,    61,    -1,    61,
      30,    61,    -1,    61,    29,    61,    -1,    61,    25,    61,
      -1,    61,    26,    61,    -1,    61,    27,    61,    -1,    61,
      28,    61,    -1,    62,    -1,    64,    -1,    63,    -1,    67,
      -1,    65,    -1,    65,    38,    61,    -1,    65,    22,    91,
      49,    68,    52,    -1,    65,    23,    61,    -1,    91,    66,
      -1,    65,    48,    91,    66,    -1,    50,    61,    54,    66,
      -1,    -1,    91,    49,    68,    52,    -1,    65,    48,    91,
      49,    68,    52,    -1,    70,    69,    -1,    -1,    55,    70,
      69,    -1,    -1,    61,    -1,    91,    24,    61,    -1,    11,
      91,    49,    72,    52,    75,    -1,    11,    91,    91,    49,
      72,    52,    75,    -1,    74,    73,    -1,    -1,    55,    74,
      73,    -1,    -1,    91,    -1,    91,    24,    61,    -1,    91,
      91,    -1,    91,    91,    24,    61,    -1,    56,    76,    57,
      -1,    78,    -1,    78,    77,    -1,    -1,    58,    78,    77,
      -1,    51,    78,    77,    -1,    -1,    83,    -1,    85,    -1,
      87,    -1,    88,    -1,    89,    -1,    90,    -1,    61,    51,
      -1,    61,    58,    -1,    58,    -1,    82,    -1,    71,    -1,
      81,    -1,    80,    58,    81,    -1,    80,    51,    81,    -1,
      74,    -1,    71,    -1,    12,    91,    44,    91,    56,    80,
      57,    -1,    15,    84,    75,    -1,    15,    84,    75,    16,
      75,    -1,    49,    61,    52,    -1,    13,    86,    75,    -1,
      49,    91,    14,    61,    52,    -1,    17,    84,    75,    -1,
      18,    -1,    19,    -1,    20,    -1,    20,    61,    -1,     5,
      -1,     8,    -1,     9,    -1,     7,    -1,     4,    -1,     6,
      -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   176,   176,   177,   178,   179,   180,   181,   182,   185,
     187,   189,   190,   191,   192,   194,   196,   197,   198,   199,
     200,   202,   203,   204,   205,   206,   207,   209,   210,   211,
     212,   214,   215,   216,   218,   220,   223,   230,   237,   244,
     249,   256,   257,   260,   265,   272,   273,   276,   277,   280,
     285,   292,   297,   304,   305,   308,   309,   312,   317,   322,
     327,   334,   335,   338,   339,   342,   343,   344,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   358,   359,   362,
     363,   364,   367,   368,   371,   378,   379,   381,   384,   387,
     390,   393,   396,   399,   400,   403,   406,   407,   408,   409,
     410,   411
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "REAL", "INT", "NAME", "STRING", "NULL",
  "FALSE", "TRUE", "COMMENT", "FUNCTION", "CLASS", "FOR", "IN", "IF",
  "ELSE", "WHILE", "NEXT", "BREAK", "RETURN", "LEFT_ASSIGN",
  "TILDE_ASSIGN", "EQUATION_ASSIGN", "EQUAL", "AND", "OR", "AND2", "OR2",
  "GT", "GE", "LT", "LE", "EQ", "NE", "END_OF_INPUT", "'?'", "LOW",
  "ARROW_ASSIGN", "UNOT", "'+'", "'-'", "'*'", "'/'", "':'", "UPLUS",
  "UMINUS", "'^'", "'.'", "'('", "'['", "'\\n'", "')'", "'!'", "']'",
  "','", "'{'", "'}'", "';'", "$accept", "prog", "expression",
  "arrowAssign", "tildeAssign", "equationAssign", "variable", "optElement",
  "functionCall", "argumentList", "optArguments", "argument",
  "functionDef", "formalList", "optFormals", "formal", "stmts", "stmtList",
  "optStatements", "statement", "declaration", "memberDefs", "memberDef",
  "classDef", "ifStatement", "cond", "forStatement", "forCond",
  "whileStatement", "nextStatement", "breakStatement", "returnStatement",
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
     285,   286,   287,   288,   289,   290,    63,   291,   292,   293,
      43,    45,    42,    47,    58,   294,   295,    94,    46,    40,
      91,    10,    41,    33,    93,    44,   123,   125,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    59,    60,    60,    60,    60,    60,    60,    60,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    62,    63,    64,    65,
      65,    66,    66,    67,    67,    68,    68,    69,    69,    70,
      70,    71,    71,    72,    72,    73,    73,    74,    74,    74,
      74,    75,    75,    76,    76,    77,    77,    77,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    79,    79,    80,
      80,    80,    81,    81,    82,    83,    83,    84,    85,    86,
      87,    88,    89,    90,    90,    91,    92,    92,    92,    92,
      92,    92
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     1,     1,
       3,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     3,     6,     3,     2,
       4,     4,     0,     4,     6,     2,     0,     3,     0,     1,
       3,     6,     7,     2,     0,     3,     0,     1,     3,     2,
       4,     3,     1,     2,     0,     3,     3,     0,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       3,     3,     1,     1,     7,     3,     5,     3,     3,     5,
       3,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     8,   101,    99,    95,   100,    98,    96,    97,     0,
       0,     0,     0,     0,     0,    91,    92,    93,     2,     0,
       0,     0,     0,     3,     0,    76,     0,     0,    31,    33,
      32,    35,    34,    78,     0,     0,    77,    68,    69,    70,
      71,    72,    73,    42,     9,     4,     0,     0,     0,     0,
       0,     0,     0,    94,    14,    12,    11,     0,    13,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,    75,     0,     0,
       0,     0,     5,     6,    46,     0,    39,    54,     0,     0,
       0,    64,     0,    88,    62,     0,    85,    90,    10,    27,
      28,    29,    30,    26,    25,    21,    22,    23,    24,    16,
      17,    18,    19,    15,    20,     0,    38,    36,    42,    49,
       0,    48,    42,     0,     0,    56,    57,    54,     0,     0,
       0,    67,    74,    87,     0,    46,    46,    40,    43,     0,
      45,     0,    42,     0,     0,    53,     0,    59,     0,     0,
       0,    61,     0,     0,    63,    86,     0,     0,    48,    50,
      41,    51,    56,    58,     0,     0,    83,    82,     0,    79,
      89,    67,    67,    37,    44,    47,    55,    60,    52,     0,
      84,     0,    66,    65,    81,    80
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,    92,    28,    29,    30,    31,    86,    32,   120,
     140,   121,   166,   124,   145,   167,    93,   130,   154,    94,
      35,   168,   169,    36,    37,    51,    38,    49,    39,    40,
      41,    42,    43,    44
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -127
static const yytype_int16 yypact[] =
{
     182,  -127,  -127,  -127,  -127,  -127,  -127,  -127,  -127,   -47,
      16,    16,   -25,   -14,   -14,  -127,  -127,    23,  -127,    16,
      23,    23,    23,  -127,    23,  -127,    52,   328,  -127,  -127,
    -127,   -15,  -127,  -127,    18,    22,  -127,  -127,  -127,  -127,
    -127,  -127,  -127,     8,  -127,  -127,    -3,    31,    16,   238,
      23,   238,   238,   510,  -127,     9,     9,   426,    80,  -127,
      23,    23,    23,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    27,  -127,    16,    23,
      23,    16,  -127,  -127,    23,    23,  -127,    16,    49,    16,
      53,   294,   362,  -127,  -127,   454,    85,  -127,  -127,    80,
     531,    80,   531,   550,   550,   550,   550,   550,   550,     7,
       7,   -31,   -31,     9,     9,    58,   510,   510,    12,   510,
      56,    60,    -2,   396,    66,    64,     1,    16,    74,    23,
      75,   -39,  -127,  -127,   238,    23,    23,  -127,  -127,    23,
    -127,    23,    81,   238,    16,  -127,    23,   109,    83,    34,
     482,  -127,   294,   294,  -127,  -127,    84,    86,    60,   510,
    -127,  -127,    64,   510,    23,   238,  -127,  -127,   -40,  -127,
    -127,   -39,   -39,  -127,  -127,  -127,  -127,   510,  -127,    34,
    -127,    34,  -127,  -127,  -127,  -127
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -127,  -127,    20,  -127,  -127,  -127,  -127,  -108,  -127,   -70,
     -21,     2,   140,    15,   -19,   -84,   -37,  -127,   -69,     5,
    -127,  -127,  -126,  -127,  -127,   130,  -127,  -127,  -127,  -127,
    -127,  -127,   -10,  -127
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -75
static const yytype_int16 yytable[] =
{
      46,    47,     4,   125,    45,    34,     4,    78,    79,    54,
     137,   179,   152,    74,    96,    97,    75,   180,   181,   153,
      27,     4,   141,    80,    48,   146,     2,     3,     4,     5,
       6,     7,     8,    81,   160,    50,    88,    53,    90,     4,
      55,    56,    57,   125,    58,    10,    87,    84,    85,    72,
      73,    74,    59,   184,    75,   185,    75,    84,    85,    19,
     162,   136,    85,    20,    21,   156,   157,   129,   115,    82,
      95,   118,    22,    83,   122,    89,    24,   126,   -74,   128,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   131,   155,   127,   116,
     117,   134,   182,   183,   119,   123,   161,   135,   138,    64,
      65,    66,    67,    68,    69,   139,   147,   126,   143,   144,
      70,    71,    72,    73,    74,   122,   122,    75,   178,   122,
     149,    85,   151,   164,   126,   165,   173,   175,   174,   126,
      33,   158,   148,   176,    52,     0,     0,     0,     0,   150,
       0,     0,     0,     0,     0,   119,   119,   171,   172,   119,
       0,   159,     0,     0,     0,     0,   163,     0,     0,   126,
       0,   126,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,   177,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,     0,    13,     0,    14,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,    19,     0,
       0,     0,    20,    21,     0,     0,     0,     0,     0,     0,
       0,    22,     0,    23,     0,    24,     0,     0,     0,     0,
      25,     2,     3,     4,     5,     6,     7,     8,     0,     0,
       0,    12,     0,    13,     0,    14,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    19,     0,     0,     0,    20,    21,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,    24,     0,     0,    91,     0,    25,     2,     3,     4,
       5,     6,     7,     8,     0,     0,     0,    12,     0,    13,
       0,    14,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,     0,     0,     0,    20,    21,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,    24,     0,     0,
       0,     0,    25,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,     0,     0,    75,     0,     0,     0,    76,
       0,     0,     0,     0,     0,     0,    77,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,     0,     0,    75,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
      77,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,     0,     0,    75,     0,     0,     0,     0,     0,     0,
     142,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,     0,     0,    75,     0,     0,     0,     0,    98,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,     0,
       0,    75,     0,     0,     0,     0,   133,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,     0,     0,    75,
       0,     0,     0,     0,   170,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,     0,    60,    75,    62,     0,
      64,    65,    66,    67,    68,    69,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,     0,     0,    75,   -75,
     -75,   -75,   -75,   -75,   -75,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,     0,     0,    75
};

static const yytype_int16 yycheck[] =
{
      10,    11,     5,    87,    51,     0,     5,    22,    23,    19,
     118,    51,    51,    44,    51,    52,    47,    57,    58,    58,
       0,     5,    24,    38,    49,    24,     3,     4,     5,     6,
       7,     8,     9,    48,   142,    49,    46,    17,    48,     5,
      20,    21,    22,   127,    24,    11,    49,    49,    50,    42,
      43,    44,     0,   179,    47,   181,    47,    49,    50,    36,
     144,    49,    50,    40,    41,   135,   136,    14,    78,    51,
      50,    81,    49,    51,    84,    44,    53,    87,    51,    89,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    91,   134,    49,    79,
      80,    16,   171,   172,    84,    85,   143,    49,    52,    29,
      30,    31,    32,    33,    34,    55,   126,   127,    52,    55,
      40,    41,    42,    43,    44,   135,   136,    47,   165,   139,
      56,    50,    57,    24,   144,    52,    52,   158,    52,   149,
       0,   139,   127,   162,    14,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    -1,   135,   136,   152,   153,   139,
      -1,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,   179,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,   164,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    -1,    15,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      -1,    -1,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,
      58,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    -1,    15,    -1,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    40,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    56,    -1,    58,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    15,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    40,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    58,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    41,
      42,    43,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,
      44,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,
      44,    -1,    -1,    47,    -1,    -1,    -1,    -1,    52,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    41,    42,    43,    44,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    52,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    44,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    52,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    41,    42,    43,    44,    -1,    25,    47,    27,    -1,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,    43,    44,    -1,    -1,    47,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    41,    42,    43,    44,    -1,    -1,    47
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    15,    17,    18,    19,    20,    35,    36,
      40,    41,    49,    51,    53,    58,    60,    61,    62,    63,
      64,    65,    67,    71,    78,    79,    82,    83,    85,    87,
      88,    89,    90,    91,    92,    51,    91,    91,    49,    86,
      49,    84,    84,    61,    91,    61,    61,    61,    61,     0,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      40,    41,    42,    43,    44,    47,    51,    58,    22,    23,
      38,    48,    51,    51,    49,    50,    66,    49,    91,    44,
      91,    56,    61,    75,    78,    61,    75,    75,    52,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    91,    61,    61,    91,    61,
      68,    70,    91,    61,    72,    74,    91,    49,    91,    14,
      76,    78,    51,    52,    16,    49,    49,    66,    52,    55,
      69,    24,    54,    52,    55,    73,    24,    91,    72,    56,
      61,    57,    51,    58,    77,    75,    68,    68,    70,    61,
      66,    75,    74,    61,    24,    52,    71,    74,    80,    81,
      52,    78,    78,    52,    52,    69,    73,    61,    75,    51,
      57,    58,    77,    77,    81,    81
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
#line 176 "grammar.y"
    { return 0; }
    break;

  case 3:
#line 177 "grammar.y"
    { return 0; }
    break;

  case 4:
#line 178 "grammar.y"
    { return 0;}
    break;

  case 5:
#line 179 "grammar.y"
    { return parser.execute((yyvsp[(1) - (2)].syntaxElement)); }
    break;

  case 6:
#line 180 "grammar.y"
    { return parser.execute((yyvsp[(1) - (2)].syntaxElement)); }
    break;

  case 7:
#line 181 "grammar.y"
    { return parser.execute((yyvsp[(1) - (2)].syntaxElement)); }
    break;

  case 8:
#line 182 "grammar.y"
    { YYABORT; }
    break;

  case 9:
#line 185 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 10:
#line 187 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 11:
#line 189 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UMINUS, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 12:
#line 190 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UPLUS, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 13:
#line 191 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UNOT, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 14:
#line 192 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxHelpExpr((yyvsp[(2) - (2)].string)); }
    break;

  case 15:
#line 194 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::RANGE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 16:
#line 196 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::ADD, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 17:
#line 197 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::SUB, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 18:
#line 198 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::MUL, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 19:
#line 199 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::DIV, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 20:
#line 200 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::EXP, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 21:
#line 202 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::LT, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 22:
#line 203 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::LE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 23:
#line 204 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::EQ, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 24:
#line 205 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::NE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 25:
#line 206 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::GE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 26:
#line 207 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::GT, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 27:
#line 209 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::AND, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 28:
#line 210 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::OR, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 29:
#line 211 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::AND, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 30:
#line 212 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::OR, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 31:
#line 214 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 32:
#line 215 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 33:
#line 216 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 34:
#line 218 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 35:
#line 220 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 36:
#line 224 "grammar.y"
    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::ARROW_ASSIGN, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 37:
#line 231 "grammar.y"
    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::TILDE_ASSIGN, (yyvsp[(1) - (6)].syntaxElement), (yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].syntaxElementList));
                    }
    break;

  case 38:
#line 238 "grammar.y"
    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::EQUATION_ASSIGN, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 39:
#line 245 "grammar.y"
    {
                    PRINTF("Parser inserting variable (VARIABLE) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxVariable((yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].syntaxElement));
                }
    break;

  case 40:
#line 250 "grammar.y"
    {
                    PRINTF("Parser inserting member variable (MEMBER) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxVariable((yyvsp[(1) - (4)].syntaxElement), (yyvsp[(3) - (4)].string), (yyvsp[(4) - (4)].syntaxElement));
                }
    break;

  case 41:
#line 256 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(4) - (4)].syntaxElement)->push_front((yyvsp[(2) - (4)].syntaxElement)); }
    break;

  case 42:
#line 257 "grammar.y"
    { (yyval.syntaxElement) = new list<SyntaxElement*>(); }
    break;

  case 43:
#line 261 "grammar.y"
    { 
                        PRINTF("Parser inserting function call (FUNCTION_CALL) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxFunctionCall(string((yyvsp[(1) - (4)].string)), (yyvsp[(3) - (4)].syntaxElementList));
                    }
    break;

  case 44:
#line 266 "grammar.y"
    {
                        PRINTF("Parser inserting member function call (MEMBER_FUNCTION_CALL) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxFunctionCall((yyvsp[(1) - (6)].syntaxElement), string((yyvsp[(3) - (6)].string)), (yyvsp[(5) - (6)].syntaxElementList));
                    }
    break;

  case 45:
#line 272 "grammar.y"
    { (yyvsp[(2) - (2)].syntaxElementList)->push_front((yyvsp[(1) - (2)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(2) - (2)].syntaxElementList); }
    break;

  case 46:
#line 273 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 47:
#line 276 "grammar.y"
    { (yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(3) - (3)].syntaxElementList); }
    break;

  case 48:
#line 277 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 49:
#line 281 "grammar.y"
    {
                    PRINTF("Parser inserting unlabeled argument (ARGUMENT) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr(NULL, (yyvsp[(1) - (1)].syntaxElement));
                }
    break;

  case 50:
#line 286 "grammar.y"
    { 
                    PRINTF("Parser inserting labeled argument (ARGUMENT) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr(string((yyvsp[(1) - (3)].string)), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 51:
#line 293 "grammar.y"
    {
                PRINTF("Parser inserting function definition (FUNCTION_DEF) in syntax tree\n");
                (yyval.syntaxElement) = new SyntaxFunctionDef(NULL, (yyvsp[(2) - (6)].string), (yyvsp[(4) - (6)].syntaxElementList), (yyvsp[(6) - (6)].syntaxElementList));
                }
    break;

  case 52:
#line 298 "grammar.y"
    {
                PRINTF("Parser inserting typed function definition (FUNCTION_DEF) in syntax tree\n");
                (yyval.syntaxElement) = new SyntaxFunctionDef((yyvsp[(2) - (7)].string), (yyvsp[(3) - (7)].string), (yyvsp[(5) - (7)].syntaxElementList), (yyvsp[(7) - (7)].syntaxElementList));
                }
    break;

  case 53:
#line 304 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(2) - (2)].syntaxElementList)->push_front((yyvsp[(1) - (2)].syntaxElement))); }
    break;

  case 54:
#line 305 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 55:
#line 308 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement))); }
    break;

  case 56:
#line 309 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 57:
#line 313 "grammar.y"
    {
                    PRINTF("Parser inserting unlabeled formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr(NULL, (yyvsp[(1) - (1)].string), NULL);
                }
    break;

  case 58:
#line 318 "grammar.y"
    { 
                    PRINTF("Parser inserting labeled formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr(NULL, (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 59:
#line 323 "grammar.y"
    {
                    PRINTF("Parser inserting unlabeled typed formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr((yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].string), NULL);
                }
    break;

  case 60:
#line 328 "grammar.y"
    {
                    PRINTF("Parser inserting labeled typed formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr((yyvsp[(1) - (4)].string), (yyvsp[(2) - (4)].string), (yyvsp[(4) - (4)].syntaxElement));
                }
    break;

  case 61:
#line 334 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 62:
#line 335 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 63:
#line 338 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(2) - (2)].syntaxElementList)->push_front((yyvsp[(1) - (2)].syntaxElement))); }
    break;

  case 64:
#line 339 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 65:
#line 342 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement))); }
    break;

  case 66:
#line 343 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement))); }
    break;

  case 67:
#line 344 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 68:
#line 347 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 69:
#line 348 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 70:
#line 349 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 71:
#line 350 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 72:
#line 351 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 73:
#line 352 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 74:
#line 353 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (2)].syntaxElement); }
    break;

  case 75:
#line 354 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (2)].syntaxElement); }
    break;

  case 76:
#line 355 "grammar.y"
    { (yyval.syntaxElement) = NULL; }
    break;

  case 77:
#line 358 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 78:
#line 359 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 79:
#line 362 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement*>((yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 80:
#line 363 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 81:
#line 364 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 82:
#line 367 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 83:
#line 368 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 84:
#line 372 "grammar.y"
    {
                PRINTF("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
                (yyval.syntaxElement) = new SyntaxClassDef((yyvsp[(2) - (7)].string), (yyvsp[(4) - (7)].string), (yyvsp[(6) - (7)].syntaxElementList));
                }
    break;

  case 85:
#line 378 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::IF, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 86:
#line 379 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::IF_ELSE, (yyvsp[(2) - (5)].syntaxElement), (yyvsp[(3) - (5)].syntaxElementList), (yyvsp[(5) - (5)].syntaxElementList)); }
    break;

  case 87:
#line 381 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxCondition((yyvsp[(2) - (3)].syntaxElement)); }
    break;

  case 88:
#line 384 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::FOR, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 89:
#line 387 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxForCondition(string((yyvsp[(2) - (5)].string)), (yyvsp[(4) - (5)].syntaxElement)); }
    break;

  case 90:
#line 390 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::WHILE, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 91:
#line 393 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::NEXT); }
    break;

  case 92:
#line 396 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::BREAK); }
    break;

  case 93:
#line 399 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::RETURN); }
    break;

  case 94:
#line 400 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::RETURN, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 95:
#line 403 "grammar.y"
    { (yyval.string) = new RbString((yyvsp[(1) - (1)].string)); }
    break;

  case 96:
#line 406 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant(false); }
    break;

  case 97:
#line 407 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant(true); }
    break;

  case 98:
#line 408 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant(0); }
    break;

  case 99:
#line 409 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant((yyvsp[(1) - (1)].intValue)); }
    break;

  case 100:
#line 410 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant((yyvsp[(1) - (1)].string))); }
    break;

  case 101:
#line 412 "grammar.y"
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
                    Scalar *real = new Scalar((yyvsp[(1) - (1)].realValue));
                    //real->setPrecision(prec);
                    //real->setScientific(sci);
                    (yyval.syntaxElement) = new SyntaxConstat(real);
                }
    break;


/* Line 1267 of yacc.c.  */
#line 2306 "grammar.tab.c"
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


#line 448 "grammar.y"



/* Definition of yyerror. */
int yyerror(char *msg)
{
    PRINTF("Bison code said: %s\n", msg);
    return 0;     // or return 1 ??
}

