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
#include "SyntaxAdditionAssignment.h"
#include "SyntaxBinaryExpr.h"
#include "SyntaxClassDef.h"
#include "SyntaxConstant.h"
#include "SyntaxConstantAssignment.h"
#include "SyntaxControlAssignment.h"
#include "SyntaxDecrement.h"
#include "SyntaxDeterministicAssignment.h"
#include "SyntaxDivisionAssignment.h"
#include "SyntaxForLoop.h"
#include "SyntaxFormal.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxFunctionDef.h"
#include "SyntaxIncrement.h"
#include "SyntaxLabeledExpr.h"
#include "SyntaxMultiplicationAssignment.h"
#include "SyntaxReferenceAssignment.h"
#include "SyntaxStatement.h"
#include "SyntaxStochasticAssignment.h"
#include "SyntaxSubtractionAssignment.h"
#include "SyntaxUnaryExpr.h"
#include "SyntaxVariableDecl.h"
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
/* Line 193 of yacc.c.  */
#line 290 "./grammar.tab.c"
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
#line 315 "./grammar.tab.c"

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
#define YYFINAL  81
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1121

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  70
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  148
/* YYNRULES -- Number of states.  */
#define YYNSTATES  263

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   306

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      62,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    65,     2,     2,     2,     2,     2,     2,
      60,    64,    52,    50,    67,    51,    58,    53,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    54,    63,
       2,     2,     2,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    61,     2,    66,    57,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    68,     2,    69,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    49,    55,    56,    59
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    16,    19,    23,
      27,    31,    35,    38,    41,    43,    45,    49,    52,    55,
      58,    61,    64,    67,    70,    73,    77,    81,    85,    89,
      93,    97,   101,   105,   109,   113,   117,   121,   125,   129,
     133,   137,   139,   141,   143,   145,   147,   149,   151,   153,
     155,   157,   159,   163,   167,   171,   175,   179,   183,   187,
     191,   195,   198,   204,   212,   217,   225,   226,   228,   232,
     235,   240,   244,   249,   251,   255,   256,   258,   260,   264,
     266,   270,   277,   286,   287,   289,   291,   295,   297,   301,
     304,   309,   312,   316,   320,   324,   328,   332,   333,   335,
     338,   342,   346,   348,   349,   351,   355,   358,   362,   365,
     367,   369,   371,   373,   375,   377,   379,   381,   383,   385,
     389,   390,   392,   396,   399,   403,   406,   408,   410,   418,
     422,   428,   432,   436,   442,   446,   448,   450,   452,   455,
     457,   461,   465,   467,   469,   471,   473,   475,   477
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      71,     0,    -1,    47,    -1,    62,    -1,   100,    62,    -1,
     100,    63,    -1,   101,    62,    -1,   101,    63,    -1,    48,
     113,    62,    -1,    48,   113,    63,    -1,    48,    86,    62,
      -1,    48,    86,    63,    -1,     1,    62,    -1,     1,    63,
      -1,   116,    -1,   114,    -1,    60,    72,    64,    -1,    51,
      72,    -1,    50,    72,    -1,    65,    72,    -1,    37,    72,
      -1,    34,    82,    -1,    82,    34,    -1,    35,    82,    -1,
      82,    35,    -1,    72,    54,    72,    -1,    72,    50,    72,
      -1,    72,    51,    72,    -1,    72,    52,    72,    -1,    72,
      53,    72,    -1,    72,    57,    72,    -1,    72,    43,    72,
      -1,    72,    44,    72,    -1,    72,    45,    72,    -1,    72,
      46,    72,    -1,    72,    42,    72,    -1,    72,    41,    72,
      -1,    72,    37,    72,    -1,    72,    38,    72,    -1,    72,
      39,    72,    -1,    72,    40,    72,    -1,    73,    -1,    75,
      -1,    74,    -1,    76,    -1,    77,    -1,    78,    -1,    79,
      -1,    80,    -1,    81,    -1,    86,    -1,    82,    -1,    72,
      25,    72,    -1,    72,    26,    72,    -1,    72,    27,    72,
      -1,    72,    28,    72,    -1,    72,    29,    72,    -1,    72,
      30,    72,    -1,    72,    31,    72,    -1,    72,    32,    72,
      -1,    72,    33,    72,    -1,   113,    83,    -1,    85,    61,
      72,    66,    83,    -1,    60,    72,    64,    61,    72,    66,
      83,    -1,    82,    58,   113,    83,    -1,    82,    58,    85,
      61,    72,    66,    83,    -1,    -1,    84,    -1,    61,    72,
      66,    -1,    61,    66,    -1,    84,    61,    72,    66,    -1,
      84,    61,    66,    -1,   113,    60,    87,    64,    -1,    85,
      -1,    82,    58,    85,    -1,    -1,    88,    -1,    89,    -1,
      88,    67,    89,    -1,    72,    -1,   113,    36,    72,    -1,
      10,   113,    60,    91,    64,    97,    -1,    10,   113,    95,
     113,    60,    91,    64,    97,    -1,    -1,    92,    -1,    93,
      -1,    92,    67,    93,    -1,   113,    -1,   113,    36,    72,
      -1,    94,   113,    -1,    94,   113,    36,    72,    -1,   113,
      95,    -1,    20,   113,    95,    -1,    21,   113,    95,    -1,
      22,   113,    95,    -1,    23,   113,    95,    -1,    24,   113,
      95,    -1,    -1,    96,    -1,    61,    66,    -1,    96,    61,
      66,    -1,    68,    98,    69,    -1,   100,    -1,    -1,   100,
      -1,    98,    63,   100,    -1,    98,    63,    -1,    98,    62,
     100,    -1,    98,    62,    -1,   105,    -1,   107,    -1,   109,
      -1,   110,    -1,   111,    -1,   112,    -1,    99,    -1,    72,
      -1,   104,    -1,    90,    -1,   113,    83,   113,    -1,    -1,
     103,    -1,   102,    63,   103,    -1,   102,    63,    -1,   102,
      62,   103,    -1,   102,    62,    -1,    93,    -1,    90,    -1,
      11,   113,    54,   113,    68,   102,    69,    -1,    14,   106,
      97,    -1,    14,   106,    97,    15,    97,    -1,    60,    72,
      64,    -1,    12,   108,    97,    -1,    60,   113,    13,    72,
      64,    -1,    16,   106,    97,    -1,    17,    -1,    18,    -1,
      19,    -1,    19,    72,    -1,     5,    -1,    61,   115,    66,
      -1,   115,    67,    72,    -1,    72,    -1,     8,    -1,     9,
      -1,     7,    -1,     4,    -1,     6,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   229,   229,   236,   243,   251,   259,   267,   275,   284,
     293,   302,   311,   318,   327,   329,   331,   333,   334,   335,
     336,   338,   339,   340,   341,   343,   345,   346,   347,   348,
     349,   351,   352,   353,   354,   355,   356,   358,   359,   360,
     361,   363,   364,   365,   366,   367,   369,   370,   371,   372,
     374,   376,   379,   388,   397,   406,   415,   424,   433,   442,
     451,   460,   468,   476,   484,   492,   502,   503,   506,   507,
     508,   509,   512,   519,   526,   536,   537,   540,   541,   544,
     551,   561,   570,   583,   584,   587,   588,   591,   599,   607,
     616,   627,   628,   629,   630,   631,   632,   635,   636,   639,
     640,   643,   644,   652,   653,   654,   655,   656,   657,   660,
     661,   662,   663,   664,   665,   668,   669,   672,   673,   674,
     685,   686,   687,   688,   689,   690,   693,   694,   697,   708,
     709,   711,   714,   717,   720,   723,   726,   729,   730,   733,
     737,   740,   741,   747,   754,   761,   768,   783,   790
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "REAL", "INT", "NAME", "STRING",
  "RBNULL", "FALSE", "TRUE", "FUNCTION", "CLASS", "FOR", "IN", "IF",
  "ELSE", "WHILE", "NEXT", "BREAK", "RETURN", "MOD_CONST", "MOD_DYNAMIC",
  "MOD_STOCHASTIC", "MOD_DETERMINISTIC", "MOD_MUTABLE", "ARROW_ASSIGN",
  "TILDE_ASSIGN", "EQUATION_ASSIGN", "CONTROL_ASSIGN", "REFERENCE_ASSIGN",
  "ADDITION_ASSIGN", "SUBTRACTION_ASSIGN", "MULTIPLICATION_ASSIGN",
  "DIVISION_ASSIGN", "DECREMENT", "INCREMENT", "EQUAL", "AND", "OR",
  "AND2", "OR2", "GT", "GE", "LT", "LE", "EQ", "NE", "END_OF_INPUT", "'?'",
  "UNOT", "'+'", "'-'", "'*'", "'/'", "':'", "UPLUS", "UMINUS", "'^'",
  "'.'", "UAND", "'('", "'['", "'\\n'", "';'", "')'", "'!'", "']'", "','",
  "'{'", "'}'", "$accept", "prog", "expression", "arrowAssign",
  "tildeAssign", "equationAssign", "controlAssign", "referenceAssign",
  "additionAssign", "subtractionAssign", "multiplicationAssign",
  "divisionAssign", "variable", "optElements", "elementList", "fxnCall",
  "functionCall", "optArguments", "argumentList", "argument",
  "functionDef", "optFormals", "formalList", "formal", "typeSpec",
  "optDims", "dimList", "stmts", "stmtList", "statement", "stmt_or_expr",
  "declaration", "memberDefs", "memberDef", "classDef", "ifStatement",
  "cond", "forStatement", "forCond", "whileStatement", "nextStatement",
  "breakStatement", "returnStatement", "identifier", "vector",
  "vectorList", "constant", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    63,   303,
      43,    45,    42,    47,    58,   304,   305,    94,    46,   306,
      40,    91,    10,    59,    41,    33,    93,    44,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    70,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    82,    82,    82,    82,    83,    83,    84,    84,
      84,    84,    85,    86,    86,    87,    87,    88,    88,    89,
      89,    90,    90,    91,    91,    92,    92,    93,    93,    93,
      93,    94,    94,    94,    94,    94,    94,    95,    95,    96,
      96,    97,    97,    98,    98,    98,    98,    98,    98,    99,
      99,    99,    99,    99,    99,   100,   100,   101,   101,   101,
     102,   102,   102,   102,   102,   102,   103,   103,   104,   105,
     105,   106,   107,   108,   109,   110,   111,   112,   112,   113,
     114,   115,   115,   116,   116,   116,   116,   116,   116
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     2,     2,     1,     1,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     5,     7,     4,     7,     0,     1,     3,     2,
       4,     3,     4,     1,     3,     0,     1,     1,     3,     1,
       3,     6,     8,     0,     1,     1,     3,     1,     3,     2,
       4,     2,     3,     3,     3,     3,     3,     0,     1,     2,
       3,     3,     1,     0,     1,     3,     2,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     3,     2,     3,     2,     1,     1,     7,     3,
       5,     3,     3,     5,     3,     1,     1,     1,     2,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   148,   146,   139,   147,   145,   143,   144,     0,
       0,     0,     0,     0,   135,   136,   137,     0,     0,     0,
       2,     0,     0,     0,     0,     0,     3,     0,     0,   116,
      41,    43,    42,    44,    45,    46,    47,    48,    49,    51,
      73,    50,   118,   115,     0,     0,   117,   109,   110,   111,
     112,   113,   114,    66,    15,    14,    12,    13,    97,     0,
       0,     0,     0,     0,     0,   138,    66,     0,    21,     0,
      23,    20,     0,     0,    66,    18,    17,     0,   142,     0,
      19,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,    24,     0,
       0,     4,     5,     6,     7,    75,     0,    61,    67,    83,
       0,     0,    98,     0,     0,   103,   132,   102,     0,   129,
     134,    61,     0,     0,    10,    11,     8,     9,    16,   140,
       0,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      37,    38,    39,    40,    36,    35,    31,    32,    33,    34,
      26,    27,    28,    29,    25,    30,    74,    66,     0,    79,
       0,    76,    77,    66,    69,     0,   119,     0,     0,     0,
       0,     0,     0,     0,    84,    85,     0,    87,    99,     0,
       0,     0,     0,     0,   104,   131,     0,     0,     0,     0,
     141,     0,    64,    66,    72,     0,     0,    68,    71,     0,
      97,    97,    97,    97,    97,     0,     0,    89,     0,    91,
      83,   100,   120,     0,   108,   106,   101,   130,     0,     0,
      62,    78,    80,    70,    92,    93,    94,    95,    96,    81,
      86,     0,    88,     0,   127,   126,     0,   121,   133,   107,
     105,    66,    66,    90,     0,   125,   123,   128,    63,    65,
      82,   124,   122
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   131,   118,    40,    41,   170,   171,   172,
     244,   183,   184,   245,   186,   121,   122,   126,   193,    43,
     127,    45,   246,   247,    46,    47,    63,    48,    61,    49,
      50,    51,    52,    66,    54,    79,    55
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -183
static const yytype_int16 yypact[] =
{
     297,    37,  -183,  -183,  -183,  -183,  -183,  -183,  -183,    17,
      17,   -54,   -19,   -19,  -183,  -183,   397,    13,    13,   397,
    -183,    13,   397,   397,   397,   397,  -183,   397,    55,   808,
    -183,  -183,  -183,  -183,  -183,  -183,  -183,  -183,  -183,   -10,
     -18,  -183,  -183,  -183,    43,    48,  -183,  -183,  -183,  -183,
    -183,  -183,  -183,   -14,  -183,  -183,  -183,  -183,     6,     8,
      17,   231,   397,   231,   231,   808,   -14,   397,    45,   -18,
      45,  -183,    56,    50,     9,    44,    44,   648,   808,    63,
    1047,  -183,   397,   397,   397,   397,   397,   397,   397,   397,
     397,   397,   397,   397,   397,   397,   397,   397,   397,   397,
     397,   397,   397,   397,   397,   397,   397,  -183,  -183,    17,
     397,  -183,  -183,  -183,  -183,   397,   153,    17,    57,   117,
      59,    17,    66,    17,   118,   379,  -183,  -183,   688,   127,
    -183,  -183,   728,    17,  -183,  -183,  -183,  -183,    82,  -183,
     397,   808,   840,   871,   901,   930,   958,   985,  1008,  1030,
    1047,   233,  1047,   233,  1064,  1064,  1064,  1064,  1064,  1064,
       4,     4,   -17,   -17,    44,    44,    83,   -14,   438,   808,
      81,    79,  -183,   -16,  -183,   480,  -183,   315,    17,    17,
      17,    17,    17,    84,    80,  -183,    17,     2,  -183,    89,
      85,    86,   397,   -36,  -183,  -183,   231,    82,    83,   397,
     808,   397,  -183,    92,  -183,   397,   397,  -183,  -183,   522,
     102,   102,   102,   102,   102,   231,   117,   114,   397,  -183,
     117,  -183,    29,   768,   379,   379,  -183,  -183,   564,   606,
    -183,  -183,   808,  -183,  -183,  -183,  -183,  -183,  -183,  -183,
    -183,   397,   808,   100,  -183,  -183,   -27,  -183,  -183,  -183,
    -183,    92,    92,   808,   231,    29,    29,  -183,  -183,  -183,
    -183,  -183,  -183
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -183,  -183,    -8,  -183,  -183,  -183,  -183,  -183,  -183,  -183,
    -183,  -183,    47,   -51,  -183,    -5,   144,  -183,  -183,   -39,
     167,   -52,  -183,   -96,  -183,  -182,  -183,   -60,  -183,  -183,
       1,  -183,  -183,  -121,  -183,  -183,   157,  -183,  -183,  -183,
    -183,  -183,  -183,     0,  -183,  -183,  -183
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -98
static const yytype_int16 yytable[] =
{
      53,    44,   117,   129,   130,   219,    60,   -97,    65,    58,
      59,    71,    69,    69,    75,    76,    77,    78,     4,    80,
     206,    74,     4,   185,   107,   108,   224,   225,   234,   235,
     236,   237,   238,   226,     4,   255,   256,   105,   218,     9,
     106,    62,   257,   110,   115,   116,   115,   116,   109,   178,
     179,   180,   181,   182,   128,    81,   103,   104,   105,   132,
     124,   106,   123,   120,    68,    70,   119,   120,    72,   115,
     116,   136,   137,    67,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    56,
      57,   106,   168,   133,   166,   111,   112,   169,   175,   167,
     113,   114,   134,   135,   109,   173,   202,   176,   177,   187,
     240,   189,     4,   191,   185,   188,   194,   190,   198,   139,
     140,   192,   200,   167,   261,   262,   227,   178,   179,   180,
     181,   182,   196,   199,   201,   204,   205,   216,   215,   220,
     241,   221,   230,   116,   222,   239,     2,     3,     4,     5,
       6,     7,     8,   120,   254,    73,   231,    42,   243,   209,
      64,     0,     0,     0,     0,     0,     0,     0,   210,   211,
     212,   213,   214,     0,   223,     0,   217,    17,    18,     0,
      19,   228,     0,   229,   260,     0,     0,   169,   232,     0,
     258,   259,     0,    22,    23,   173,     0,     0,     0,     0,
     242,     0,     0,    24,    25,     0,   187,     0,    27,   174,
     187,     0,   187,     0,     0,   249,   250,     0,     0,     0,
       0,     0,     0,   253,     2,     3,     4,     5,     6,     7,
       8,     0,     0,    11,     0,    12,     0,    13,    14,    15,
      16,     0,     0,     0,     0,   187,   187,     0,     0,     0,
       0,     0,     0,     0,     0,    17,    18,     0,    19,     0,
      91,     0,    93,     0,    95,    96,    97,    98,    99,   100,
       0,    22,    23,   101,   102,   103,   104,   105,     0,     0,
     106,    24,    25,     0,     0,     0,    27,     0,     1,   125,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
       0,    12,     0,    13,    14,    15,    16,     0,     2,     3,
       4,     5,     6,     7,     8,     0,     0,     0,     0,     0,
       0,    17,    18,     0,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,    21,     0,    22,    23,    17,
      18,     0,    19,     0,     0,     0,     0,    24,    25,    26,
       0,     0,    27,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,    24,    25,     0,     0,     0,
      27,   208,     2,     3,     4,     5,     6,     7,     8,     0,
       0,    11,     0,    12,     0,    13,    14,    15,    16,     0,
       2,     3,     4,     5,     6,     7,     8,     0,     0,     0,
       0,     0,     0,    17,    18,     0,    19,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
      23,    17,    18,     0,    19,     0,     0,     0,     0,    24,
      25,     0,     0,     0,    27,     0,     0,    22,    23,     0,
       0,     0,     0,     0,     0,     0,     0,    24,    25,     0,
       0,     0,    27,    82,    83,    84,    85,    86,    87,    88,
      89,    90,     0,     0,     0,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
     103,   104,   105,     0,     0,   106,     0,     0,     0,     0,
       0,     0,     0,     0,   203,    82,    83,    84,    85,    86,
      87,    88,    89,    90,     0,     0,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,     0,     0,     0,
     101,   102,   103,   104,   105,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,   207,    82,    83,    84,
      85,    86,    87,    88,    89,    90,     0,     0,     0,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,     0,
       0,     0,   101,   102,   103,   104,   105,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,     0,   233,    82,
      83,    84,    85,    86,    87,    88,    89,    90,     0,     0,
       0,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,     0,     0,     0,   101,   102,   103,   104,   105,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,     0,
     251,    82,    83,    84,    85,    86,    87,    88,    89,    90,
       0,     0,     0,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,     0,     0,     0,   101,   102,   103,   104,
     105,     0,     0,   106,     0,     0,     0,     0,     0,     0,
       0,     0,   252,    82,    83,    84,    85,    86,    87,    88,
      89,    90,     0,     0,     0,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
     103,   104,   105,     0,     0,   106,     0,     0,     0,     0,
       0,     0,   138,    82,    83,    84,    85,    86,    87,    88,
      89,    90,     0,     0,     0,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
     103,   104,   105,     0,     0,   106,     0,     0,     0,     0,
       0,     0,   195,    82,    83,    84,    85,    86,    87,    88,
      89,    90,     0,     0,     0,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
     103,   104,   105,     0,     0,   106,     0,     0,     0,     0,
       0,     0,   197,    82,    83,    84,    85,    86,    87,    88,
      89,    90,     0,     0,     0,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
     103,   104,   105,     0,     0,   106,     0,     0,     0,     0,
       0,     0,   248,    82,    83,    84,    85,    86,    87,    88,
      89,    90,     0,     0,     0,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
     103,   104,   105,     0,     0,   106,    83,    84,    85,    86,
      87,    88,    89,    90,     0,     0,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,     0,     0,     0,
     101,   102,   103,   104,   105,     0,     0,   106,    84,    85,
      86,    87,    88,    89,    90,     0,     0,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,     0,     0,
       0,   101,   102,   103,   104,   105,     0,     0,   106,    85,
      86,    87,    88,    89,    90,     0,     0,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,     0,     0,
       0,   101,   102,   103,   104,   105,     0,     0,   106,    86,
      87,    88,    89,    90,     0,     0,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,     0,     0,     0,
     101,   102,   103,   104,   105,     0,     0,   106,    87,    88,
      89,    90,     0,     0,     0,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
     103,   104,   105,     0,     0,   106,    88,    89,    90,     0,
       0,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,     0,     0,     0,   101,   102,   103,   104,   105,
      89,    90,   106,     0,     0,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
     103,   104,   105,    90,     0,   106,     0,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,     0,     0,     0,
     101,   102,   103,   104,   105,     0,     0,   106,    95,    96,
      97,    98,    99,   100,     0,     0,     0,   101,   102,   103,
     104,   105,     0,     0,   106,   -98,   -98,   -98,   -98,   -98,
     -98,     0,     0,     0,   101,   102,   103,   104,   105,     0,
       0,   106
};

static const yytype_int16 yycheck[] =
{
       0,     0,    53,    63,    64,   187,    60,     5,    16,     9,
      10,    19,    17,    18,    22,    23,    24,    25,     5,    27,
      36,    21,     5,   119,    34,    35,    62,    63,   210,   211,
     212,   213,   214,    69,     5,    62,    63,    54,    36,    10,
      57,    60,    69,    61,    60,    61,    60,    61,    58,    20,
      21,    22,    23,    24,    62,     0,    52,    53,    54,    67,
      60,    57,    54,    61,    17,    18,    60,    61,    21,    60,
      61,    62,    63,    60,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,    62,
      63,    57,   110,    58,   109,    62,    63,   115,   116,   109,
      62,    63,    62,    63,    58,   115,   167,   117,    61,   119,
     216,   121,     5,   123,   220,    66,   125,    61,   133,    66,
      67,    13,   140,   133,   255,   256,   196,    20,    21,    22,
      23,    24,    15,    61,    61,    64,    67,    67,    64,    60,
      36,    66,   203,    61,    68,   215,     3,     4,     5,     6,
       7,     8,     9,    61,    64,    21,   205,     0,   220,   177,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,
     180,   181,   182,    -1,   192,    -1,   186,    34,    35,    -1,
      37,   199,    -1,   201,   254,    -1,    -1,   205,   206,    -1,
     251,   252,    -1,    50,    51,   205,    -1,    -1,    -1,    -1,
     218,    -1,    -1,    60,    61,    -1,   216,    -1,    65,    66,
     220,    -1,   222,    -1,    -1,   224,   225,    -1,    -1,    -1,
      -1,    -1,    -1,   241,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    12,    -1,    14,    -1,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,   255,   256,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      37,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      -1,    50,    51,    50,    51,    52,    53,    54,    -1,    -1,
      57,    60,    61,    -1,    -1,    -1,    65,    -1,     1,    68,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      -1,    14,    -1,    16,    17,    18,    19,    -1,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    48,    -1,    50,    51,    34,
      35,    -1,    37,    -1,    -1,    -1,    -1,    60,    61,    62,
      -1,    -1,    65,    -1,    -1,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,
      65,    66,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    12,    -1,    14,    -1,    16,    17,    18,    19,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    35,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    34,    35,    -1,    37,    -1,    -1,    -1,    -1,    60,
      61,    -1,    -1,    -1,    65,    -1,    -1,    50,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
      -1,    -1,    65,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    -1,    -1,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    -1,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    -1,    -1,    50,    51,    52,    53,    54,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    -1,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    -1,    -1,    50,    51,    52,    53,
      54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    -1,    -1,    57,    26,    27,    28,    29,
      30,    31,    32,    33,    -1,    -1,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    -1,    -1,    57,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      -1,    50,    51,    52,    53,    54,    -1,    -1,    57,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      -1,    50,    51,    52,    53,    54,    -1,    -1,    57,    29,
      30,    31,    32,    33,    -1,    -1,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    -1,    -1,    57,    30,    31,
      32,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    -1,    -1,    57,    31,    32,    33,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    50,    51,    52,    53,    54,
      32,    33,    57,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    33,    -1,    57,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    -1,    -1,    57,    41,    42,
      43,    44,    45,    46,    -1,    -1,    -1,    50,    51,    52,
      53,    54,    -1,    -1,    57,    41,    42,    43,    44,    45,
      46,    -1,    -1,    -1,    50,    51,    52,    53,    54,    -1,
      -1,    57
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    14,    16,    17,    18,    19,    34,    35,    37,
      47,    48,    50,    51,    60,    61,    62,    65,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      85,    86,    90,    99,   100,   101,   104,   105,   107,   109,
     110,   111,   112,   113,   114,   116,    62,    63,   113,   113,
      60,   108,    60,   106,   106,    72,   113,    60,    82,    85,
      82,    72,    82,    86,   113,    72,    72,    72,    72,   115,
      72,     0,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    50,    51,    52,    53,    54,    57,    34,    35,    58,
      61,    62,    63,    62,    63,    60,    61,    83,    84,    60,
      61,    95,    96,    54,   113,    68,    97,   100,    72,    97,
      97,    83,    72,    58,    62,    63,    62,    63,    64,    66,
      67,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    85,   113,    72,    72,
      87,    88,    89,   113,    66,    72,   113,    61,    20,    21,
      22,    23,    24,    91,    92,    93,    94,   113,    66,   113,
      61,   113,    13,    98,   100,    64,    15,    64,    85,    61,
      72,    61,    83,    66,    64,    67,    36,    66,    66,    72,
     113,   113,   113,   113,   113,    64,    67,   113,    36,    95,
      60,    66,    68,    72,    62,    63,    69,    97,    72,    72,
      83,    89,    72,    66,    95,    95,    95,    95,    95,    97,
      93,    36,    72,    91,    90,    93,   102,   103,    64,   100,
     100,    66,    66,    72,    64,    62,    63,    69,    83,    83,
      97,   103,   103
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
      case 72: /* "expression" */
#line 136 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1642 "./grammar.tab.c"
	break;
      case 73: /* "arrowAssign" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1647 "./grammar.tab.c"
	break;
      case 74: /* "tildeAssign" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1652 "./grammar.tab.c"
	break;
      case 75: /* "equationAssign" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1657 "./grammar.tab.c"
	break;
      case 76: /* "controlAssign" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1662 "./grammar.tab.c"
	break;
      case 77: /* "referenceAssign" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1667 "./grammar.tab.c"
	break;
      case 78: /* "additionAssign" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1672 "./grammar.tab.c"
	break;
      case 79: /* "subtractionAssign" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1677 "./grammar.tab.c"
	break;
      case 80: /* "multiplicationAssign" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1682 "./grammar.tab.c"
	break;
      case 81: /* "divisionAssign" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1687 "./grammar.tab.c"
	break;
      case 82: /* "variable" */
#line 135 "./grammar.y"
	{ delete ((yyvaluep->syntaxVariable)); };
#line 1692 "./grammar.tab.c"
	break;
      case 83: /* "optElements" */
#line 131 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1697 "./grammar.tab.c"
	break;
      case 84: /* "elementList" */
#line 131 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1702 "./grammar.tab.c"
	break;
      case 85: /* "fxnCall" */
#line 135 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); };
#line 1707 "./grammar.tab.c"
	break;
      case 86: /* "functionCall" */
#line 135 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); };
#line 1712 "./grammar.tab.c"
	break;
      case 87: /* "optArguments" */
#line 132 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1717 "./grammar.tab.c"
	break;
      case 88: /* "argumentList" */
#line 132 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1722 "./grammar.tab.c"
	break;
      case 89: /* "argument" */
#line 135 "./grammar.y"
	{ delete ((yyvaluep->syntaxLabeledExpr)); };
#line 1727 "./grammar.tab.c"
	break;
      case 90: /* "functionDef" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1732 "./grammar.tab.c"
	break;
      case 91: /* "optFormals" */
#line 133 "./grammar.y"
	{ for (std::list<SyntaxFormal*>::iterator it=(yyvaluep->formalList)->begin(); it != (yyvaluep->formalList)->end(); it++) { SyntaxFormal* theElement = *it; delete theElement; }; delete ((yyvaluep->formalList)); };
#line 1737 "./grammar.tab.c"
	break;
      case 92: /* "formalList" */
#line 133 "./grammar.y"
	{ for (std::list<SyntaxFormal*>::iterator it=(yyvaluep->formalList)->begin(); it != (yyvaluep->formalList)->end(); it++) { SyntaxFormal* theElement = *it; delete theElement; }; delete ((yyvaluep->formalList)); };
#line 1742 "./grammar.tab.c"
	break;
      case 93: /* "formal" */
#line 135 "./grammar.y"
	{ delete ((yyvaluep->syntaxFormal)); };
#line 1747 "./grammar.tab.c"
	break;
      case 94: /* "typeSpec" */
#line 134 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1752 "./grammar.tab.c"
	break;
      case 95: /* "optDims" */
#line 134 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1757 "./grammar.tab.c"
	break;
      case 96: /* "dimList" */
#line 134 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1762 "./grammar.tab.c"
	break;
      case 97: /* "stmts" */
#line 131 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1767 "./grammar.tab.c"
	break;
      case 98: /* "stmtList" */
#line 131 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1772 "./grammar.tab.c"
	break;
      case 99: /* "statement" */
#line 136 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1777 "./grammar.tab.c"
	break;
      case 100: /* "stmt_or_expr" */
#line 136 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1782 "./grammar.tab.c"
	break;
      case 101: /* "declaration" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1787 "./grammar.tab.c"
	break;
      case 102: /* "memberDefs" */
#line 131 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1792 "./grammar.tab.c"
	break;
      case 103: /* "memberDef" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1797 "./grammar.tab.c"
	break;
      case 104: /* "classDef" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1802 "./grammar.tab.c"
	break;
      case 105: /* "ifStatement" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1807 "./grammar.tab.c"
	break;
      case 106: /* "cond" */
#line 142 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1812 "./grammar.tab.c"
	break;
      case 107: /* "forStatement" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1817 "./grammar.tab.c"
	break;
      case 108: /* "forCond" */
#line 142 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1822 "./grammar.tab.c"
	break;
      case 109: /* "whileStatement" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1827 "./grammar.tab.c"
	break;
      case 110: /* "nextStatement" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1832 "./grammar.tab.c"
	break;
      case 111: /* "breakStatement" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1837 "./grammar.tab.c"
	break;
      case 112: /* "returnStatement" */
#line 142 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1842 "./grammar.tab.c"
	break;
      case 113: /* "identifier" */
#line 134 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1847 "./grammar.tab.c"
	break;
      case 114: /* "vector" */
#line 132 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1852 "./grammar.tab.c"
	break;
      case 115: /* "vectorList" */
#line 132 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1857 "./grammar.tab.c"
	break;
      case 116: /* "constant" */
#line 135 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1862 "./grammar.tab.c"
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
#line 230 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison encountered end_of_input; ignored\n");
#endif
                    return 0;
                }
    break;

  case 3:
#line 237 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison encountered newline; ignored\n");
#endif
                    return 0;
                }
    break;

  case 4:
#line 244 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute statement or expression\n");
#endif
                    int rv = parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    return rv;
                }
    break;

  case 5:
#line 252 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute statement or expression\n");
#endif
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    return rv;
                }
    break;

  case 6:
#line 260 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute declaration\n");
#endif
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    return rv;
                }
    break;

  case 7:
#line 268 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute declaration\n");
#endif
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    return rv;
                }
    break;

  case 8:
#line 276 "./grammar.y"
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
#line 285 "./grammar.y"
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
#line 294 "./grammar.y"
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
#line 303 "./grammar.y"
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
#line 312 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
#endif
                    YYABORT;
                }
    break;

  case 13:
#line 319 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
#endif
                    YYABORT;
                }
    break;

  case 14:
#line 327 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 15:
#line 329 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxFunctionCall("v", (yyvsp[(1) - (1)].argumentList)); }
    break;

  case 16:
#line 331 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 17:
#line 333 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UMinus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 18:
#line 334 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UPlus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 19:
#line 335 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UNot, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 20:
#line 336 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UAnd, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 21:
#line 338 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxDecrement( (yyvsp[(2) - (2)].syntaxVariable) ); }
    break;

  case 22:
#line 339 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxDecrement( (yyvsp[(1) - (2)].syntaxVariable) ); }
    break;

  case 23:
#line 340 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxIncrement( (yyvsp[(2) - (2)].syntaxVariable) ); }
    break;

  case 24:
#line 341 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxIncrement( (yyvsp[(1) - (2)].syntaxVariable) ); }
    break;

  case 25:
#line 343 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Range, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 26:
#line 345 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Add, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 27:
#line 346 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Sub, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 28:
#line 347 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Mul, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 29:
#line 348 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Div, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 30:
#line 349 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Exp, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 31:
#line 351 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Lt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 32:
#line 352 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Le, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 33:
#line 353 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Eq, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 34:
#line 354 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ne, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 35:
#line 355 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ge, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 36:
#line 356 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Gt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 37:
#line 358 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 38:
#line 359 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 39:
#line 360 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 40:
#line 361 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 41:
#line 363 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 42:
#line 364 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 43:
#line 365 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 44:
#line 366 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 45:
#line 367 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 46:
#line 369 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 47:
#line 370 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 48:
#line 371 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 49:
#line 372 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 50:
#line 374 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFunctionCall); }
    break;

  case 51:
#line 376 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxVariable); }
    break;

  case 52:
#line 380 "./grammar.y"
    { 
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxConstantAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 53:
#line 389 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxStochasticAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 54:
#line 398 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxDeterministicAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 55:
#line 407 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting control assignment (CONTROL_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxControlAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 56:
#line 416 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting reference assignment (REFERENCE_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxReferenceAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 57:
#line 425 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting addition assignment (ADDITION_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxAdditionAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 58:
#line 434 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                            printf("Parser inserting subtraction assignment (SUBTRACTION_ASSIGN) in syntax tree\n");
#endif
                            (yyval.syntaxElement) = new SyntaxSubtractionAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                        }
    break;

  case 59:
#line 443 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                                printf("Parser inserting multiplication assignment (MULTIPLICATION_ASSIGN) in syntax tree\n");
#endif
                                (yyval.syntaxElement) = new SyntaxMultiplicationAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                            }
    break;

  case 60:
#line 452 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting division assignment (DIVISION_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxDivisionAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 61:
#line 461 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting variable (NAMED_VAR) in syntax tree\n");
#endif
                    (yyval.syntaxVariable) = new SyntaxVariable(*(yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].syntaxElementList));
                    delete (yyvsp[(1) - (2)].string);
                }
    break;

  case 62:
#line 469 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting variable (FUNCTION_VAR) in syntax tree\n");
#endif
                    (yyvsp[(5) - (5)].syntaxElementList)->push_front((yyvsp[(3) - (5)].syntaxElement));
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (5)].syntaxFunctionCall), (yyvsp[(5) - (5)].syntaxElementList));
                }
    break;

  case 63:
#line 477 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting variable (EXPRESSION_VAR) in syntax tree\n");
#endif
                    (yyvsp[(7) - (7)].syntaxElementList)->push_front((yyvsp[(5) - (7)].syntaxElement));
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(2) - (7)].syntaxElement), (yyvsp[(7) - (7)].syntaxElementList));
                }
    break;

  case 64:
#line 485 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting member variable (NAMED_VAR) in syntax tree\n");
#endif
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (4)].syntaxVariable), *(yyvsp[(3) - (4)].string), (yyvsp[(4) - (4)].syntaxElementList));
                    delete (yyvsp[(3) - (4)].string);
                }
    break;

  case 65:
#line 493 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting member variable (FUNCTION_VAR) in syntax tree\n");
#endif
                    (yyvsp[(7) - (7)].syntaxElementList)->push_front((yyvsp[(5) - (7)].syntaxElement));
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (7)].syntaxVariable), (yyvsp[(3) - (7)].syntaxFunctionCall), (yyvsp[(7) - (7)].syntaxElementList));
                }
    break;

  case 66:
#line 502 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 67:
#line 503 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (1)].syntaxElementList); }
    break;

  case 68:
#line 506 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(2) - (3)].syntaxElement)); }
    break;

  case 69:
#line 507 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 70:
#line 508 "./grammar.y"
    { (yyvsp[(1) - (4)].syntaxElementList)->push_back((yyvsp[(3) - (4)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (4)].syntaxElementList); }
    break;

  case 71:
#line 509 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back( NULL ); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 72:
#line 513 "./grammar.y"
    {
                    (yyval.syntaxFunctionCall) = new SyntaxFunctionCall(*(yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].argumentList));
                    delete (yyvsp[(1) - (4)].string);
                }
    break;

  case 73:
#line 520 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting function call in syntax tree\n");
#endif
                        (yyval.syntaxFunctionCall) = (yyvsp[(1) - (1)].syntaxFunctionCall);
                    }
    break;

  case 74:
#line 527 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting member call in syntax tree\n");
#endif
                        (yyvsp[(3) - (3)].syntaxFunctionCall)->setBaseVariable((yyvsp[(1) - (3)].syntaxVariable));
                        (yyval.syntaxFunctionCall) = (yyvsp[(3) - (3)].syntaxFunctionCall);
                    }
    break;

  case 75:
#line 536 "./grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(); }
    break;

  case 76:
#line 537 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(1) - (1)].argumentList); }
    break;

  case 77:
#line 540 "./grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(1,(yyvsp[(1) - (1)].syntaxLabeledExpr)); }
    break;

  case 78:
#line 541 "./grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back((yyvsp[(3) - (3)].syntaxLabeledExpr)); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 79:
#line 545 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting unlabeled argument in syntax tree\n");
#endif
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr( "", (yyvsp[(1) - (1)].syntaxElement));
                }
    break;

  case 80:
#line 552 "./grammar.y"
    { 
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting labeled argument in syntax tree\n");
#endif
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr(*(yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].syntaxElement));
                    delete (yyvsp[(1) - (3)].string);
                }
    break;

  case 81:
#line 562 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting function definition in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxFunctionDef("", *(yyvsp[(2) - (6)].string), (yyvsp[(4) - (6)].formalList), (yyvsp[(6) - (6)].syntaxElementList));
                    delete (yyvsp[(2) - (6)].string);
                }
    break;

  case 82:
#line 571 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting typed function definition in syntax tree\n");
#endif
                    (yyvsp[(2) - (8)].string)->append(*(yyvsp[(3) - (8)].string));
                    (yyval.syntaxElement) = new SyntaxFunctionDef(*(yyvsp[(2) - (8)].string), *(yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].formalList), (yyvsp[(8) - (8)].syntaxElementList));
                    delete (yyvsp[(2) - (8)].string);
                    delete (yyvsp[(3) - (8)].string);
                    delete (yyvsp[(4) - (8)].string);
                }
    break;

  case 83:
#line 583 "./grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal*>(); }
    break;

  case 84:
#line 584 "./grammar.y"
    { (yyval.formalList) = (yyvsp[(1) - (1)].formalList); }
    break;

  case 85:
#line 587 "./grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal*>(1, (yyvsp[(1) - (1)].syntaxFormal)); }
    break;

  case 86:
#line 588 "./grammar.y"
    { (yyvsp[(1) - (3)].formalList)->push_back((yyvsp[(3) - (3)].syntaxFormal)); (yyval.formalList) = (yyvsp[(1) - (3)].formalList); }
    break;

  case 87:
#line 592 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting labeled formal argument without default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (1)].string), NULL );
                    delete (yyvsp[(1) - (1)].string);
                }
    break;

  case 88:
#line 600 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting labeled formal argument with default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].syntaxElement));
                    delete (yyvsp[(1) - (3)].string);
                }
    break;

  case 89:
#line 608 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting typed labeled formal argument without default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (2)].string), *(yyvsp[(2) - (2)].string), NULL);
                    delete (yyvsp[(1) - (2)].string);
                    delete (yyvsp[(2) - (2)].string);
                }
    break;

  case 90:
#line 617 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting typed labeled formal argument with default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (4)].string), *(yyvsp[(2) - (4)].string), (yyvsp[(4) - (4)].syntaxElement));
                    delete (yyvsp[(1) - (4)].string);
                    delete (yyvsp[(2) - (4)].string);
                }
    break;

  case 91:
#line 627 "./grammar.y"
    { (yyvsp[(1) - (2)].string)->append(*((yyvsp[(2) - (2)].string))); delete (yyvsp[(2) - (2)].string); (yyval.string) = (yyvsp[(1) - (2)].string); }
    break;

  case 92:
#line 628 "./grammar.y"
    { (yyvsp[(2) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); (yyvsp[(2) - (3)].string)->insert(0, "const ");           delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 93:
#line 629 "./grammar.y"
    { (yyvsp[(2) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); (yyvsp[(2) - (3)].string)->insert(0, "dynamic ");         delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 94:
#line 630 "./grammar.y"
    { (yyvsp[(2) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); (yyvsp[(2) - (3)].string)->insert(0, "stochastic ");      delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 95:
#line 631 "./grammar.y"
    { (yyvsp[(2) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); (yyvsp[(2) - (3)].string)->insert(0, "deterministic ");   delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 96:
#line 632 "./grammar.y"
    { (yyvsp[(2) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); (yyvsp[(2) - (3)].string)->insert(0, "mutable ");         delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 97:
#line 635 "./grammar.y"
    { (yyval.string) = new std::string(""); }
    break;

  case 98:
#line 636 "./grammar.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); }
    break;

  case 99:
#line 639 "./grammar.y"
    { (yyval.string) = new std::string("[]"); }
    break;

  case 100:
#line 640 "./grammar.y"
    { (yyvsp[(1) - (3)].string)->append("[]"); (yyval.string) = (yyvsp[(1) - (3)].string); }
    break;

  case 101:
#line 643 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 102:
#line 645 "./grammar.y"
    {
                    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();
                    stmts->push_back((yyvsp[(1) - (1)].syntaxElement));
                    (yyval.syntaxElementList) = stmts;
                }
    break;

  case 103:
#line 652 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 104:
#line 653 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 105:
#line 654 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 106:
#line 655 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 107:
#line 656 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 108:
#line 657 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 109:
#line 660 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 110:
#line 661 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 111:
#line 662 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 112:
#line 663 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 113:
#line 664 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 114:
#line 665 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 115:
#line 668 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 116:
#line 669 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 117:
#line 672 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 118:
#line 673 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 119:
#line 675 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting variable declaration in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxVariableDecl(*(yyvsp[(1) - (3)].string), (yyvsp[(2) - (3)].syntaxElementList), *(yyvsp[(3) - (3)].string));
                        delete (yyvsp[(1) - (3)].string);
                        delete (yyvsp[(3) - (3)].string);
                    }
    break;

  case 120:
#line 685 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 121:
#line 686 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 122:
#line 687 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 123:
#line 688 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 124:
#line 689 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 125:
#line 690 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 126:
#line 693 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFormal); }
    break;

  case 127:
#line 694 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 128:
#line 698 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                printf("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxClassDef(*(yyvsp[(2) - (7)].string), *(yyvsp[(4) - (7)].string), (yyvsp[(6) - (7)].syntaxElementList));
                    delete (yyvsp[(2) - (7)].string);
                    delete (yyvsp[(4) - (7)].string);
                }
    break;

  case 129:
#line 708 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::If, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 130:
#line 709 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::IfElse, (yyvsp[(2) - (5)].syntaxElement), (yyvsp[(3) - (5)].syntaxElementList), (yyvsp[(5) - (5)].syntaxElementList)); }
    break;

  case 131:
#line 711 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 132:
#line 714 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::For, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 133:
#line 717 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxForLoop(*(yyvsp[(2) - (5)].string), (yyvsp[(4) - (5)].syntaxElement)); delete (yyvsp[(2) - (5)].string); }
    break;

  case 134:
#line 720 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::While, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 135:
#line 723 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Next); }
    break;

  case 136:
#line 726 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Break); }
    break;

  case 137:
#line 729 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return); }
    break;

  case 138:
#line 730 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 139:
#line 733 "./grammar.y"
    { (yyval.string) = new std::string((yyvsp[(1) - (1)].c_string)); }
    break;

  case 140:
#line 737 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(2) - (3)].argumentList); }
    break;

  case 141:
#line 740 "./grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back(new SyntaxLabeledExpr( "", (yyvsp[(3) - (3)].syntaxElement)) ); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 142:
#line 742 "./grammar.y"
    {
                (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(1, new SyntaxLabeledExpr("", (yyvsp[(1) - (1)].syntaxElement)) );
                }
    break;

  case 143:
#line 748 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting bool constant (false) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant(new RlBoolean(false) );
                }
    break;

  case 144:
#line 755 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting bool constant (true) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant(new RlBoolean(true) );
                }
    break;

  case 145:
#line 762 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting null constant in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant( NULL );
                }
    break;

  case 146:
#line 769 "./grammar.y"
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

  case 147:
#line 784 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting String constant in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant(new RlString((yyvsp[(1) - (1)].c_string)) );
                }
    break;

  case 148:
#line 791 "./grammar.y"
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
#line 3237 "./grammar.tab.c"
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


#line 844 "./grammar.y"



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



