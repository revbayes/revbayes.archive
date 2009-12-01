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
     RIGHT_ARROW = 280,
     AND = 281,
     OR = 282,
     AND2 = 283,
     OR2 = 284,
     GT = 285,
     GE = 286,
     LT = 287,
     LE = 288,
     EQ = 289,
     NE = 290,
     END_OF_INPUT = 291,
     LOW = 292,
     ARROW_ASSIGN = 293,
     UNOT = 294,
     UPLUS = 295,
     UMINUS = 296
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
#define RIGHT_ARROW 280
#define AND 281
#define OR 282
#define AND2 283
#define OR2 284
#define GT 285
#define GE 286
#define LT 287
#define LE 288
#define EQ 289
#define NE 290
#define END_OF_INPUT 291
#define LOW 292
#define ARROW_ASSIGN 293
#define UNOT 294
#define UPLUS 295
#define UMINUS 296




/* Copy the first part of user declarations.  */
#line 1 "grammar.y"

/**
 * @file
 * Grammar specification in bison format for RevBayes, a computing environment 
 * for evolutionary analysis using Bayesian inference. RevBayes uses EvoMoDeL,
 * Evolutionary Model Description Language, to describe evolutionary models,
 * so the EvoMoDeL grammar is a subset of the RevBayes grammar.
 *
 * The grammar borrows heavily from the R grammar specification in the gram.y 
 * file of the R source code but deviates significantly in many respects, being
 * more similar to object-oriented languages like C++ or Java. EvoMoDeL itself
 * is inspired by the language used by BUGS to describe models. Unlike BUGS,
 * and similar programs such as JAGS, REvBayes allows models to be built in an
 * interpreted (interactive) environment.
 *
 * @brief Grammar specification in bison format
 *
 * @author Fredrik Ronquist
 * @date Last modified: $Date$
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
#line 77 "grammar.y"
{
    char*                  string;
    double                 realValue;
    int                    intValue;
    bool                   boolValue;
    SyntaxElement*         syntaxElement;
    list<SyntaxElement *>* syntaxElementList;
}
/* Line 193 of yacc.c.  */
#line 262 "grammar.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 275 "grammar.tab.c"

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
#define YYLAST   574

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNRULES -- Number of states.  */
#define YYNSTATES  189

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      52,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    54,     2,     2,     2,     2,     2,     2,
      50,    53,    43,    41,    56,    42,    49,    44,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    45,    59,
       2,     2,     2,    37,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    51,     2,    55,    48,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    57,     2,    58,     2,     2,     2,     2,
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
      35,    36,    38,    39,    40,    46,    47
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
     131,   136,   141,   146,   147,   152,   159,   162,   163,   167,
     168,   170,   174,   181,   189,   192,   193,   197,   198,   200,
     204,   207,   212,   216,   218,   221,   222,   226,   230,   231,
     233,   235,   237,   239,   241,   243,   246,   249,   251,   253,
     255,   257,   261,   265,   267,   269,   277,   281,   287,   291,
     295,   301,   305,   307,   309,   311,   314,   316,   318,   320,
     322,   324,   326
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      61,     0,    -1,    36,    -1,    52,    -1,    10,    52,    -1,
      79,    52,    -1,    80,    52,    -1,    62,    52,    -1,     1,
      -1,    93,    -1,    50,    62,    53,    -1,    42,    62,    -1,
      41,    62,    -1,    54,    62,    -1,    37,    92,    -1,    62,
      45,    62,    -1,    62,    41,    62,    -1,    62,    42,    62,
      -1,    62,    43,    62,    -1,    62,    44,    62,    -1,    62,
      48,    62,    -1,    62,    32,    62,    -1,    62,    33,    62,
      -1,    62,    34,    62,    -1,    62,    35,    62,    -1,    62,
      31,    62,    -1,    62,    30,    62,    -1,    62,    26,    62,
      -1,    62,    27,    62,    -1,    62,    28,    62,    -1,    62,
      29,    62,    -1,    63,    -1,    65,    -1,    64,    -1,    68,
      -1,    66,    -1,    66,    39,    62,    -1,    66,    22,    92,
      50,    69,    53,    -1,    66,    23,    62,    -1,    92,    67,
      -1,    66,    49,    92,    67,    -1,    66,    25,    92,    67,
      -1,    51,    62,    55,    67,    -1,    -1,    92,    50,    69,
      53,    -1,    66,    49,    92,    50,    69,    53,    -1,    71,
      70,    -1,    -1,    56,    71,    70,    -1,    -1,    62,    -1,
      92,    24,    62,    -1,    11,    92,    50,    73,    53,    76,
      -1,    11,    92,    92,    50,    73,    53,    76,    -1,    75,
      74,    -1,    -1,    56,    75,    74,    -1,    -1,    92,    -1,
      92,    24,    62,    -1,    92,    92,    -1,    92,    92,    24,
      62,    -1,    57,    77,    58,    -1,    79,    -1,    79,    78,
      -1,    -1,    59,    79,    78,    -1,    52,    79,    78,    -1,
      -1,    84,    -1,    86,    -1,    88,    -1,    89,    -1,    90,
      -1,    91,    -1,    62,    52,    -1,    62,    59,    -1,    59,
      -1,    83,    -1,    72,    -1,    82,    -1,    81,    59,    82,
      -1,    81,    52,    82,    -1,    75,    -1,    72,    -1,    12,
      92,    45,    92,    57,    81,    58,    -1,    15,    85,    76,
      -1,    15,    85,    76,    16,    76,    -1,    50,    62,    53,
      -1,    13,    87,    76,    -1,    50,    92,    14,    62,    53,
      -1,    17,    85,    76,    -1,    18,    -1,    19,    -1,    20,
      -1,    20,    62,    -1,     5,    -1,     8,    -1,     9,    -1,
       7,    -1,     4,    -1,     6,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   175,   175,   176,   177,   178,   179,   180,   181,   184,
     186,   188,   189,   190,   191,   193,   195,   196,   197,   198,
     199,   201,   202,   203,   204,   205,   206,   208,   209,   210,
     211,   213,   214,   215,   217,   219,   222,   229,   236,   243,
     248,   253,   260,   261,   264,   269,   276,   277,   280,   281,
     284,   289,   296,   301,   308,   309,   312,   313,   316,   321,
     326,   331,   338,   339,   342,   343,   346,   347,   348,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   362,   363,
     366,   367,   368,   371,   372,   375,   382,   383,   385,   388,
     391,   394,   397,   400,   403,   404,   407,   410,   411,   412,
     413,   414,   415
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
  "TILDE_ASSIGN", "EQUATION_ASSIGN", "EQUAL", "RIGHT_ARROW", "AND", "OR",
  "AND2", "OR2", "GT", "GE", "LT", "LE", "EQ", "NE", "END_OF_INPUT", "'?'",
  "LOW", "ARROW_ASSIGN", "UNOT", "'+'", "'-'", "'*'", "'/'", "':'",
  "UPLUS", "UMINUS", "'^'", "'.'", "'('", "'['", "'\\n'", "')'", "'!'",
  "']'", "','", "'{'", "'}'", "';'", "$accept", "prog", "expression",
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
     285,   286,   287,   288,   289,   290,   291,    63,   292,   293,
     294,    43,    45,    42,    47,    58,   295,   296,    94,    46,
      40,    91,    10,    41,    33,    93,    44,   123,   125,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    60,    61,    61,    61,    61,    61,    61,    61,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    63,    64,    65,    66,
      66,    66,    67,    67,    68,    68,    69,    69,    70,    70,
      71,    71,    72,    72,    73,    73,    74,    74,    75,    75,
      75,    75,    76,    76,    77,    77,    78,    78,    78,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    80,    80,
      81,    81,    81,    82,    82,    83,    84,    84,    85,    86,
      87,    88,    89,    90,    91,    91,    92,    93,    93,    93,
      93,    93,    93
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     1,     1,
       3,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     3,     6,     3,     2,
       4,     4,     4,     0,     4,     6,     2,     0,     3,     0,
       1,     3,     6,     7,     2,     0,     3,     0,     1,     3,
       2,     4,     3,     1,     2,     0,     3,     3,     0,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     3,     3,     1,     1,     7,     3,     5,     3,     3,
       5,     3,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     8,   102,   100,    96,   101,    99,    97,    98,     0,
       0,     0,     0,     0,     0,    92,    93,    94,     2,     0,
       0,     0,     0,     3,     0,    77,     0,     0,    31,    33,
      32,    35,    34,    79,     0,     0,    78,    69,    70,    71,
      72,    73,    74,    43,     9,     4,     0,     0,     0,     0,
       0,     0,     0,    95,    14,    12,    11,     0,    13,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,    76,     0,     0,
       0,     0,     0,     5,     6,    47,     0,    39,    55,     0,
       0,     0,    65,     0,    89,    63,     0,    86,    91,    10,
      27,    28,    29,    30,    26,    25,    21,    22,    23,    24,
      16,    17,    18,    19,    15,    20,     0,    38,    43,    36,
      43,    50,     0,    49,    43,     0,     0,    57,    58,    55,
       0,     0,     0,    68,    75,    88,     0,    47,    41,    47,
      40,    44,     0,    46,     0,    43,     0,     0,    54,     0,
      60,     0,     0,     0,    62,     0,     0,    64,    87,     0,
       0,    49,    51,    42,    52,    57,    59,     0,     0,    84,
      83,     0,    80,    90,    68,    68,    37,    45,    48,    56,
      61,    53,     0,    85,     0,    67,    66,    82,    81
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,    93,    28,    29,    30,    31,    87,    32,   122,
     143,   123,   169,   126,   148,   170,    94,   132,   157,    95,
      35,   171,   172,    36,    37,    51,    38,    49,    39,    40,
      41,    42,    43,    44
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -126
static const yytype_int16 yypact[] =
{
     175,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,   -49,
      13,    13,     3,    19,    19,  -126,  -126,    23,  -126,    13,
      23,    23,    23,  -126,    23,  -126,    40,   323,  -126,  -126,
    -126,    -2,  -126,  -126,     6,    27,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,    12,  -126,  -126,    -1,    26,    13,   232,
      23,   232,   232,   505,  -126,    28,    28,   421,   223,  -126,
      23,    23,    23,    23,    23,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    31,  -126,    13,    23,
      13,    23,    13,  -126,  -126,    23,    23,  -126,    13,    50,
      13,    88,   289,   357,  -126,  -126,   449,    91,  -126,  -126,
     223,   526,   223,   526,   280,   280,   280,   280,   280,   280,
       7,     7,    -6,    -6,    28,    28,    54,   505,    57,   505,
      16,   505,    59,    58,   -17,   391,    60,    61,     1,    13,
      63,    23,    64,   -37,  -126,  -126,   232,    23,  -126,    23,
    -126,  -126,    23,  -126,    23,    57,   232,    13,  -126,    23,
      92,    62,     8,   477,  -126,   289,   289,  -126,  -126,    68,
      70,    58,   505,  -126,  -126,    61,   505,    23,   232,  -126,
    -126,   -47,  -126,  -126,   -37,   -37,  -126,  -126,  -126,  -126,
     505,  -126,     8,  -126,     8,  -126,  -126,  -126,  -126
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -126,  -126,    24,  -126,  -126,  -126,  -126,  -110,  -126,   -83,
     -36,   -18,   126,     2,   -31,   -86,   -35,  -126,   -93,    14,
    -126,  -126,  -125,  -126,  -126,   114,  -126,  -126,  -126,  -126,
    -126,  -126,   -10,  -126
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -76
static const yytype_int16 yytable[] =
{
      46,    47,   127,    45,     4,   182,     4,   144,   138,    54,
     140,   183,   184,     4,    34,   155,    97,    98,     4,    10,
      78,    79,   156,    80,    27,   149,     2,     3,     4,     5,
       6,     7,     8,    85,    86,   163,    89,    81,    91,    74,
      59,    53,    75,   127,    55,    56,    57,    82,    58,    88,
      72,    73,    74,    48,   159,    75,   160,   187,    83,   188,
      19,   165,    85,    86,    20,    21,   139,    86,   116,    50,
     118,    90,   120,    22,    96,   124,    75,    24,   128,    84,
     130,   185,   186,   -75,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     129,   158,   131,   117,   137,   119,   133,   136,    86,   121,
     125,   164,   141,   146,   142,   168,   167,   147,   150,   128,
     152,   176,   154,   177,   161,   178,    33,   124,    52,   124,
       0,   151,   124,   181,   179,     0,     0,   128,     0,     0,
       0,     0,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   153,     0,     0,     0,     0,
       0,   121,     0,   121,     0,     0,   121,     0,   162,   174,
     175,     0,   128,   166,   128,     0,     1,     0,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
      13,   180,    14,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,    19,     0,     0,     0,    20,    21,     0,     0,
       0,     0,     0,     0,     0,    22,     0,    23,     0,    24,
       0,     0,     0,     0,    25,     2,     3,     4,     5,     6,
       7,     8,     0,     0,     0,    12,     0,    13,     0,    14,
      15,    16,    17,    64,    65,    66,    67,    68,    69,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,    19,
       0,    75,     0,    20,    21,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,    24,     0,     0,    92,
       0,    25,     2,     3,     4,     5,     6,     7,     8,     0,
       0,     0,    12,     0,    13,     0,    14,    15,    16,    17,
     -76,   -76,   -76,   -76,   -76,   -76,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,    19,     0,    75,     0,
      20,    21,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,    24,     0,     0,     0,     0,    25,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,     0,
       0,     0,     0,     0,    70,    71,    72,    73,    74,     0,
       0,    75,     0,     0,     0,    76,     0,     0,     0,     0,
       0,     0,    77,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,     0,     0,    75,     0,     0,     0,   134,
       0,     0,     0,     0,     0,     0,    77,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,     0,     0,    75,
       0,     0,     0,     0,     0,     0,   145,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,     0,     0,     0,
       0,     0,    70,    71,    72,    73,    74,     0,     0,    75,
       0,     0,     0,     0,    99,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,     0,     0,    75,     0,     0,
       0,     0,   135,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,     0,     0,    75,     0,     0,     0,     0,
     173,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,     0,    60,    75,    62,     0,    64,    65,    66,    67,
      68,    69,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,     0,     0,    75
};

static const yytype_int16 yycheck[] =
{
      10,    11,    88,    52,     5,    52,     5,    24,   118,    19,
     120,    58,    59,     5,     0,    52,    51,    52,     5,    11,
      22,    23,    59,    25,     0,    24,     3,     4,     5,     6,
       7,     8,     9,    50,    51,   145,    46,    39,    48,    45,
       0,    17,    48,   129,    20,    21,    22,    49,    24,    50,
      43,    44,    45,    50,   137,    48,   139,   182,    52,   184,
      37,   147,    50,    51,    41,    42,    50,    51,    78,    50,
      80,    45,    82,    50,    50,    85,    48,    54,    88,    52,
      90,   174,   175,    52,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      50,   136,    14,    79,    50,    81,    92,    16,    51,    85,
      86,   146,    53,    53,    56,    53,    24,    56,   128,   129,
      57,    53,    58,    53,   142,   161,     0,   137,    14,   139,
      -1,   129,   142,   168,   165,    -1,    -1,   147,    -1,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,   137,    -1,   139,    -1,    -1,   142,    -1,   144,   155,
     156,    -1,   182,   149,   184,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    -1,
      15,   167,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    52,    -1,    54,
      -1,    -1,    -1,    -1,    59,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    -1,    15,    -1,    17,
      18,    19,    20,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,    37,
      -1,    48,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    57,
      -1,    59,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    15,    -1,    17,    18,    19,    20,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    41,    42,    43,    44,    45,    37,    -1,    48,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    59,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    53,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    53,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      53,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    41,    42,    43,    44,
      45,    -1,    26,    48,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,    43,
      44,    45,    -1,    -1,    48
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    15,    17,    18,    19,    20,    36,    37,
      41,    42,    50,    52,    54,    59,    61,    62,    63,    64,
      65,    66,    68,    72,    79,    80,    83,    84,    86,    88,
      89,    90,    91,    92,    93,    52,    92,    92,    50,    87,
      50,    85,    85,    62,    92,    62,    62,    62,    62,     0,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      41,    42,    43,    44,    45,    48,    52,    59,    22,    23,
      25,    39,    49,    52,    52,    50,    51,    67,    50,    92,
      45,    92,    57,    62,    76,    79,    62,    76,    76,    53,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    92,    62,    92,    62,
      92,    62,    69,    71,    92,    62,    73,    75,    92,    50,
      92,    14,    77,    79,    52,    53,    16,    50,    67,    50,
      67,    53,    56,    70,    24,    55,    53,    56,    74,    24,
      92,    73,    57,    62,    58,    52,    59,    78,    76,    69,
      69,    71,    62,    67,    76,    75,    62,    24,    53,    72,
      75,    81,    82,    53,    79,    79,    53,    53,    70,    74,
      62,    76,    52,    58,    59,    78,    78,    82,    82
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
#line 175 "grammar.y"
    { return 0; }
    break;

  case 3:
#line 176 "grammar.y"
    { return 0; }
    break;

  case 4:
#line 177 "grammar.y"
    { return 0;}
    break;

  case 5:
#line 178 "grammar.y"
    { return parser.execute((yyvsp[(1) - (2)].syntaxElement)); }
    break;

  case 6:
#line 179 "grammar.y"
    { return parser.execute((yyvsp[(1) - (2)].syntaxElement)); }
    break;

  case 7:
#line 180 "grammar.y"
    { return parser.execute((yyvsp[(1) - (2)].syntaxElement)); }
    break;

  case 8:
#line 181 "grammar.y"
    { YYABORT; }
    break;

  case 9:
#line 184 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 10:
#line 186 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 11:
#line 188 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UMINUS, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 12:
#line 189 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UPLUS, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 13:
#line 190 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UNOT, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 14:
#line 191 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxHelpExpr((yyvsp[(2) - (2)].string)); }
    break;

  case 15:
#line 193 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::RANGE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 16:
#line 195 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::ADD, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 17:
#line 196 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::SUB, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 18:
#line 197 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::MUL, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 19:
#line 198 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::DIV, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 20:
#line 199 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::EXP, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 21:
#line 201 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::LT, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 22:
#line 202 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::LE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 23:
#line 203 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::EQ, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 24:
#line 204 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::NE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 25:
#line 205 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::GE, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 26:
#line 206 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::GT, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 27:
#line 208 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::AND, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 28:
#line 209 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::OR, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 29:
#line 210 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::AND, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 30:
#line 211 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::OR, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 31:
#line 213 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 32:
#line 214 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 33:
#line 215 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 34:
#line 217 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 35:
#line 219 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 36:
#line 223 "grammar.y"
    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::ARROW_ASSIGN, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 37:
#line 230 "grammar.y"
    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::TILDE_ASSIGN, (yyvsp[(1) - (6)].syntaxElement), (yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].syntaxElementList));
                    }
    break;

  case 38:
#line 237 "grammar.y"
    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::EQUATION_ASSIGN, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 39:
#line 244 "grammar.y"
    {
                    PRINTF("Parser inserting variable (VARIABLE) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxVariable((yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].syntaxElementList));
                }
    break;

  case 40:
#line 249 "grammar.y"
    {
                    PRINTF("Parser inserting member variable (MEMBER) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxVariable((yyvsp[(1) - (4)].syntaxElement), (yyvsp[(3) - (4)].string), (yyvsp[(4) - (4)].syntaxElementList));
                }
    break;

  case 41:
#line 254 "grammar.y"
    {
                    PRINTF("Parser inserting member variable (MEMBER) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxVariable((yyvsp[(1) - (4)].syntaxElement), (yyvsp[(3) - (4)].string), (yyvsp[(4) - (4)].syntaxElementList));
                }
    break;

  case 42:
#line 260 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(4) - (4)].syntaxElementList)->push_front((yyvsp[(2) - (4)].syntaxElement)); }
    break;

  case 43:
#line 261 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement*>(); }
    break;

  case 44:
#line 265 "grammar.y"
    { 
                        PRINTF("Parser inserting function call (FUNCTION_CALL) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxFunctionCall(string((yyvsp[(1) - (4)].string)), (yyvsp[(3) - (4)].syntaxElementList));
                    }
    break;

  case 45:
#line 270 "grammar.y"
    {
                        PRINTF("Parser inserting member function call (MEMBER_FUNCTION_CALL) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxFunctionCall((yyvsp[(1) - (6)].syntaxElement), string((yyvsp[(3) - (6)].string)), (yyvsp[(5) - (6)].syntaxElementList));
                    }
    break;

  case 46:
#line 276 "grammar.y"
    { (yyvsp[(2) - (2)].syntaxElementList)->push_front((yyvsp[(1) - (2)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(2) - (2)].syntaxElementList); }
    break;

  case 47:
#line 277 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 48:
#line 280 "grammar.y"
    { (yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(3) - (3)].syntaxElementList); }
    break;

  case 49:
#line 281 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 50:
#line 285 "grammar.y"
    {
                    PRINTF("Parser inserting unlabeled argument (ARGUMENT) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr(NULL, (yyvsp[(1) - (1)].syntaxElement));
                }
    break;

  case 51:
#line 290 "grammar.y"
    { 
                    PRINTF("Parser inserting labeled argument (ARGUMENT) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr(string((yyvsp[(1) - (3)].string)), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 52:
#line 297 "grammar.y"
    {
                PRINTF("Parser inserting function definition (FUNCTION_DEF) in syntax tree\n");
                (yyval.syntaxElement) = new SyntaxFunctionDef(NULL, (yyvsp[(2) - (6)].string), (yyvsp[(4) - (6)].syntaxElementList), (yyvsp[(6) - (6)].syntaxElementList));
                }
    break;

  case 53:
#line 302 "grammar.y"
    {
                PRINTF("Parser inserting typed function definition (FUNCTION_DEF) in syntax tree\n");
                (yyval.syntaxElement) = new SyntaxFunctionDef((yyvsp[(2) - (7)].string), (yyvsp[(3) - (7)].string), (yyvsp[(5) - (7)].syntaxElementList), (yyvsp[(7) - (7)].syntaxElementList));
                }
    break;

  case 54:
#line 308 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(2) - (2)].syntaxElementList)->push_front((yyvsp[(1) - (2)].syntaxElement))); }
    break;

  case 55:
#line 309 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 56:
#line 312 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement))); }
    break;

  case 57:
#line 313 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 58:
#line 317 "grammar.y"
    {
                    PRINTF("Parser inserting unlabeled formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr(NULL, (yyvsp[(1) - (1)].string), NULL);
                }
    break;

  case 59:
#line 322 "grammar.y"
    { 
                    PRINTF("Parser inserting labeled formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr(NULL, (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 60:
#line 327 "grammar.y"
    {
                    PRINTF("Parser inserting unlabeled typed formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr((yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].string), NULL);
                }
    break;

  case 61:
#line 332 "grammar.y"
    {
                    PRINTF("Parser inserting labeled typed formal argument (FORMAL) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxLabeledExpr((yyvsp[(1) - (4)].string), (yyvsp[(2) - (4)].string), (yyvsp[(4) - (4)].syntaxElement));
                }
    break;

  case 62:
#line 338 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 63:
#line 339 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 64:
#line 342 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(2) - (2)].syntaxElementList)->push_front((yyvsp[(1) - (2)].syntaxElement))); }
    break;

  case 65:
#line 343 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 66:
#line 346 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement))); }
    break;

  case 67:
#line 347 "grammar.y"
    { (yyval.syntaxElementList) = ((yyvsp[(3) - (3)].syntaxElementList)->push_front((yyvsp[(2) - (3)].syntaxElement))); }
    break;

  case 68:
#line 348 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement *>(); }
    break;

  case 69:
#line 351 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 70:
#line 352 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 71:
#line 353 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 72:
#line 354 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 73:
#line 355 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 74:
#line 356 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 75:
#line 357 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (2)].syntaxElement); }
    break;

  case 76:
#line 358 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (2)].syntaxElement); }
    break;

  case 77:
#line 359 "grammar.y"
    { (yyval.syntaxElement) = NULL; }
    break;

  case 78:
#line 362 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 79:
#line 363 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 80:
#line 366 "grammar.y"
    { (yyval.syntaxElementList) = new list<SyntaxElement*>((yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 81:
#line 367 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 82:
#line 368 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 83:
#line 371 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 84:
#line 372 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 85:
#line 376 "grammar.y"
    {
                PRINTF("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
                (yyval.syntaxElement) = new SyntaxClassDef((yyvsp[(2) - (7)].string), (yyvsp[(4) - (7)].string), (yyvsp[(6) - (7)].syntaxElementList));
                }
    break;

  case 86:
#line 382 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::IF, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 87:
#line 383 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::IF_ELSE, (yyvsp[(2) - (5)].syntaxElement), (yyvsp[(3) - (5)].syntaxElementList), (yyvsp[(5) - (5)].syntaxElementList)); }
    break;

  case 88:
#line 385 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxCondition((yyvsp[(2) - (3)].syntaxElement)); }
    break;

  case 89:
#line 388 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::FOR, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 90:
#line 391 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxForCondition(string((yyvsp[(2) - (5)].string)), (yyvsp[(4) - (5)].syntaxElement)); }
    break;

  case 91:
#line 394 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::WHILE, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 92:
#line 397 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::NEXT); }
    break;

  case 93:
#line 400 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::BREAK); }
    break;

  case 94:
#line 403 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::RETURN); }
    break;

  case 95:
#line 404 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::RETURN, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 96:
#line 407 "grammar.y"
    { (yyval.string) = new RbString((yyvsp[(1) - (1)].string)); }
    break;

  case 97:
#line 410 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant(new RbBool(false)); }
    break;

  case 98:
#line 411 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant(new RbBool(true)); }
    break;

  case 99:
#line 412 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant(NULL); }
    break;

  case 100:
#line 413 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant(new RbInt((yyvsp[(1) - (1)].intValue))); }
    break;

  case 101:
#line 414 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxConstant(new RbString((yyvsp[(1) - (1)].string))); }
    break;

  case 102:
#line 416 "grammar.y"
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
                    RbDouble *real = new RbDouble((yyvsp[(1) - (1)].realValue));
                    //real->setPrecision(prec);
                    //real->setScientific(sci);
                    (yyval.syntaxElement) = new SyntaxConstant(real);
                }
    break;


/* Line 1267 of yacc.c.  */
#line 2311 "grammar.tab.c"
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


#line 452 "grammar.y"



/* Definition of yyerror. */
int yyerror(char *msg)
{
    PRINTF("Bison code said: %s\n", msg);
    return 0;     // or return 1 ??
}

