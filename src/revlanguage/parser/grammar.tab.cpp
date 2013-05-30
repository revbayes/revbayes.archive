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
     EQUAL = 278,
     AND = 279,
     OR = 280,
     AND2 = 281,
     OR2 = 282,
     GT = 283,
     GE = 284,
     LT = 285,
     LE = 286,
     EQ = 287,
     NE = 288,
     END_OF_INPUT = 289,
     UNOT = 290,
     UPLUS = 291,
     UMINUS = 292,
     UAND = 293
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
#define EQUAL 278
#define AND 279
#define OR 280
#define AND2 281
#define OR2 282
#define GT 283
#define GE 284
#define LT 285
#define LE 286
#define EQ 287
#define NE 288
#define END_OF_INPUT 289
#define UNOT 290
#define UPLUS 291
#define UMINUS 292
#define UAND 293




/* Copy the first part of user declarations.  */
#line 1 "./grammar.y"

/**
 * @file
 * Grammar specification in bison format for RevBayes, a computing environment 
 * for evolutionary analysis, particularly Bayesian phylogenetic inference. The
 * language used by RevBayes is referred to as Rev.
 *
 * The grammar borrows heavily from the R grammar specification in the gram.y 
 * file of the R source code but deviates significantly in many respects, being
 * more similar to object-oriented languages like C++ or Java. The model description
 * syntax is inspired by the language used originally by BUGS to describe complex 
 * stochastic models. Unlike BUGS and similar programs, such as JAGS, RevBayes
 * allows models to be built in an interpreted (interactive) environment.
 *
 * @brief Grammar specification in bison format
 *
 * @author Fredrik Ronquist and Sebastian Hoehna
 * @date Last modified: $Date$
 *
 * $Id$
 */

/* The following statements go into the resulting C code */

#include "Environment.h"
#include "Integer.h"
#include "Natural.h"
#include "Parser.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "Real.h"
#include "RealPos.h"
#include "SyntaxElement.h"
#include "SyntaxConstantAssignment.h"
#include "SyntaxDeterministicAssignment.h"
#include "SyntaxStochasticAssignment.h"
#include "SyntaxBinaryExpr.h"
#include "SyntaxClassDef.h"
#include "SyntaxConstant.h"
#include "SyntaxVariableDecl.h"
#include "SyntaxForLoop.h"
#include "SyntaxFormal.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxFunctionDef.h"
#include "SyntaxLabeledExpr.h"
#include "SyntaxStatement.h"
#include "SyntaxUnaryExpr.h"
#include "SyntaxVariable.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>
#include <string>

using namespace RevLanguage;

extern int yylex(void);
extern char *yytext;
extern Environment *executionEnvironment;

/* The function yyerror handles errors. It is defined below. */
int yyerror(const char *);

/* We use the global parser to execute the syntax tree */
Parser& parser = Parser::getParser();


#define YY_NEVER_INTERACTIVE


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
#line 73 "./grammar.y"
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
/* Line 193 of yacc.c.  */
#line 259 "./grammar.tab.c"
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
#line 284 "./grammar.tab.c"

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
#define YYFINAL  70
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   656

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  133
/* YYNRULES -- Number of states.  */
#define YYNSTATES  227

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
       2,     2,     2,    52,     2,     2,     2,     2,    55,     2,
      47,    51,    39,    37,    54,    38,    45,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    41,    50,
       2,     2,     2,    35,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    48,     2,    53,    44,     2,     2,     2,     2,     2,
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
      36,    42,    43,    46
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    16,    19,    23,
      27,    31,    35,    38,    41,    43,    45,    49,    52,    55,
      58,    61,    65,    69,    73,    77,    81,    85,    89,    93,
      97,   101,   105,   109,   113,   117,   121,   125,   127,   129,
     131,   133,   135,   139,   143,   147,   151,   155,   159,   162,
     168,   173,   181,   182,   184,   188,   191,   196,   200,   205,
     207,   211,   212,   214,   216,   220,   222,   226,   233,   241,
     242,   244,   246,   250,   252,   256,   259,   264,   268,   269,
     271,   274,   278,   282,   287,   288,   290,   294,   296,   297,
     299,   303,   306,   310,   313,   315,   317,   319,   321,   323,
     325,   327,   329,   331,   333,   338,   339,   341,   345,   348,
     352,   355,   357,   359,   367,   371,   377,   381,   385,   391,
     395,   397,   399,   401,   404,   406,   410,   414,   416,   418,
     420,   422,   424,   426
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      59,     0,    -1,    34,    -1,    49,    -1,    83,    49,    -1,
      83,    50,    -1,    84,    49,    -1,    84,    50,    -1,    35,
      96,    49,    -1,    35,    96,    50,    -1,    35,    68,    49,
      -1,    35,    68,    50,    -1,     1,    49,    -1,     1,    50,
      -1,    99,    -1,    97,    -1,    47,    60,    51,    -1,    38,
      60,    -1,    37,    60,    -1,    52,    60,    -1,    24,    60,
      -1,    60,    41,    60,    -1,    60,    37,    60,    -1,    60,
      38,    60,    -1,    60,    39,    60,    -1,    60,    40,    60,
      -1,    60,    44,    60,    -1,    60,    30,    60,    -1,    60,
      31,    60,    -1,    60,    32,    60,    -1,    60,    33,    60,
      -1,    60,    29,    60,    -1,    60,    28,    60,    -1,    60,
      24,    60,    -1,    60,    25,    60,    -1,    60,    26,    60,
      -1,    60,    27,    60,    -1,    61,    -1,    63,    -1,    62,
      -1,    68,    -1,    64,    -1,    64,    20,    60,    -1,    68,
      20,    60,    -1,    64,    21,    68,    -1,    68,    21,    68,
      -1,    64,    22,    60,    -1,    68,    22,    60,    -1,    96,
      65,    -1,    67,    48,    60,    53,    65,    -1,    64,    45,
      96,    65,    -1,    64,    45,    67,    48,    60,    53,    65,
      -1,    -1,    66,    -1,    48,    60,    53,    -1,    48,    53,
      -1,    66,    48,    60,    53,    -1,    66,    48,    53,    -1,
      96,    47,    69,    51,    -1,    67,    -1,    64,    45,    67,
      -1,    -1,    70,    -1,    71,    -1,    70,    54,    71,    -1,
      60,    -1,    96,    23,    60,    -1,    10,    96,    47,    73,
      51,    80,    -1,    10,    76,    96,    47,    73,    51,    80,
      -1,    -1,    74,    -1,    75,    -1,    74,    54,    75,    -1,
      96,    -1,    96,    23,    60,    -1,    76,    96,    -1,    76,
      96,    23,    60,    -1,    96,    77,    79,    -1,    -1,    78,
      -1,    48,    53,    -1,    48,     4,    53,    -1,    78,    48,
      53,    -1,    78,    48,     4,    53,    -1,    -1,    55,    -1,
      56,    81,    57,    -1,    83,    -1,    -1,    83,    -1,    81,
      50,    83,    -1,    81,    50,    -1,    81,    49,    83,    -1,
      81,    49,    -1,    88,    -1,    90,    -1,    92,    -1,    93,
      -1,    94,    -1,    95,    -1,    82,    -1,    60,    -1,    87,
      -1,    72,    -1,    96,    65,    79,    96,    -1,    -1,    86,
      -1,    85,    50,    86,    -1,    85,    50,    -1,    85,    49,
      86,    -1,    85,    49,    -1,    75,    -1,    72,    -1,    11,
      96,    41,    96,    56,    85,    57,    -1,    14,    89,    80,
      -1,    14,    89,    80,    15,    80,    -1,    47,    60,    51,
      -1,    12,    91,    80,    -1,    47,    96,    13,    60,    51,
      -1,    16,    89,    80,    -1,    17,    -1,    18,    -1,    19,
      -1,    19,    60,    -1,     5,    -1,    48,    98,    53,    -1,
      98,    54,    60,    -1,    60,    -1,     8,    -1,     9,    -1,
       7,    -1,     4,    -1,     6,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   200,   200,   207,   214,   222,   230,   238,   246,   255,
     264,   273,   282,   289,   298,   300,   302,   304,   305,   306,
     307,   309,   311,   312,   313,   314,   315,   317,   318,   319,
     320,   321,   322,   324,   325,   326,   327,   329,   330,   331,
     333,   335,   338,   345,   354,   361,   370,   377,   386,   394,
     402,   410,   420,   421,   424,   425,   426,   427,   430,   437,
     444,   454,   455,   458,   459,   462,   469,   479,   488,   499,
     500,   503,   504,   507,   515,   523,   532,   543,   546,   547,
     550,   551,   558,   559,   569,   570,   573,   574,   582,   583,
     584,   585,   586,   587,   590,   591,   592,   593,   594,   595,
     598,   599,   602,   603,   604,   616,   617,   618,   619,   620,
     621,   624,   625,   628,   639,   640,   642,   645,   648,   651,
     654,   657,   660,   661,   664,   668,   671,   672,   678,   685,
     692,   699,   714,   721
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "REAL", "INT", "NAME", "STRING",
  "RBNULL", "FALSE", "TRUE", "FUNCTION", "CLASS", "FOR", "IN", "IF",
  "ELSE", "WHILE", "NEXT", "BREAK", "RETURN", "ARROW_ASSIGN",
  "TILDE_ASSIGN", "EQUATION_ASSIGN", "EQUAL", "AND", "OR", "AND2", "OR2",
  "GT", "GE", "LT", "LE", "EQ", "NE", "END_OF_INPUT", "'?'", "UNOT", "'+'",
  "'-'", "'*'", "'/'", "':'", "UPLUS", "UMINUS", "'^'", "'.'", "UAND",
  "'('", "'['", "'\\n'", "';'", "')'", "'!'", "']'", "','", "'&'", "'{'",
  "'}'", "$accept", "prog", "expression", "arrowAssign", "tildeAssign",
  "equationAssign", "variable", "optElements", "elementList", "fxnCall",
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
     285,   286,   287,   288,   289,    63,   290,    43,    45,    42,
      47,    58,   291,   292,    94,    46,   293,    40,    91,    10,
      59,    41,    33,    93,    44,    38,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    61,    62,    62,    63,    63,    64,    64,
      64,    64,    65,    65,    66,    66,    66,    66,    67,    68,
      68,    69,    69,    70,    70,    71,    71,    72,    72,    73,
      73,    74,    74,    75,    75,    75,    75,    76,    77,    77,
      78,    78,    78,    78,    79,    79,    80,    80,    81,    81,
      81,    81,    81,    81,    82,    82,    82,    82,    82,    82,
      83,    83,    84,    84,    84,    85,    85,    85,    85,    85,
      85,    86,    86,    87,    88,    88,    89,    90,    91,    92,
      93,    94,    95,    95,    96,    97,    98,    98,    99,    99,
      99,    99,    99,    99
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     2,     2,     1,     1,     3,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     2,     5,
       4,     7,     0,     1,     3,     2,     4,     3,     4,     1,
       3,     0,     1,     1,     3,     1,     3,     6,     7,     0,
       1,     1,     3,     1,     3,     2,     4,     3,     0,     1,
       2,     3,     3,     4,     0,     1,     3,     1,     0,     1,
       3,     2,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     0,     1,     3,     2,     3,
       2,     1,     1,     7,     3,     5,     3,     3,     5,     3,
       1,     1,     1,     2,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   133,   131,   124,   132,   130,   128,   129,     0,
       0,     0,     0,     0,   120,   121,   122,     0,     2,     0,
       0,     0,     0,     0,     3,     0,     0,   101,    37,    39,
      38,    41,    59,    40,   103,   100,     0,     0,   102,    94,
      95,    96,    97,    98,    99,    52,    15,    14,    12,    13,
       0,    78,     0,     0,     0,     0,     0,     0,   123,    52,
      20,     0,     0,    52,    18,    17,     0,   127,     0,    19,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,    61,
       0,    48,    53,     0,    69,     0,    84,    79,     0,     0,
      88,   117,    87,     0,   114,   119,    48,    10,    11,     8,
       9,    16,   125,     0,    33,    34,    35,    36,    32,    31,
      27,    28,    29,    30,    22,    23,    24,    25,    21,    26,
      42,    44,    46,    60,    52,     0,    43,    45,    47,    65,
       0,    62,    63,    52,    55,     0,    85,     0,     0,    69,
       0,    70,    71,     0,    73,     0,    80,    77,     0,     0,
       0,     0,    89,   116,     0,   126,     0,    50,    52,    58,
       0,     0,    54,   104,    57,     0,     0,     0,     0,    75,
       0,    81,     0,    82,   105,     0,    93,    91,    86,   115,
       0,    49,    64,    66,    56,     0,    67,    72,     0,    74,
      83,   112,   111,     0,   106,   118,    92,    90,    52,    68,
      76,   110,   108,   113,    51,   109,   107
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,    27,    28,    29,    30,    31,   116,   102,    32,
      33,   150,   151,   152,   211,   160,   161,   212,   163,   106,
     107,   157,   111,   171,    35,   112,    37,   213,   214,    38,
      39,    56,    40,    54,    41,    42,    43,    44,    59,    46,
      68,    47
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -103
static const yytype_int16 yypact[] =
{
     231,   -38,  -103,  -103,  -103,  -103,  -103,  -103,  -103,    17,
      17,   -40,   -12,   -12,  -103,  -103,   326,   326,  -103,    17,
     326,   326,   326,   326,  -103,   326,    37,   559,  -103,  -103,
    -103,    25,    -4,   118,  -103,  -103,   -25,    51,  -103,  -103,
    -103,  -103,  -103,  -103,  -103,    62,  -103,  -103,  -103,  -103,
      17,    65,     8,    17,    24,   326,    24,    24,   559,    62,
    -103,     7,    96,    46,    10,    10,   475,   559,    94,   595,
    -103,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,    17,   326,
      17,   326,   326,    17,   326,  -103,  -103,  -103,  -103,   326,
     178,    12,    27,     9,    17,     2,    22,    49,    17,    69,
     304,  -103,  -103,   503,    70,  -103,  -103,  -103,  -103,  -103,
    -103,  -103,  -103,   326,   595,   578,   595,   578,   612,   612,
     612,   612,   612,   612,    48,    48,   -23,   -23,    10,    10,
     559,  -103,   559,    55,    62,   355,   559,  -103,   559,   559,
      56,    79,  -103,    58,  -103,   385,  -103,    17,   253,    17,
      90,    97,  -103,    17,     3,    99,  -103,  -103,    16,    98,
     326,   -34,  -103,  -103,    24,   559,   326,  -103,   105,  -103,
     326,   326,  -103,  -103,  -103,   415,   104,    24,    17,   133,
     326,  -103,   107,  -103,    29,   531,   304,   304,  -103,  -103,
     445,  -103,  -103,   559,  -103,    24,  -103,  -103,   326,   559,
    -103,  -103,  -103,    34,  -103,  -103,  -103,  -103,   105,  -103,
     559,    29,    29,  -103,  -103,  -103,  -103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -103,  -103,    43,  -103,  -103,  -103,   -15,   -42,  -103,    68,
     -14,  -103,  -103,   -19,   164,     6,  -103,  -102,   158,  -103,
    -103,    63,   -43,  -103,  -103,     1,  -103,  -103,   -72,  -103,
    -103,   155,  -103,  -103,  -103,  -103,  -103,  -103,     0,  -103,
    -103,  -103
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -85
static const yytype_int16 yytable[] =
{
      45,    36,   162,   101,    61,    62,   165,    53,   -78,    51,
      52,    48,    49,   114,   115,   196,   197,   -84,    85,    63,
     192,    86,     4,   198,    95,    96,   190,     2,     3,     4,
       5,     6,     7,     8,     4,    55,    11,    70,    12,     9,
      13,    14,    15,    16,    91,    87,    88,    89,    17,   108,
     103,   105,    90,   109,    86,   166,   159,   162,   -78,    58,
      60,    20,    21,    64,    65,    66,    67,   156,    69,   193,
      90,    22,    23,    61,   141,   158,    25,   156,    61,   147,
     110,   181,   170,   221,   222,   174,   207,    83,    84,    85,
     144,   223,    86,    99,   100,   119,   120,   168,   113,   153,
      97,    98,   177,   176,   164,    99,   100,   179,   169,    99,
     100,   172,   104,   105,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   199,   142,   180,   145,   146,   201,   148,    92,    93,
      94,   187,   149,   155,   206,   117,   118,   122,   123,   225,
     226,   188,   191,   100,   194,   205,   208,   183,   143,   164,
     210,   202,   219,   189,    34,   186,   175,    50,    57,   167,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
     153,     2,     3,     4,     5,     6,     7,     8,   164,     0,
       0,     0,     0,     0,   164,     0,     0,   216,   217,     0,
       0,   185,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   195,     0,    20,    21,     0,     0,   200,
       0,   164,   164,   149,   203,    22,    23,     0,     0,     0,
      25,   154,     1,   209,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,     0,    12,     0,    13,    14,    15,
      16,   220,     0,     0,     0,    17,     2,     3,     4,     5,
       6,     7,     8,     0,     0,    18,    19,     0,    20,    21,
       0,     0,     0,     0,     0,     0,     0,    17,    22,    23,
      24,     0,     0,    25,     0,     0,     0,     0,     0,     0,
      20,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      22,    23,     0,     0,     0,    25,   184,     2,     3,     4,
       5,     6,     7,     8,     0,     0,    11,     0,    12,     0,
      13,    14,    15,    16,     0,     0,     0,     0,    17,     2,
       3,     4,     5,     6,     7,     8,     0,     0,     0,     0,
       0,    20,    21,     0,     0,     0,     0,     0,     0,     0,
      17,    22,    23,     0,     0,     0,    25,     0,     0,     0,
       0,     0,     0,    20,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    22,    23,     0,     0,     0,    25,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,     0,
       0,     0,    81,    82,    83,    84,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,   178,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,     0,
       0,     0,    81,    82,    83,    84,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,   182,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,     0,
       0,     0,    81,    82,    83,    84,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,   204,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,     0,
       0,     0,    81,    82,    83,    84,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,   218,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,     0,
       0,     0,    81,    82,    83,    84,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,   121,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,     0,     0,     0,
      81,    82,    83,    84,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,   173,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,     0,     0,     0,    81,    82,
      83,    84,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,   215,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,     0,     0,     0,    81,    82,    83,    84,
      85,     0,    71,    86,    73,     0,    75,    76,    77,    78,
      79,    80,     0,     0,     0,    81,    82,    83,    84,    85,
       0,     0,    86,    75,    76,    77,    78,    79,    80,     0,
       0,     0,    81,    82,    83,    84,    85,     0,     0,    86,
     -85,   -85,   -85,   -85,   -85,   -85,     0,     0,     0,    81,
      82,    83,    84,    85,     0,     0,    86
};

static const yytype_int16 yycheck[] =
{
       0,     0,   104,    45,    19,    19,     4,    47,     5,     9,
      10,    49,    50,    56,    57,    49,    50,     5,    41,    19,
       4,    44,     5,    57,    49,    50,    23,     3,     4,     5,
       6,     7,     8,     9,     5,    47,    12,     0,    14,    10,
      16,    17,    18,    19,    48,    20,    21,    22,    24,    41,
      50,    48,    45,    53,    44,    53,    47,   159,    55,    16,
      17,    37,    38,    20,    21,    22,    23,    55,    25,    53,
      45,    47,    48,    88,    88,    48,    52,    55,    93,    93,
      56,    23,    13,    49,    50,    15,   188,    39,    40,    41,
      90,    57,    44,    47,    48,    49,    50,    48,    55,    99,
      49,    50,   144,    48,   104,    47,    48,    51,   108,    47,
      48,   110,    47,    48,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,   174,    89,    54,    91,    92,   178,    94,    20,    21,
      22,    51,    99,   100,   187,    49,    50,    53,    54,   221,
     222,    54,    53,    48,    56,    51,    23,   157,    90,   159,
      53,   180,   205,   163,     0,   159,   123,     9,    13,   106,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,
     180,     3,     4,     5,     6,     7,     8,     9,   188,    -1,
      -1,    -1,    -1,    -1,   194,    -1,    -1,   196,   197,    -1,
      -1,   158,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    37,    38,    -1,    -1,   176,
      -1,   221,   222,   180,   181,    47,    48,    -1,    -1,    -1,
      52,    53,     1,   190,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    -1,    14,    -1,    16,    17,    18,
      19,   208,    -1,    -1,    -1,    24,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    34,    35,    -1,    37,    38,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    47,    48,
      49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    48,    -1,    -1,    -1,    52,    53,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    12,    -1,    14,    -1,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    24,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    47,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    48,    -1,    -1,    -1,    52,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    -1,    -1,
      37,    38,    39,    40,    41,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    37,    38,
      39,    40,    41,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    -1,    24,    44,    26,    -1,    28,    29,    30,    31,
      32,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      -1,    -1,    44,    28,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    -1,    -1,    44,
      28,    29,    30,    31,    32,    33,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    -1,    -1,    44
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    14,    16,    17,    18,    19,    24,    34,    35,
      37,    38,    47,    48,    49,    52,    59,    60,    61,    62,
      63,    64,    67,    68,    72,    82,    83,    84,    87,    88,
      90,    92,    93,    94,    95,    96,    97,    99,    49,    50,
      76,    96,    96,    47,    91,    47,    89,    89,    60,    96,
      60,    64,    68,    96,    60,    60,    60,    60,    98,    60,
       0,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    37,    38,    39,    40,    41,    44,    20,    21,    22,
      45,    48,    20,    21,    22,    49,    50,    49,    50,    47,
      48,    65,    66,    96,    47,    48,    77,    78,    41,    96,
      56,    80,    83,    60,    80,    80,    65,    49,    50,    49,
      50,    51,    53,    54,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    68,    60,    67,    96,    60,    60,    68,    60,    60,
      69,    70,    71,    96,    53,    60,    55,    79,    48,    47,
      73,    74,    75,    76,    96,     4,    53,    79,    48,    96,
      13,    81,    83,    51,    15,    60,    48,    65,    53,    51,
      54,    23,    53,    96,    53,    60,    73,    51,    54,    96,
      23,    53,     4,    53,    56,    60,    49,    50,    57,    80,
      60,    65,    71,    60,    53,    51,    80,    75,    23,    60,
      53,    72,    75,    85,    86,    51,    83,    83,    53,    80,
      60,    49,    50,    57,    65,    86,    86
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
      case 60: /* "expression" */
#line 126 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1489 "./grammar.tab.c"
	break;
      case 61: /* "arrowAssign" */
#line 127 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1494 "./grammar.tab.c"
	break;
      case 62: /* "tildeAssign" */
#line 127 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1499 "./grammar.tab.c"
	break;
      case 63: /* "equationAssign" */
#line 127 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1504 "./grammar.tab.c"
	break;
      case 64: /* "variable" */
#line 125 "./grammar.y"
	{ delete ((yyvaluep->syntaxVariable)); };
#line 1509 "./grammar.tab.c"
	break;
      case 65: /* "optElements" */
#line 121 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1514 "./grammar.tab.c"
	break;
      case 66: /* "elementList" */
#line 121 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1519 "./grammar.tab.c"
	break;
      case 67: /* "fxnCall" */
#line 125 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); };
#line 1524 "./grammar.tab.c"
	break;
      case 68: /* "functionCall" */
#line 125 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); };
#line 1529 "./grammar.tab.c"
	break;
      case 69: /* "optArguments" */
#line 122 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1534 "./grammar.tab.c"
	break;
      case 70: /* "argumentList" */
#line 122 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1539 "./grammar.tab.c"
	break;
      case 71: /* "argument" */
#line 125 "./grammar.y"
	{ delete ((yyvaluep->syntaxLabeledExpr)); };
#line 1544 "./grammar.tab.c"
	break;
      case 72: /* "functionDef" */
#line 129 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1549 "./grammar.tab.c"
	break;
      case 73: /* "optFormals" */
#line 123 "./grammar.y"
	{ for (std::list<SyntaxFormal*>::iterator it=(yyvaluep->formalList)->begin(); it != (yyvaluep->formalList)->end(); it++) { SyntaxFormal* theElement = *it; delete theElement; }; delete ((yyvaluep->formalList)); };
#line 1554 "./grammar.tab.c"
	break;
      case 74: /* "formalList" */
#line 123 "./grammar.y"
	{ for (std::list<SyntaxFormal*>::iterator it=(yyvaluep->formalList)->begin(); it != (yyvaluep->formalList)->end(); it++) { SyntaxFormal* theElement = *it; delete theElement; }; delete ((yyvaluep->formalList)); };
#line 1559 "./grammar.tab.c"
	break;
      case 75: /* "formal" */
#line 125 "./grammar.y"
	{ delete ((yyvaluep->syntaxFormal)); };
#line 1564 "./grammar.tab.c"
	break;
      case 76: /* "typeSpec" */
#line 124 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1569 "./grammar.tab.c"
	break;
      case 77: /* "optDims" */
#line 124 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1574 "./grammar.tab.c"
	break;
      case 78: /* "dimList" */
#line 124 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1579 "./grammar.tab.c"
	break;
      case 79: /* "optRef" */
#line 124 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1584 "./grammar.tab.c"
	break;
      case 80: /* "stmts" */
#line 121 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1589 "./grammar.tab.c"
	break;
      case 81: /* "stmtList" */
#line 121 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1594 "./grammar.tab.c"
	break;
      case 82: /* "statement" */
#line 126 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1599 "./grammar.tab.c"
	break;
      case 83: /* "stmt_or_expr" */
#line 126 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1604 "./grammar.tab.c"
	break;
      case 84: /* "declaration" */
#line 128 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1609 "./grammar.tab.c"
	break;
      case 85: /* "memberDefs" */
#line 121 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1614 "./grammar.tab.c"
	break;
      case 86: /* "memberDef" */
#line 128 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1619 "./grammar.tab.c"
	break;
      case 87: /* "classDef" */
#line 128 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1624 "./grammar.tab.c"
	break;
      case 88: /* "ifStatement" */
#line 130 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1629 "./grammar.tab.c"
	break;
      case 89: /* "cond" */
#line 131 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1634 "./grammar.tab.c"
	break;
      case 90: /* "forStatement" */
#line 130 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1639 "./grammar.tab.c"
	break;
      case 91: /* "forCond" */
#line 131 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1644 "./grammar.tab.c"
	break;
      case 92: /* "whileStatement" */
#line 130 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1649 "./grammar.tab.c"
	break;
      case 93: /* "nextStatement" */
#line 132 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1654 "./grammar.tab.c"
	break;
      case 94: /* "breakStatement" */
#line 132 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1659 "./grammar.tab.c"
	break;
      case 95: /* "returnStatement" */
#line 131 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1664 "./grammar.tab.c"
	break;
      case 96: /* "identifier" */
#line 124 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1669 "./grammar.tab.c"
	break;
      case 97: /* "vector" */
#line 122 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1674 "./grammar.tab.c"
	break;
      case 98: /* "vectorList" */
#line 122 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1679 "./grammar.tab.c"
	break;
      case 99: /* "constant" */
#line 125 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1684 "./grammar.tab.c"
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
#line 201 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison encountered end_of_input; ignored\n");
#endif
                    return 0;
                }
    break;

  case 3:
#line 208 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison encountered newline; ignored\n");
#endif
                    return 0;
                }
    break;

  case 4:
#line 215 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute statement or expression\n");
#endif
                    int rv = parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    return rv;
                }
    break;

  case 5:
#line 223 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute statement or expression\n");
#endif
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    return rv;
                }
    break;

  case 6:
#line 231 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute declaration\n");
#endif
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    return rv;
                }
    break;

  case 7:
#line 239 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute declaration\n");
#endif
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    return rv;
                }
    break;

  case 8:
#line 247 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to get help for symbol\n");
#endif
                    int rv =  parser.help(*((yyvsp[(2) - (3)].string)));
                    delete (yyvsp[(2) - (3)].string);
                    return rv;
                }
    break;

  case 9:
#line 256 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to get help for symbol\n");
#endif
                    int rv =  parser.help(*(yyvsp[(2) - (3)].string));
                    delete (yyvsp[(2) - (3)].string);
                    return rv;
                }
    break;

  case 10:
#line 265 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to get help for function call\n");
#endif
                    int rv =  parser.help(*(yyvsp[(2) - (3)].syntaxFunctionCall));
                    delete (yyvsp[(2) - (3)].syntaxFunctionCall);
                    return rv;
                }
    break;

  case 11:
#line 274 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to get help for function call\n");
#endif
                    int rv =  parser.help(*(yyvsp[(2) - (3)].syntaxFunctionCall));
                    delete (yyvsp[(2) - (3)].syntaxFunctionCall);
                    return rv;
                }
    break;

  case 12:
#line 283 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
#endif
                    YYABORT;
                }
    break;

  case 13:
#line 290 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
#endif
                    YYABORT;
                }
    break;

  case 14:
#line 298 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 15:
#line 300 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxFunctionCall("v", (yyvsp[(1) - (1)].argumentList)); }
    break;

  case 16:
#line 302 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 17:
#line 304 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UMinus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 18:
#line 305 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UPlus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 19:
#line 306 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UNot, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 20:
#line 307 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UAnd, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 21:
#line 309 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Range, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 22:
#line 311 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Add, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 23:
#line 312 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Sub, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 24:
#line 313 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Mul, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 25:
#line 314 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Div, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 26:
#line 315 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Exp, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 27:
#line 317 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Lt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 28:
#line 318 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Le, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 29:
#line 319 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Eq, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 30:
#line 320 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ne, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 31:
#line 321 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ge, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 32:
#line 322 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Gt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 33:
#line 324 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 34:
#line 325 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 35:
#line 326 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 36:
#line 327 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 37:
#line 329 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 38:
#line 330 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 39:
#line 331 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 40:
#line 333 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFunctionCall); }
    break;

  case 41:
#line 335 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxVariable); }
    break;

  case 42:
#line 339 "./grammar.y"
    { 
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxConstantAssignment((yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 43:
#line 346 "./grammar.y"
    { 
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxConstantAssignment((yyvsp[(1) - (3)].syntaxFunctionCall), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 44:
#line 355 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxStochasticAssignment((yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxFunctionCall));
                    }
    break;

  case 45:
#line 362 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxStochasticAssignment((yyvsp[(1) - (3)].syntaxFunctionCall), (yyvsp[(3) - (3)].syntaxFunctionCall));
                    }
    break;

  case 46:
#line 371 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxDeterministicAssignment((yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 47:
#line 378 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxDeterministicAssignment((yyvsp[(1) - (3)].syntaxFunctionCall), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 48:
#line 387 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting variable (NAMED_VAR)in syntax tree\n");
#endif
                    (yyval.syntaxVariable) = new SyntaxVariable(*(yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].syntaxElementList));
                    delete (yyvsp[(1) - (2)].string);
                }
    break;

  case 49:
#line 395 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting variable (FUNCTION_VAR) in syntax tree\n");
#endif
                    (yyvsp[(5) - (5)].syntaxElementList)->push_front((yyvsp[(3) - (5)].syntaxElement));
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (5)].syntaxFunctionCall), (yyvsp[(5) - (5)].syntaxElementList));
                }
    break;

  case 50:
#line 403 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting member variable (NAMED_VAR)in syntax tree\n");
#endif
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (4)].syntaxVariable), *(yyvsp[(3) - (4)].string), (yyvsp[(4) - (4)].syntaxElementList));
                    delete (yyvsp[(3) - (4)].string);
                }
    break;

  case 51:
#line 411 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting member variable (FUNCTION_VAR) in syntax tree\n");
#endif
                    (yyvsp[(7) - (7)].syntaxElementList)->push_front((yyvsp[(5) - (7)].syntaxElement));
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (7)].syntaxVariable), (yyvsp[(3) - (7)].syntaxFunctionCall), (yyvsp[(7) - (7)].syntaxElementList));
                }
    break;

  case 52:
#line 420 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 53:
#line 421 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (1)].syntaxElementList); }
    break;

  case 54:
#line 424 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(2) - (3)].syntaxElement)); }
    break;

  case 55:
#line 425 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 56:
#line 426 "./grammar.y"
    { (yyvsp[(1) - (4)].syntaxElementList)->push_back((yyvsp[(3) - (4)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (4)].syntaxElementList); }
    break;

  case 57:
#line 427 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back( NULL ); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 58:
#line 431 "./grammar.y"
    {
                    (yyval.syntaxFunctionCall) = new SyntaxFunctionCall(*(yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].argumentList));
                    delete (yyvsp[(1) - (4)].string);
                }
    break;

  case 59:
#line 438 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting function call in syntax tree\n");
#endif
                        (yyval.syntaxFunctionCall) = (yyvsp[(1) - (1)].syntaxFunctionCall);
                    }
    break;

  case 60:
#line 445 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting member call in syntax tree\n");
#endif
                        (yyvsp[(3) - (3)].syntaxFunctionCall)->setBaseVariable((yyvsp[(1) - (3)].syntaxVariable));
                        (yyval.syntaxFunctionCall) = (yyvsp[(3) - (3)].syntaxFunctionCall);
                    }
    break;

  case 61:
#line 454 "./grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(); }
    break;

  case 62:
#line 455 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(1) - (1)].argumentList); }
    break;

  case 63:
#line 458 "./grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(1,(yyvsp[(1) - (1)].syntaxLabeledExpr)); }
    break;

  case 64:
#line 459 "./grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back((yyvsp[(3) - (3)].syntaxLabeledExpr)); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 65:
#line 463 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting unlabeled argument in syntax tree\n");
#endif
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr( "", (yyvsp[(1) - (1)].syntaxElement));
                }
    break;

  case 66:
#line 470 "./grammar.y"
    { 
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting labeled argument in syntax tree\n");
#endif
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr(*(yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].syntaxElement));
                    delete (yyvsp[(1) - (3)].string);
                }
    break;

  case 67:
#line 480 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting function definition in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxFunctionDef("", *(yyvsp[(2) - (6)].string), (yyvsp[(4) - (6)].formalList), (yyvsp[(6) - (6)].syntaxElementList));
                    delete (yyvsp[(2) - (6)].string);
                }
    break;

  case 68:
#line 489 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting typed function definition in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxFunctionDef(*(yyvsp[(2) - (7)].string), *(yyvsp[(3) - (7)].string), (yyvsp[(5) - (7)].formalList), (yyvsp[(7) - (7)].syntaxElementList));
                    delete (yyvsp[(2) - (7)].string);
                    delete (yyvsp[(3) - (7)].string);
                }
    break;

  case 69:
#line 499 "./grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal*>(); }
    break;

  case 70:
#line 500 "./grammar.y"
    { (yyval.formalList) = (yyvsp[(1) - (1)].formalList); }
    break;

  case 71:
#line 503 "./grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal*>(1, (yyvsp[(1) - (1)].syntaxFormal)); }
    break;

  case 72:
#line 504 "./grammar.y"
    { (yyvsp[(1) - (3)].formalList)->push_back((yyvsp[(3) - (3)].syntaxFormal)); (yyval.formalList) = (yyvsp[(1) - (3)].formalList); }
    break;

  case 73:
#line 508 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting labeled formal argument without default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (1)].string), NULL );
                    delete (yyvsp[(1) - (1)].string);
                }
    break;

  case 74:
#line 516 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting labeled formal argument with default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].syntaxElement));
                    delete (yyvsp[(1) - (3)].string);
                }
    break;

  case 75:
#line 524 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting typed labeled formal argument without default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (2)].string), *(yyvsp[(2) - (2)].string), NULL);
                    delete (yyvsp[(1) - (2)].string);
                    delete (yyvsp[(2) - (2)].string);
                }
    break;

  case 76:
#line 533 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting typed labeled formal argument with default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (4)].string), *(yyvsp[(2) - (4)].string), (yyvsp[(4) - (4)].syntaxElement));
                    delete (yyvsp[(1) - (4)].string);
                    delete (yyvsp[(2) - (4)].string);
                }
    break;

  case 77:
#line 543 "./grammar.y"
    { (yyvsp[(1) - (3)].string)->append(*((yyvsp[(2) - (3)].string))); (yyvsp[(1) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); delete (yyvsp[(2) - (3)].string); delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(1) - (3)].string); }
    break;

  case 78:
#line 546 "./grammar.y"
    { (yyval.string) = new std::string(""); }
    break;

  case 79:
#line 547 "./grammar.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); }
    break;

  case 80:
#line 550 "./grammar.y"
    { (yyval.string) = new std::string("[]"); }
    break;

  case 81:
#line 551 "./grammar.y"
    { 
                                                (yyval.string) = new std::string("["); 
                                                std::stringstream o;
                                                o << (yyvsp[(2) - (3)].intValue);
                                                (yyval.string)->append(o.str()); 
                                                (yyval.string)->append("]"); 
                                            }
    break;

  case 82:
#line 558 "./grammar.y"
    { (yyvsp[(1) - (3)].string)->append("[]"); (yyval.string) = (yyvsp[(1) - (3)].string); }
    break;

  case 83:
#line 559 "./grammar.y"
    { 
                                                (yyvsp[(1) - (4)].string)->append("["); 
                                                std::stringstream o;
                                                o << (yyvsp[(3) - (4)].intValue);
                                                (yyvsp[(1) - (4)].string)->append(o.str()); 
                                                (yyvsp[(1) - (4)].string)->append("]"); 
                                                (yyval.string) = (yyvsp[(1) - (4)].string);
                                            }
    break;

  case 84:
#line 569 "./grammar.y"
    { (yyval.string) = new std::string(""); }
    break;

  case 85:
#line 570 "./grammar.y"
    { (yyval.string) = new std::string("&"); }
    break;

  case 86:
#line 573 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 87:
#line 575 "./grammar.y"
    {
                    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();
                    stmts->push_back((yyvsp[(1) - (1)].syntaxElement));
                    (yyval.syntaxElementList) = stmts;
                }
    break;

  case 88:
#line 582 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 89:
#line 583 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 90:
#line 584 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 91:
#line 585 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 92:
#line 586 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 93:
#line 587 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 94:
#line 590 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 95:
#line 591 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 96:
#line 592 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 97:
#line 593 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 98:
#line 594 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 99:
#line 595 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 100:
#line 598 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 101:
#line 599 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 102:
#line 602 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 103:
#line 603 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 104:
#line 605 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting variable declaration in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxVariableDecl(*(yyvsp[(1) - (4)].string), (yyvsp[(2) - (4)].syntaxElementList), *(yyvsp[(3) - (4)].string), *(yyvsp[(4) - (4)].string));
                        delete (yyvsp[(1) - (4)].string);
                        delete (yyvsp[(3) - (4)].string);
                        delete (yyvsp[(4) - (4)].string);
                    }
    break;

  case 105:
#line 616 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 106:
#line 617 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 107:
#line 618 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 108:
#line 619 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 109:
#line 620 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 110:
#line 621 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 111:
#line 624 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFormal); }
    break;

  case 112:
#line 625 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 113:
#line 629 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                printf("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxClassDef(*(yyvsp[(2) - (7)].string), *(yyvsp[(4) - (7)].string), (yyvsp[(6) - (7)].syntaxElementList));
                    delete (yyvsp[(2) - (7)].string);
                    delete (yyvsp[(4) - (7)].string);
                }
    break;

  case 114:
#line 639 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::If, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 115:
#line 640 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::IfElse, (yyvsp[(2) - (5)].syntaxElement), (yyvsp[(3) - (5)].syntaxElementList), (yyvsp[(5) - (5)].syntaxElementList)); }
    break;

  case 116:
#line 642 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 117:
#line 645 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::For, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 118:
#line 648 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxForLoop(*(yyvsp[(2) - (5)].string), (yyvsp[(4) - (5)].syntaxElement)); delete (yyvsp[(2) - (5)].string); }
    break;

  case 119:
#line 651 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::While, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 120:
#line 654 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Next); }
    break;

  case 121:
#line 657 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Break); }
    break;

  case 122:
#line 660 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return); }
    break;

  case 123:
#line 661 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 124:
#line 664 "./grammar.y"
    { (yyval.string) = new std::string((yyvsp[(1) - (1)].c_string)); }
    break;

  case 125:
#line 668 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(2) - (3)].argumentList); }
    break;

  case 126:
#line 671 "./grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back(new SyntaxLabeledExpr( "", (yyvsp[(3) - (3)].syntaxElement)) ); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 127:
#line 673 "./grammar.y"
    {
                (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(1, new SyntaxLabeledExpr("", (yyvsp[(1) - (1)].syntaxElement)) );
                }
    break;

  case 128:
#line 679 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting bool constant (false) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant(new RlBoolean(false) );
                }
    break;

  case 129:
#line 686 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting bool constant (true) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant(new RlBoolean(true) );
                }
    break;

  case 130:
#line 693 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting null constant in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant( NULL );
                }
    break;

  case 131:
#line 700 "./grammar.y"
    {
                    if ( (yyvsp[(1) - (1)].intValue) < 0 ) {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting Integer constant in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxConstant(new Integer((yyvsp[(1) - (1)].intValue)) );
                    }
                    else { 
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting Natural constant in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxConstant(new Natural((yyvsp[(1) - (1)].intValue)) );
                    }
                }
    break;

  case 132:
#line 715 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting String constant in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant(new RlString((yyvsp[(1) - (1)].c_string)) );
                }
    break;

  case 133:
#line 722 "./grammar.y"
    {
                    /* This code records and preserves input format of the real */
                    /*
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
                    Real* real;
                    if ($1 >= 0.0)
                        real = new RealPos($1);
                    else
                        real = new Real($1);
                    real->setPrecision(prec);
                    real->setScientific(sci);
                    */
                    if ((yyvsp[(1) - (1)].realValue) >= 0.0) {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting RealPos constant in syntax tree \n");
#endif
                        (yyval.syntaxElement) = new SyntaxConstant(new RealPos((yyvsp[(1) - (1)].realValue)) );
                    }
                    else {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting Real constant in syntax tree \n");
#endif
                        (yyval.syntaxElement) = new SyntaxConstant(new Real((yyvsp[(1) - (1)].realValue)) );
                    }
                }
    break;


/* Line 1267 of yacc.c.  */
#line 2975 "./grammar.tab.c"
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


#line 775 "./grammar.y"



/* Definition of yyerror. */
int yyerror(const char *msg)
{
#ifdef DEBUG_BISON_FLEX
    printf("Bison code said: %s\n", msg);
#endif
    if ( foundNewline == true )
        foundErrorBeforeEnd = false;
    else
        foundErrorBeforeEnd = true;

    yylloc.first_column = yycolumn - yyleng;
    yylloc.last_column  = yycolumn - 1;

    return 1;
}



