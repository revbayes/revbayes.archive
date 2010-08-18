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
#define YYLSP_NEEDED 1



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
     ARROW_ASSIGN = 276,
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
     UNOT = 291,
     UPLUS = 292,
     UMINUS = 293,
     UAND = 294
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
#define ARROW_ASSIGN 276
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
#define UNOT 291
#define UPLUS 292
#define UMINUS 293
#define UAND 294




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

#include "Parser.h"
#include "PosReal.h"
#include "RbBool.h"
#include "RbDouble.h"
#include "RbInt.h"
#include "RbString.h"
#include "SyntaxElement.h"
#include "SyntaxAssignExpr.h"
#include "SyntaxBinaryExpr.h"
#include "SyntaxClassDef.h"
#include "SyntaxConstant.h"
#include "SyntaxForCondition.h"
#include "SyntaxFormal.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxFunctionDef.h"
#include "SyntaxLabeledExpr.h"
#include "SyntaxStatement.h"
#include "SyntaxUnaryExpr.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

/* Set flag for debugging info */
#if defined (DEBUG_PARSER)
#define YY_VERBOSE
#endif

extern int yylex(void);
extern char *yytext;

/* The function yyerror handles errors. It is defined below. */
int yyerror(const char *);

/* We use the global parser to execute the syntax tree */
Parser& parser = Parser::getParser();

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


/* struct holding source location for accurate error reports */
typedef struct yyltype
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} yyltype;



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
#line 90 "grammar.y"
{
    char*                           string;
    double                          realValue;
    int                             intValue;
    bool                            boolValue;
    RbString*                       idString;
    SyntaxElement*                  syntaxElement;
    SyntaxVariable*                 syntaxVariable;
    SyntaxFunctionCall*             syntaxFunctionCall;
    SyntaxLabeledExpr*              syntaxLabeledExpr;
    SyntaxFormal*                   syntaxFormal;
    std::list<SyntaxElement *>*     syntaxElementList;
    std::list<SyntaxLabeledExpr *>* argumentList;
    std::list<SyntaxFormal *>*      formalList;
}
/* Line 193 of yacc.c.  */
#line 278 "grammar.tab.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

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


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 303 "grammar.tab.cpp"

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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  68
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   507

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  122
/* YYNRULES -- Number of states.  */
#define YYNSTATES  204

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      50,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    53,     2,     2,     2,     2,    56,     2,
      48,    52,    40,    38,    55,    39,    46,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    42,    51,
       2,     2,     2,    36,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    49,     2,    54,    45,     2,     2,     2,     2,     2,
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
      35,    37,    43,    44,    47
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    16,    19,    22,
      26,    30,    34,    38,    40,    42,    44,    48,    51,    54,
      57,    60,    64,    68,    72,    76,    80,    84,    88,    92,
      96,   100,   104,   108,   112,   116,   120,   124,   126,   128,
     130,   132,   134,   138,   142,   146,   149,   154,   155,   157,
     161,   166,   171,   178,   179,   181,   183,   187,   189,   193,
     200,   208,   209,   211,   213,   217,   219,   223,   226,   231,
     235,   236,   238,   241,   245,   246,   248,   252,   254,   255,
     257,   261,   264,   268,   271,   273,   275,   277,   279,   281,
     283,   285,   287,   289,   291,   292,   294,   298,   301,   305,
     308,   310,   312,   320,   324,   330,   334,   338,   344,   348,
     350,   352,   354,   357,   359,   363,   367,   369,   371,   373,
     375,   377,   379
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      60,     0,    -1,    35,    -1,    50,    -1,    10,    50,    -1,
      83,    50,    -1,    83,    51,    -1,    84,    50,    -1,    84,
      51,    -1,    36,    96,    50,    -1,    36,    96,    51,    -1,
      36,    68,    50,    -1,    36,    68,    51,    -1,     1,    -1,
      99,    -1,    97,    -1,    48,    61,    52,    -1,    39,    61,
      -1,    38,    61,    -1,    53,    61,    -1,    25,    61,    -1,
      61,    42,    61,    -1,    61,    38,    61,    -1,    61,    39,
      61,    -1,    61,    40,    61,    -1,    61,    41,    61,    -1,
      61,    45,    61,    -1,    61,    31,    61,    -1,    61,    32,
      61,    -1,    61,    33,    61,    -1,    61,    34,    61,    -1,
      61,    30,    61,    -1,    61,    29,    61,    -1,    61,    25,
      61,    -1,    61,    26,    61,    -1,    61,    27,    61,    -1,
      61,    28,    61,    -1,    62,    -1,    64,    -1,    63,    -1,
      68,    -1,    65,    -1,    65,    21,    61,    -1,    65,    22,
      68,    -1,    65,    23,    61,    -1,    96,    66,    -1,    65,
      46,    96,    66,    -1,    -1,    67,    -1,    49,    61,    54,
      -1,    67,    49,    61,    54,    -1,    96,    48,    69,    52,
      -1,    65,    46,    96,    48,    69,    52,    -1,    -1,    70,
      -1,    71,    -1,    70,    55,    71,    -1,    61,    -1,    96,
      24,    61,    -1,    11,    96,    48,    73,    52,    80,    -1,
      11,    76,    96,    48,    73,    52,    80,    -1,    -1,    74,
      -1,    75,    -1,    74,    55,    75,    -1,    96,    -1,    96,
      24,    61,    -1,    76,    96,    -1,    76,    96,    24,    61,
      -1,    96,    77,    79,    -1,    -1,    78,    -1,    49,    54,
      -1,    78,    49,    54,    -1,    -1,    56,    -1,    57,    81,
      58,    -1,    83,    -1,    -1,    83,    -1,    81,    51,    83,
      -1,    81,    51,    -1,    81,    50,    83,    -1,    81,    50,
      -1,    88,    -1,    90,    -1,    92,    -1,    93,    -1,    94,
      -1,    95,    -1,    82,    -1,    61,    -1,    87,    -1,    72,
      -1,    -1,    86,    -1,    85,    51,    86,    -1,    85,    51,
      -1,    85,    50,    86,    -1,    85,    50,    -1,    75,    -1,
      72,    -1,    12,    96,    42,    96,    57,    85,    58,    -1,
      15,    89,    80,    -1,    15,    89,    80,    16,    80,    -1,
      48,    61,    52,    -1,    13,    91,    80,    -1,    48,    96,
      14,    61,    52,    -1,    17,    89,    80,    -1,    18,    -1,
      19,    -1,    20,    -1,    20,    61,    -1,     5,    -1,    49,
      98,    54,    -1,    98,    55,    61,    -1,    61,    -1,     8,
      -1,     9,    -1,     7,    -1,     4,    -1,     6,    -1,     3,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   218,   218,   223,   228,   233,   238,   243,   248,   253,
     258,   263,   268,   273,   281,   283,   285,   287,   288,   289,
     290,   292,   294,   295,   296,   297,   298,   300,   301,   302,
     303,   304,   305,   307,   308,   309,   310,   312,   313,   314,
     316,   318,   321,   328,   335,   342,   347,   354,   355,   358,
     359,   362,   367,   374,   375,   378,   379,   382,   387,   394,
     400,   407,   408,   411,   412,   415,   420,   425,   430,   437,
     440,   441,   444,   445,   448,   449,   452,   453,   461,   462,
     463,   464,   465,   466,   469,   470,   471,   472,   473,   474,
     477,   478,   481,   482,   485,   486,   487,   488,   489,   490,
     493,   494,   497,   504,   505,   507,   510,   513,   516,   519,
     522,   525,   526,   529,   533,   536,   537,   540,   545,   550,
     555,   560,   565
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "REAL", "INT", "NAME", "STRING",
  "RBNULL", "FALSE", "TRUE", "COMMENT", "FUNCTION", "CLASS", "FOR", "IN",
  "IF", "ELSE", "WHILE", "NEXT", "BREAK", "RETURN", "ARROW_ASSIGN",
  "TILDE_ASSIGN", "EQUATION_ASSIGN", "EQUAL", "AND", "OR", "AND2", "OR2",
  "GT", "GE", "LT", "LE", "EQ", "NE", "END_OF_INPUT", "'?'", "UNOT", "'+'",
  "'-'", "'*'", "'/'", "':'", "UPLUS", "UMINUS", "'^'", "'.'", "UAND",
  "'('", "'['", "'\\n'", "';'", "')'", "'!'", "']'", "','", "'&'", "'{'",
  "'}'", "$accept", "prog", "expression", "arrowAssign", "tildeAssign",
  "equationAssign", "variable", "optElements", "elementList",
  "functionCall", "optArguments", "argumentList", "argument",
  "functionDef", "optFormals", "formalList", "formal", "typeSpec",
  "optDims", "dimList", "optRef", "stmts", "stmtList", "statement",
  "stmt_or_expr", "declaration", "memberDefs", "memberDef", "classDef",
  "ifStatement", "cond", "forStatement", "forCond", "whileStatement",
  "nextStatement", "breakStatement", "returnStatement", "identifier",
  "vector", "vectorList", "constant", 0
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
     285,   286,   287,   288,   289,   290,    63,   291,    43,    45,
      42,    47,    58,   292,   293,    94,    46,   294,    40,    91,
      10,    59,    41,    33,    93,    44,    38,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    59,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    62,    63,    64,    65,    65,    66,    66,    67,
      67,    68,    68,    69,    69,    70,    70,    71,    71,    72,
      72,    73,    73,    74,    74,    75,    75,    75,    75,    76,
      77,    77,    78,    78,    79,    79,    80,    80,    81,    81,
      81,    81,    81,    81,    82,    82,    82,    82,    82,    82,
      83,    83,    84,    84,    85,    85,    85,    85,    85,    85,
      86,    86,    87,    88,    88,    89,    90,    91,    92,    93,
      94,    95,    95,    96,    97,    98,    98,    99,    99,    99,
      99,    99,    99
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     1,     1,     1,     3,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     3,     3,     3,     2,     4,     0,     1,     3,
       4,     4,     6,     0,     1,     1,     3,     1,     3,     6,
       7,     0,     1,     1,     3,     1,     3,     2,     4,     3,
       0,     1,     2,     3,     0,     1,     3,     1,     0,     1,
       3,     2,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     3,     2,     3,     2,
       1,     1,     7,     3,     5,     3,     3,     5,     3,     1,
       1,     1,     2,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    13,   122,   120,   113,   121,   119,   117,   118,     0,
       0,     0,     0,     0,     0,   109,   110,   111,     0,     2,
       0,     0,     0,     0,     0,     3,     0,     0,    91,    37,
      39,    38,    41,    40,    93,    90,     0,     0,    92,    84,
      85,    86,    87,    88,    89,    47,    15,    14,     4,     0,
      70,     0,     0,     0,     0,     0,     0,   112,    20,     0,
       0,    47,    18,    17,     0,   116,     0,    19,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     8,    53,     0,    45,    48,     0,    61,     0,
      74,    71,     0,     0,    78,   106,    77,     0,   103,   108,
      11,    12,     9,    10,    16,   114,     0,    33,    34,    35,
      36,    32,    31,    27,    28,    29,    30,    22,    23,    24,
      25,    21,    26,    42,    43,    44,    47,    57,     0,    54,
      55,    47,     0,     0,    61,     0,    62,    63,     0,    65,
      72,    75,    69,     0,     0,     0,     0,    79,   105,     0,
     115,    53,    46,    51,     0,     0,    49,     0,     0,     0,
       0,    67,     0,    73,    94,     0,    83,    81,    76,   104,
       0,    56,    58,    50,     0,    59,    64,     0,    66,   101,
     100,     0,    95,   107,    82,    80,    52,    60,    68,    99,
      97,   102,    98,    96
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    27,    28,    29,    30,    31,    32,    95,    96,    33,
     138,   139,   140,   189,   145,   146,   190,   148,   100,   101,
     152,   105,   156,    35,   106,    37,   191,   192,    38,    39,
      55,    40,    53,    41,    42,    43,    44,    45,    46,    66,
      47
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -101
static const yytype_int16 yypact[] =
{
     212,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,   -43,
      11,    11,    -4,    10,    10,  -101,  -101,   138,   138,  -101,
      11,   138,   138,   138,   138,  -101,   138,    40,   409,  -101,
    -101,  -101,    -8,  -101,  -101,  -101,   -27,   -22,  -101,  -101,
    -101,  -101,  -101,  -101,  -101,    20,  -101,  -101,  -101,    11,
      24,    22,    11,    28,   138,    28,    28,   409,  -101,    36,
       0,    47,    29,    29,   325,   409,    25,   445,  -101,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,    11,   138,    11,  -101,
    -101,  -101,  -101,   138,   138,  -101,    35,    53,    11,    48,
      51,    54,    11,    90,   263,  -101,  -101,   353,    89,  -101,
    -101,  -101,  -101,  -101,  -101,  -101,   138,   445,   428,   445,
     428,   462,   462,   462,   462,   462,   462,    49,    49,   -36,
     -36,    29,    29,   409,  -101,   409,    38,   409,    75,    73,
    -101,     6,   265,   138,    11,    77,    76,  -101,    11,     3,
    -101,  -101,  -101,    82,    80,   138,   -39,  -101,  -101,    28,
     409,   138,  -101,  -101,   138,   138,  -101,   295,    78,    28,
      11,   115,   138,  -101,    15,   381,   263,   263,  -101,  -101,
      88,  -101,   409,  -101,    28,  -101,  -101,   138,   409,  -101,
    -101,   -33,  -101,  -101,  -101,  -101,  -101,  -101,   409,    15,
      15,  -101,  -101,  -101
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -101,  -101,    39,  -101,  -101,  -101,   -16,    12,  -101,   -15,
     -12,  -101,   -11,   152,    13,  -101,   -95,   146,  -101,  -101,
    -101,   -34,  -101,  -101,     2,  -101,  -101,  -100,  -101,  -101,
     144,  -101,  -101,  -101,  -101,  -101,  -101,   -10,  -101,  -101,
    -101
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -71
static const yytype_int16 yytable[] =
{
      50,    51,    36,   147,    59,    60,    83,    48,   -70,    84,
      61,   176,   177,    85,    86,    87,     4,   199,   200,   178,
       4,   108,   109,    89,    90,   201,    10,   172,    91,    92,
     165,     2,     3,     4,     5,     6,     7,     8,    88,    97,
      68,    12,   103,    13,    52,    14,    15,    16,    17,   147,
     110,   111,    99,    18,    93,    94,    57,    58,    54,   -70,
      62,    63,    64,    65,   102,    67,    21,    22,    93,    94,
      59,   134,    98,    99,    84,   186,    23,    24,   136,   115,
     116,    26,    88,   141,   143,   104,   161,    94,   149,    81,
      82,    83,   154,   107,    84,    93,    94,   112,   113,   202,
     203,   144,   150,   153,   155,   159,   157,   151,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   179,   135,   163,   164,   169,
     184,   170,   137,   142,   149,   185,   173,   174,   171,   187,
     196,     2,     3,     4,     5,     6,     7,     8,   162,   180,
     197,   141,    34,   181,   141,   160,    49,   168,    56,     0,
     149,     0,     0,    18,   149,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,    22,   194,   195,
       0,     0,   167,     0,     0,     0,    23,    24,     0,   149,
     149,    26,     0,     0,   175,     0,     0,     0,     0,     0,
     137,     0,     0,   137,   182,     0,     0,     0,     0,     0,
       0,   188,     0,     1,     0,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,   198,    13,     0,    14,
      15,    16,    17,     0,     0,     0,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    19,    20,     0,
      21,    22,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    24,    25,     0,     0,    26,     2,     3,     4,     5,
       6,     7,     8,     0,     0,     0,    12,     0,    13,     0,
      14,    15,    16,    17,     0,     0,     0,     0,    18,     0,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    21,    22,    79,    80,    81,    82,    83,     0,     0,
      84,    23,    24,     0,     0,     0,    26,     0,     0,   166,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
       0,     0,     0,    79,    80,    81,    82,    83,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,   183,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
       0,     0,     0,    79,    80,    81,    82,    83,     0,     0,
      84,     0,     0,     0,     0,     0,     0,   114,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,     0,     0,
       0,    79,    80,    81,    82,    83,     0,     0,    84,     0,
       0,     0,     0,     0,     0,   158,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,     0,     0,     0,    79,
      80,    81,    82,    83,     0,     0,    84,     0,     0,     0,
       0,     0,     0,   193,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,     0,     0,     0,    79,    80,    81,
      82,    83,     0,    69,    84,    71,     0,    73,    74,    75,
      76,    77,    78,     0,     0,     0,    79,    80,    81,    82,
      83,     0,     0,    84,    73,    74,    75,    76,    77,    78,
       0,     0,     0,    79,    80,    81,    82,    83,     0,     0,
      84,   -71,   -71,   -71,   -71,   -71,   -71,     0,     0,     0,
      79,    80,    81,    82,    83,     0,     0,    84
};

static const yytype_int16 yycheck[] =
{
      10,    11,     0,    98,    20,    20,    42,    50,     5,    45,
      20,    50,    51,    21,    22,    23,     5,    50,    51,    58,
       5,    55,    56,    50,    51,    58,    11,    24,    50,    51,
      24,     3,     4,     5,     6,     7,     8,     9,    46,    49,
       0,    13,    52,    15,    48,    17,    18,    19,    20,   144,
      50,    51,    49,    25,    48,    49,    17,    18,    48,    56,
      21,    22,    23,    24,    42,    26,    38,    39,    48,    49,
      86,    86,    48,    49,    45,   170,    48,    49,    88,    54,
      55,    53,    46,    93,    49,    57,    48,    49,    98,    40,
      41,    42,   102,    54,    45,    48,    49,    50,    51,   199,
     200,    48,    54,    49,    14,    16,   104,    56,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,   159,    87,    52,    55,    52,
      52,    55,    93,    94,   144,   169,    54,    57,   148,    24,
      52,     3,     4,     5,     6,     7,     8,     9,   136,   161,
     184,   161,     0,   164,   164,   116,    10,   144,    14,    -1,
     170,    -1,    -1,    25,   174,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,   176,   177,
      -1,    -1,   143,    -1,    -1,    -1,    48,    49,    -1,   199,
     200,    53,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,   187,    15,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    -1,    53,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    13,    -1,    15,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    38,    39,    38,    39,    40,    41,    42,    -1,    -1,
      45,    48,    49,    -1,    -1,    -1,    53,    -1,    -1,    54,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    38,    39,    40,    41,    42,    -1,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    38,    39,    40,    41,    42,    -1,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    52,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    38,    39,    40,    41,    42,    -1,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    38,    39,    40,
      41,    42,    -1,    25,    45,    27,    -1,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    38,    39,    40,    41,
      42,    -1,    -1,    45,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    38,    39,    40,    41,    42,    -1,    -1,
      45,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    -1,    -1,    45
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    15,    17,    18,    19,    20,    25,    35,
      36,    38,    39,    48,    49,    50,    53,    60,    61,    62,
      63,    64,    65,    68,    72,    82,    83,    84,    87,    88,
      90,    92,    93,    94,    95,    96,    97,    99,    50,    76,
      96,    96,    48,    91,    48,    89,    89,    61,    61,    65,
      68,    96,    61,    61,    61,    61,    98,    61,     0,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    38,
      39,    40,    41,    42,    45,    21,    22,    23,    46,    50,
      51,    50,    51,    48,    49,    66,    67,    96,    48,    49,
      77,    78,    42,    96,    57,    80,    83,    61,    80,    80,
      50,    51,    50,    51,    52,    54,    55,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    68,    61,    96,    61,    69,    70,
      71,    96,    61,    49,    48,    73,    74,    75,    76,    96,
      54,    56,    79,    49,    96,    14,    81,    83,    52,    16,
      61,    48,    66,    52,    55,    24,    54,    61,    73,    52,
      55,    96,    24,    54,    57,    61,    50,    51,    58,    80,
      69,    71,    61,    54,    52,    80,    75,    24,    61,    72,
      75,    85,    86,    52,    83,    83,    52,    80,    61,    50,
      51,    58,    86,    86
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
		  Type, Value, Location); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
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
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 61: /* "expression" */
#line 143 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting expression  ...\n"); };
#line 1464 "grammar.tab.cpp"
	break;
      case 62: /* "arrowAssign" */
#line 144 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1469 "grammar.tab.cpp"
	break;
      case 63: /* "tildeAssign" */
#line 144 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1474 "grammar.tab.cpp"
	break;
      case 64: /* "equationAssign" */
#line 144 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1479 "grammar.tab.cpp"
	break;
      case 65: /* "variable" */
#line 142 "grammar.y"
	{ delete ((yyvaluep->syntaxVariable)); PRINTF("Deleting variable    ...\n"); };
#line 1484 "grammar.tab.cpp"
	break;
      case 66: /* "optElements" */
#line 138 "grammar.y"
	{ for (std::list<SyntaxElement*    >::iterator i=(yyvaluep->syntaxElementList)->begin(); i!=(yyvaluep->syntaxElementList)->end(); i++) delete (*i); delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1489 "grammar.tab.cpp"
	break;
      case 67: /* "elementList" */
#line 138 "grammar.y"
	{ for (std::list<SyntaxElement*    >::iterator i=(yyvaluep->syntaxElementList)->begin(); i!=(yyvaluep->syntaxElementList)->end(); i++) delete (*i); delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1494 "grammar.tab.cpp"
	break;
      case 68: /* "functionCall" */
#line 142 "grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); PRINTF("Deleting variable    ...\n"); };
#line 1499 "grammar.tab.cpp"
	break;
      case 69: /* "optArguments" */
#line 139 "grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator i=(yyvaluep->argumentList)->begin(); i!=(yyvaluep->argumentList)->end(); i++) delete (*i); delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1504 "grammar.tab.cpp"
	break;
      case 70: /* "argumentList" */
#line 139 "grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator i=(yyvaluep->argumentList)->begin(); i!=(yyvaluep->argumentList)->end(); i++) delete (*i); delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1509 "grammar.tab.cpp"
	break;
      case 71: /* "argument" */
#line 142 "grammar.y"
	{ delete ((yyvaluep->syntaxLabeledExpr)); PRINTF("Deleting variable    ...\n"); };
#line 1514 "grammar.tab.cpp"
	break;
      case 72: /* "functionDef" */
#line 146 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting functiondef ...\n"); };
#line 1519 "grammar.tab.cpp"
	break;
      case 73: /* "optFormals" */
#line 140 "grammar.y"
	{ for (std::list<SyntaxFormal*     >::iterator i=(yyvaluep->formalList)->begin(); i!=(yyvaluep->formalList)->end(); i++) delete (*i); delete ((yyvaluep->formalList)); PRINTF("Deleting formal list\n"); };
#line 1524 "grammar.tab.cpp"
	break;
      case 74: /* "formalList" */
#line 140 "grammar.y"
	{ for (std::list<SyntaxFormal*     >::iterator i=(yyvaluep->formalList)->begin(); i!=(yyvaluep->formalList)->end(); i++) delete (*i); delete ((yyvaluep->formalList)); PRINTF("Deleting formal list\n"); };
#line 1529 "grammar.tab.cpp"
	break;
      case 75: /* "formal" */
#line 142 "grammar.y"
	{ delete ((yyvaluep->syntaxFormal)); PRINTF("Deleting variable    ...\n"); };
#line 1534 "grammar.tab.cpp"
	break;
      case 76: /* "typeSpec" */
#line 141 "grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1539 "grammar.tab.cpp"
	break;
      case 77: /* "optDims" */
#line 141 "grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1544 "grammar.tab.cpp"
	break;
      case 78: /* "dimList" */
#line 141 "grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1549 "grammar.tab.cpp"
	break;
      case 79: /* "optRef" */
#line 141 "grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1554 "grammar.tab.cpp"
	break;
      case 80: /* "stmts" */
#line 138 "grammar.y"
	{ for (std::list<SyntaxElement*    >::iterator i=(yyvaluep->syntaxElementList)->begin(); i!=(yyvaluep->syntaxElementList)->end(); i++) delete (*i); delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1559 "grammar.tab.cpp"
	break;
      case 81: /* "stmtList" */
#line 138 "grammar.y"
	{ for (std::list<SyntaxElement*    >::iterator i=(yyvaluep->syntaxElementList)->begin(); i!=(yyvaluep->syntaxElementList)->end(); i++) delete (*i); delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1564 "grammar.tab.cpp"
	break;
      case 82: /* "statement" */
#line 143 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting expression  ...\n"); };
#line 1569 "grammar.tab.cpp"
	break;
      case 83: /* "stmt_or_expr" */
#line 143 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting expression  ...\n"); };
#line 1574 "grammar.tab.cpp"
	break;
      case 84: /* "declaration" */
#line 145 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting declaration ...\n"); };
#line 1579 "grammar.tab.cpp"
	break;
      case 85: /* "memberDefs" */
#line 138 "grammar.y"
	{ for (std::list<SyntaxElement*    >::iterator i=(yyvaluep->syntaxElementList)->begin(); i!=(yyvaluep->syntaxElementList)->end(); i++) delete (*i); delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1584 "grammar.tab.cpp"
	break;
      case 86: /* "memberDef" */
#line 145 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting declaration ...\n"); };
#line 1589 "grammar.tab.cpp"
	break;
      case 87: /* "classDef" */
#line 145 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting declaration ...\n"); };
#line 1594 "grammar.tab.cpp"
	break;
      case 88: /* "ifStatement" */
#line 147 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting for/if/while...\n"); };
#line 1599 "grammar.tab.cpp"
	break;
      case 89: /* "cond" */
#line 148 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting cond/return ...\n"); };
#line 1604 "grammar.tab.cpp"
	break;
      case 90: /* "forStatement" */
#line 147 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting for/if/while...\n"); };
#line 1609 "grammar.tab.cpp"
	break;
      case 91: /* "forCond" */
#line 148 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting cond/return ...\n"); };
#line 1614 "grammar.tab.cpp"
	break;
      case 92: /* "whileStatement" */
#line 147 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting for/if/while...\n"); };
#line 1619 "grammar.tab.cpp"
	break;
      case 93: /* "nextStatement" */
#line 149 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting next/break  ...\n"); };
#line 1624 "grammar.tab.cpp"
	break;
      case 94: /* "breakStatement" */
#line 149 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting next/break  ...\n"); };
#line 1629 "grammar.tab.cpp"
	break;
      case 95: /* "returnStatement" */
#line 148 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting cond/return ...\n"); };
#line 1634 "grammar.tab.cpp"
	break;
      case 96: /* "identifier" */
#line 141 "grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1639 "grammar.tab.cpp"
	break;
      case 97: /* "vector" */
#line 139 "grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator i=(yyvaluep->argumentList)->begin(); i!=(yyvaluep->argumentList)->end(); i++) delete (*i); delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1644 "grammar.tab.cpp"
	break;
      case 98: /* "vectorList" */
#line 139 "grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator i=(yyvaluep->argumentList)->begin(); i!=(yyvaluep->argumentList)->end(); i++) delete (*i); delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1649 "grammar.tab.cpp"
	break;
      case 99: /* "constant" */
#line 142 "grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting variable    ...\n"); };
#line 1654 "grammar.tab.cpp"
	break;

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
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;



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

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

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
  yylsp = yyls;
#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 219 "grammar.y"
    {
                    PRINTF("Bison encountered end_of_input; ignored\n");
                    return 0;
                }
    break;

  case 3:
#line 224 "grammar.y"
    {
                    PRINTF("Bison encountered newline; ignored\n");
                    return 0;
                }
    break;

  case 4:
#line 229 "grammar.y"
    {
                    PRINTF("Bison encountered comment; ignored\n");
                    return 0;
                }
    break;

  case 5:
#line 234 "grammar.y"
    {
                    PRINTF("Bison trying to execute statement or expression\n");
                    return parser.execute((yyvsp[(1) - (2)].syntaxElement));
                }
    break;

  case 6:
#line 239 "grammar.y"
    {
                    PRINTF("Bison trying to execute statement or expression\n");
                    return parser.execute((yyvsp[(1) - (2)].syntaxElement));
                }
    break;

  case 7:
#line 244 "grammar.y"
    {
                    PRINTF("Bison trying to execute declaration\n");
                    return parser.execute((yyvsp[(1) - (2)].syntaxElement));
                }
    break;

  case 8:
#line 249 "grammar.y"
    {
                    PRINTF("Bison trying to execute declaration\n");
                    return parser.execute((yyvsp[(1) - (2)].syntaxElement));
                }
    break;

  case 9:
#line 254 "grammar.y"
    {
                    PRINTF("Bison trying to get help for symbol\n");
                    return parser.help((yyvsp[(2) - (3)].idString));
                }
    break;

  case 10:
#line 259 "grammar.y"
    {
                    PRINTF("Bison trying to get help for symbol\n");
                    return parser.help((yyvsp[(2) - (3)].idString));
                }
    break;

  case 11:
#line 264 "grammar.y"
    {
                    PRINTF("Bison trying to get help for function call\n");
                    return parser.help((yyvsp[(2) - (3)].syntaxFunctionCall));
                }
    break;

  case 12:
#line 269 "grammar.y"
    {
                    PRINTF("Bison trying to get help for function call\n");
                    return parser.help((yyvsp[(2) - (3)].syntaxFunctionCall));
                }
    break;

  case 13:
#line 274 "grammar.y"
    {
                    PRINTF("Bison calling YYABORT\n");
                    PRINTF("Error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
                    YYABORT;
                }
    break;

  case 14:
#line 281 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 15:
#line 283 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxFunctionCall(new RbString("v"), (yyvsp[(1) - (1)].argumentList)); }
    break;

  case 16:
#line 285 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 17:
#line 287 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UMinus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 18:
#line 288 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UPlus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 19:
#line 289 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UNot, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 20:
#line 290 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UAnd, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 21:
#line 292 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Range, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 22:
#line 294 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Add, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 23:
#line 295 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Sub, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 24:
#line 296 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Mul, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 25:
#line 297 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Div, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 26:
#line 298 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Exp, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 27:
#line 300 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Lt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 28:
#line 301 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Le, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 29:
#line 302 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Eq, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 30:
#line 303 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ne, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 31:
#line 304 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ge, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 32:
#line 305 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Gt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 33:
#line 307 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 34:
#line 308 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 35:
#line 309 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 36:
#line 310 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 37:
#line 312 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 38:
#line 313 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 39:
#line 314 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 40:
#line 316 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFunctionCall); }
    break;

  case 41:
#line 318 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxVariable); }
    break;

  case 42:
#line 322 "grammar.y"
    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::ArrowAssign, (yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 43:
#line 329 "grammar.y"
    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::TildeAssign, (yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxFunctionCall));
                    }
    break;

  case 44:
#line 336 "grammar.y"
    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::EquationAssign, (yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 45:
#line 343 "grammar.y"
    {
                    PRINTF("Parser inserting variable (VARIABLE) in syntax tree\n");
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (2)].idString), (yyvsp[(2) - (2)].syntaxElementList));
                }
    break;

  case 46:
#line 348 "grammar.y"
    {
                    PRINTF("Parser inserting member variable (MEMBER) in syntax tree\n");
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (4)].syntaxVariable), (yyvsp[(3) - (4)].idString), (yyvsp[(4) - (4)].syntaxElementList));
                }
    break;

  case 47:
#line 354 "grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 48:
#line 355 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (1)].syntaxElementList); }
    break;

  case 49:
#line 358 "grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(2) - (3)].syntaxElement)); }
    break;

  case 50:
#line 359 "grammar.y"
    { (yyvsp[(1) - (4)].syntaxElementList)->push_back((yyvsp[(3) - (4)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (4)].syntaxElementList); }
    break;

  case 51:
#line 363 "grammar.y"
    {
                        PRINTF("Parser inserting function call in syntax tree\n");
                        (yyval.syntaxFunctionCall) = new SyntaxFunctionCall((yyvsp[(1) - (4)].idString), (yyvsp[(3) - (4)].argumentList));
                    }
    break;

  case 52:
#line 368 "grammar.y"
    {
                        PRINTF("Parser inserting member function call in syntax tree\n");
                        (yyval.syntaxFunctionCall) = new SyntaxFunctionCall((yyvsp[(1) - (6)].syntaxVariable), (yyvsp[(3) - (6)].idString), (yyvsp[(5) - (6)].argumentList));
                    }
    break;

  case 53:
#line 374 "grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr *>(); }
    break;

  case 54:
#line 375 "grammar.y"
    { (yyval.argumentList) = (yyvsp[(1) - (1)].argumentList); }
    break;

  case 55:
#line 378 "grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr *>(1,(yyvsp[(1) - (1)].syntaxLabeledExpr)); }
    break;

  case 56:
#line 379 "grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back((yyvsp[(3) - (3)].syntaxLabeledExpr)); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 57:
#line 383 "grammar.y"
    {
                    PRINTF("Parser inserting unlabeled argument in syntax tree\n");
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr(new RbString(""), (yyvsp[(1) - (1)].syntaxElement));
                }
    break;

  case 58:
#line 388 "grammar.y"
    { 
                    PRINTF("Parser inserting labeled argument in syntax tree\n");
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr((yyvsp[(1) - (3)].idString), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 59:
#line 395 "grammar.y"
    {
                PRINTF("Parser inserting function definition in syntax tree\n");
                (yyval.syntaxElement) = new SyntaxFunctionDef(NULL, (yyvsp[(2) - (6)].idString), (yyvsp[(4) - (6)].formalList), (yyvsp[(6) - (6)].syntaxElementList));
                }
    break;

  case 60:
#line 401 "grammar.y"
    {
                PRINTF("Parser inserting typed function definition in syntax tree\n");
                (yyval.syntaxElement) = new SyntaxFunctionDef((yyvsp[(2) - (7)].idString), (yyvsp[(3) - (7)].idString), (yyvsp[(5) - (7)].formalList), (yyvsp[(7) - (7)].syntaxElementList));
                }
    break;

  case 61:
#line 407 "grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal *>(); }
    break;

  case 62:
#line 408 "grammar.y"
    { (yyval.formalList) = (yyvsp[(1) - (1)].formalList); }
    break;

  case 63:
#line 411 "grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal *>(1,(yyvsp[(1) - (1)].syntaxFormal)); }
    break;

  case 64:
#line 412 "grammar.y"
    { (yyvsp[(1) - (3)].formalList)->push_back((yyvsp[(3) - (3)].syntaxFormal)); (yyval.formalList) = (yyvsp[(1) - (3)].formalList); }
    break;

  case 65:
#line 416 "grammar.y"
    {
                    PRINTF("Inserting labeled formal argument without default in syntax tree\n");
                    (yyval.syntaxFormal) = new SyntaxFormal((yyvsp[(1) - (1)].idString), NULL);
                }
    break;

  case 66:
#line 421 "grammar.y"
    {
                    PRINTF("Inserting labeled formal argument with default in syntax tree\n");
                    (yyval.syntaxFormal) = new SyntaxFormal((yyvsp[(1) - (3)].idString), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 67:
#line 426 "grammar.y"
    {
                    PRINTF("Inserting typed labeled formal argument without default in syntax tree\n");
                    (yyval.syntaxFormal) = new SyntaxFormal((yyvsp[(1) - (2)].idString), (yyvsp[(2) - (2)].idString), NULL);
                }
    break;

  case 68:
#line 431 "grammar.y"
    {
                    PRINTF("Inserting typed labeled formal argument with default in syntax tree\n");
                    (yyval.syntaxFormal) = new SyntaxFormal((yyvsp[(1) - (4)].idString), (yyvsp[(2) - (4)].idString), (yyvsp[(4) - (4)].syntaxElement));
                }
    break;

  case 69:
#line 437 "grammar.y"
    { (yyvsp[(1) - (3)].idString)->append(*((yyvsp[(2) - (3)].idString))); delete((yyvsp[(2) - (3)].idString)); (yyvsp[(1) - (3)].idString)->append(*((yyvsp[(3) - (3)].idString))); delete((yyvsp[(3) - (3)].idString)); (yyval.idString) = (yyvsp[(1) - (3)].idString); }
    break;

  case 70:
#line 440 "grammar.y"
    { (yyval.idString) = new RbString(); }
    break;

  case 71:
#line 441 "grammar.y"
    { (yyval.idString) = (yyvsp[(1) - (1)].idString); }
    break;

  case 72:
#line 444 "grammar.y"
    { (yyval.idString) = new RbString("[]"); }
    break;

  case 73:
#line 445 "grammar.y"
    { (yyvsp[(1) - (3)].idString)->append("[]"); (yyval.idString) = (yyvsp[(1) - (3)].idString); }
    break;

  case 74:
#line 448 "grammar.y"
    { (yyval.idString) = new RbString(); }
    break;

  case 75:
#line 449 "grammar.y"
    { (yyval.idString) = new RbString("&"); }
    break;

  case 76:
#line 452 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 77:
#line 454 "grammar.y"
    {
                    std::list<SyntaxElement*> stmts;
                    stmts.push_back((yyvsp[(1) - (1)].syntaxElement));
                    (yyval.syntaxElementList) = &stmts;
                }
    break;

  case 78:
#line 461 "grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 79:
#line 462 "grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 80:
#line 463 "grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 81:
#line 464 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 82:
#line 465 "grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 83:
#line 466 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 84:
#line 469 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 85:
#line 470 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 86:
#line 471 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 87:
#line 472 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 88:
#line 473 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 89:
#line 474 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 90:
#line 477 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 91:
#line 478 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 92:
#line 481 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 93:
#line 482 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 94:
#line 485 "grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 95:
#line 486 "grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 96:
#line 487 "grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 97:
#line 488 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 98:
#line 489 "grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 99:
#line 490 "grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 100:
#line 493 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFormal); }
    break;

  case 101:
#line 494 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 102:
#line 498 "grammar.y"
    {
                PRINTF("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
                (yyval.syntaxElement) = new SyntaxClassDef((yyvsp[(2) - (7)].idString), (yyvsp[(4) - (7)].idString), (yyvsp[(6) - (7)].syntaxElementList));
                }
    break;

  case 103:
#line 504 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::If, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 104:
#line 505 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::IfElse, (yyvsp[(2) - (5)].syntaxElement), (yyvsp[(3) - (5)].syntaxElementList), (yyvsp[(5) - (5)].syntaxElementList)); }
    break;

  case 105:
#line 507 "grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 106:
#line 510 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::For, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 107:
#line 513 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxForCondition((yyvsp[(2) - (5)].idString), (yyvsp[(4) - (5)].syntaxElement)); }
    break;

  case 108:
#line 516 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::While, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 109:
#line 519 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Next); }
    break;

  case 110:
#line 522 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Break); }
    break;

  case 111:
#line 525 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return); }
    break;

  case 112:
#line 526 "grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 113:
#line 529 "grammar.y"
    { (yyval.idString) = new RbString((yyvsp[(1) - (1)].string)); }
    break;

  case 114:
#line 533 "grammar.y"
    { (yyval.argumentList) = (yyvsp[(2) - (3)].argumentList); }
    break;

  case 115:
#line 536 "grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back(new SyntaxLabeledExpr(new RbString(""), (yyvsp[(3) - (3)].syntaxElement))); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 116:
#line 537 "grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(1, new SyntaxLabeledExpr(new RbString(""), (yyvsp[(1) - (1)].syntaxElement))); }
    break;

  case 117:
#line 541 "grammar.y"
    {
                    PRINTF("Parser inserting bool constant (false) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxConstant(new RbBool(false));
                }
    break;

  case 118:
#line 546 "grammar.y"
    {
                    PRINTF("Parser inserting bool constant (true) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxConstant(new RbBool(true));
                }
    break;

  case 119:
#line 551 "grammar.y"
    {
                    PRINTF("Parser inserting null constant in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxConstant(NULL);
                }
    break;

  case 120:
#line 556 "grammar.y"
    {
                    PRINTF("Parser inserting int constant in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxConstant(new RbInt((yyvsp[(1) - (1)].intValue)));
                }
    break;

  case 121:
#line 561 "grammar.y"
    {
                    PRINTF("Parser inserting string constant in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxConstant(new RbString((yyvsp[(1) - (1)].string)));
                }
    break;

  case 122:
#line 566 "grammar.y"
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
                        prec = (int)(strlen(yytext)) - 1 - i;
                    }
                    RbDouble* real;
                    if ((yyvsp[(1) - (1)].realValue) > 0.0)
                        real = new PosReal((yyvsp[(1) - (1)].realValue));
                    else
                        real = new RbDouble((yyvsp[(1) - (1)].realValue));
                    //real->setPrecision(prec);
                    //real->setScientific(sci);
                    PRINTF("Parser inserting double constant in syntax tree \n");
                    (yyval.syntaxElement) = new SyntaxConstant(real);
                }
    break;


/* Line 1267 of yacc.c.  */
#line 2725 "grammar.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[0] = yylsp[1-yylen];
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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


#line 607 "grammar.y"



/* Definition of yyerror. */
int yyerror(const char *msg)
{
    PRINTF("Bison code said: %s\n", msg);
    return 1;
}

