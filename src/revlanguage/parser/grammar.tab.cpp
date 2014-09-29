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
     PROCEDURE = 266,
     CLASS = 267,
     FOR = 268,
     IN = 269,
     IF = 270,
     ELSE = 271,
     WHILE = 272,
     NEXT = 273,
     BREAK = 274,
     RETURN = 275,
     MOD_CONST = 276,
     MOD_DYNAMIC = 277,
     MOD_STOCHASTIC = 278,
     MOD_DETERMINISTIC = 279,
     PROTECTED = 280,
     ARROW_ASSIGN = 281,
     TILDE_ASSIGN = 282,
     EQUATION_ASSIGN = 283,
     CONTROL_ASSIGN = 284,
     REFERENCE_ASSIGN = 285,
     ADDITION_ASSIGN = 286,
     SUBTRACTION_ASSIGN = 287,
     MULTIPLICATION_ASSIGN = 288,
     DIVISION_ASSIGN = 289,
     DECREMENT = 290,
     INCREMENT = 291,
     EQUAL = 292,
     AND = 293,
     OR = 294,
     AND2 = 295,
     OR2 = 296,
     GT = 297,
     GE = 298,
     LT = 299,
     LE = 300,
     EQ = 301,
     NE = 302,
     END_OF_INPUT = 303,
     UNOT = 304,
     UPLUS = 305,
     UMINUS = 306,
     UAND = 307
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
#define PROCEDURE 266
#define CLASS 267
#define FOR 268
#define IN 269
#define IF 270
#define ELSE 271
#define WHILE 272
#define NEXT 273
#define BREAK 274
#define RETURN 275
#define MOD_CONST 276
#define MOD_DYNAMIC 277
#define MOD_STOCHASTIC 278
#define MOD_DETERMINISTIC 279
#define PROTECTED 280
#define ARROW_ASSIGN 281
#define TILDE_ASSIGN 282
#define EQUATION_ASSIGN 283
#define CONTROL_ASSIGN 284
#define REFERENCE_ASSIGN 285
#define ADDITION_ASSIGN 286
#define SUBTRACTION_ASSIGN 287
#define MULTIPLICATION_ASSIGN 288
#define DIVISION_ASSIGN 289
#define DECREMENT 290
#define INCREMENT 291
#define EQUAL 292
#define AND 293
#define OR 294
#define AND2 295
#define OR2 296
#define GT 297
#define GE 298
#define LT 299
#define LE 300
#define EQ 301
#define NE 302
#define END_OF_INPUT 303
#define UNOT 304
#define UPLUS 305
#define UMINUS 306
#define UAND 307




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
#include "Probability.h"
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
#include "SyntaxIndexOperation.h"
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
#line 80 "./grammar.y"
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
#line 294 "./grammar.tab.c"
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
#line 319 "./grammar.tab.c"

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
#define YYFINAL  84
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1176

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  153
/* YYNRULES -- Number of states.  */
#define YYNSTATES  278

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   307

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      64,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,     2,     2,     2,    56,     2,     2,
      62,    66,    53,    51,    69,    52,    60,    54,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    55,    65,
       2,     2,     2,    49,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    63,     2,    68,    59,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    70,     2,    71,     2,     2,     2,     2,
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
      45,    46,    47,    48,    50,    57,    58,    61
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
     133,   137,   141,   143,   145,   147,   149,   151,   153,   155,
     157,   159,   161,   163,   167,   171,   175,   179,   183,   187,
     191,   195,   199,   202,   208,   216,   221,   229,   230,   232,
     236,   239,   244,   248,   253,   255,   259,   260,   262,   264,
     268,   270,   274,   281,   290,   297,   306,   307,   309,   311,
     315,   317,   321,   324,   329,   332,   336,   340,   344,   348,
     349,   351,   354,   358,   362,   364,   365,   367,   371,   374,
     378,   381,   383,   385,   387,   389,   391,   393,   395,   397,
     399,   401,   403,   407,   408,   410,   414,   417,   421,   424,
     426,   429,   431,   433,   441,   445,   451,   455,   459,   465,
     469,   471,   473,   475,   478,   480,   484,   488,   490,   492,
     494,   496,   498,   500
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      73,     0,    -1,    48,    -1,    64,    -1,   103,    64,    -1,
     103,    65,    -1,   104,    64,    -1,   104,    65,    -1,    49,
     116,    64,    -1,    49,   116,    65,    -1,    49,    88,    64,
      -1,    49,    88,    65,    -1,     1,    64,    -1,     1,    65,
      -1,   119,    -1,   117,    -1,    62,    74,    66,    -1,    52,
      74,    -1,    51,    74,    -1,    67,    74,    -1,    38,    74,
      -1,    35,    84,    -1,    84,    35,    -1,    36,    84,    -1,
      84,    36,    -1,    74,    55,    74,    -1,    74,    51,    74,
      -1,    74,    52,    74,    -1,    74,    53,    74,    -1,    74,
      54,    74,    -1,    74,    59,    74,    -1,    74,    56,    74,
      -1,    74,    44,    74,    -1,    74,    45,    74,    -1,    74,
      46,    74,    -1,    74,    47,    74,    -1,    74,    43,    74,
      -1,    74,    42,    74,    -1,    74,    38,    74,    -1,    74,
      39,    74,    -1,    74,    40,    74,    -1,    74,    41,    74,
      -1,    75,    -1,    77,    -1,    76,    -1,    78,    -1,    79,
      -1,    80,    -1,    81,    -1,    82,    -1,    83,    -1,    88,
      -1,    84,    -1,    74,    26,    74,    -1,    74,    27,    74,
      -1,    74,    28,    74,    -1,    74,    29,    74,    -1,    74,
      30,    74,    -1,    74,    31,    74,    -1,    74,    32,    74,
      -1,    74,    33,    74,    -1,    74,    34,    74,    -1,   116,
      85,    -1,    87,    63,    74,    68,    85,    -1,    62,    74,
      66,    63,    74,    68,    85,    -1,    84,    60,   116,    85,
      -1,    84,    60,    87,    63,    74,    68,    85,    -1,    -1,
      86,    -1,    63,    74,    68,    -1,    63,    68,    -1,    86,
      63,    74,    68,    -1,    86,    63,    68,    -1,   116,    62,
      89,    66,    -1,    87,    -1,    84,    60,    87,    -1,    -1,
      90,    -1,    91,    -1,    90,    69,    91,    -1,    74,    -1,
     116,    37,    74,    -1,    10,   116,    62,    94,    66,   100,
      -1,    10,   116,    98,   116,    62,    94,    66,   100,    -1,
      11,   116,    62,    94,    66,   100,    -1,    11,   116,    98,
     116,    62,    94,    66,   100,    -1,    -1,    95,    -1,    96,
      -1,    95,    69,    96,    -1,   116,    -1,   116,    37,    74,
      -1,    97,   116,    -1,    97,   116,    37,    74,    -1,   116,
      98,    -1,    21,   116,    98,    -1,    22,   116,    98,    -1,
      23,   116,    98,    -1,    24,   116,    98,    -1,    -1,    99,
      -1,    63,    68,    -1,    99,    63,    68,    -1,    70,   101,
      71,    -1,   103,    -1,    -1,   103,    -1,   101,    65,   103,
      -1,   101,    65,    -1,   101,    64,   103,    -1,   101,    64,
      -1,   108,    -1,   110,    -1,   112,    -1,   113,    -1,   114,
      -1,   115,    -1,   102,    -1,    74,    -1,   107,    -1,    92,
      -1,    93,    -1,   116,    85,   116,    -1,    -1,   106,    -1,
     105,    65,   106,    -1,   105,    65,    -1,   105,    64,   106,
      -1,   105,    64,    -1,    96,    -1,    25,    96,    -1,    92,
      -1,    93,    -1,    12,   116,    55,   116,    70,   105,    71,
      -1,    15,   109,   100,    -1,    15,   109,   100,    16,   100,
      -1,    62,    74,    66,    -1,    13,   111,   100,    -1,    62,
     116,    14,    74,    66,    -1,    17,   109,   100,    -1,    18,
      -1,    19,    -1,    20,    -1,    20,    74,    -1,     5,    -1,
      63,   118,    68,    -1,   118,    69,    74,    -1,    74,    -1,
       8,    -1,     9,    -1,     7,    -1,     4,    -1,     6,    -1,
       3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   231,   231,   238,   245,   254,   263,   272,   281,   290,
     299,   308,   317,   324,   333,   335,   337,   339,   340,   341,
     342,   344,   345,   346,   347,   349,   351,   352,   353,   354,
     355,   356,   358,   359,   360,   361,   362,   363,   365,   366,
     367,   368,   370,   371,   372,   373,   374,   376,   377,   378,
     379,   381,   383,   386,   395,   404,   413,   422,   431,   440,
     449,   458,   467,   480,   492,   504,   517,   532,   533,   536,
     537,   538,   539,   542,   549,   556,   566,   567,   570,   571,
     574,   581,   591,   600,   613,   622,   635,   636,   639,   640,
     643,   651,   659,   668,   679,   680,   681,   682,   683,   686,
     687,   690,   691,   694,   695,   703,   704,   705,   706,   707,
     708,   711,   712,   713,   714,   715,   716,   719,   720,   723,
     724,   725,   726,   737,   738,   739,   740,   741,   742,   745,
     746,   747,   748,   751,   762,   763,   765,   768,   771,   774,
     777,   780,   783,   784,   787,   791,   794,   795,   801,   808,
     815,   822,   837,   844
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "REAL", "INT", "NAME", "STRING",
  "RBNULL", "FALSE", "TRUE", "FUNCTION", "PROCEDURE", "CLASS", "FOR", "IN",
  "IF", "ELSE", "WHILE", "NEXT", "BREAK", "RETURN", "MOD_CONST",
  "MOD_DYNAMIC", "MOD_STOCHASTIC", "MOD_DETERMINISTIC", "PROTECTED",
  "ARROW_ASSIGN", "TILDE_ASSIGN", "EQUATION_ASSIGN", "CONTROL_ASSIGN",
  "REFERENCE_ASSIGN", "ADDITION_ASSIGN", "SUBTRACTION_ASSIGN",
  "MULTIPLICATION_ASSIGN", "DIVISION_ASSIGN", "DECREMENT", "INCREMENT",
  "EQUAL", "AND", "OR", "AND2", "OR2", "GT", "GE", "LT", "LE", "EQ", "NE",
  "END_OF_INPUT", "'?'", "UNOT", "'+'", "'-'", "'*'", "'/'", "':'", "'%'",
  "UPLUS", "UMINUS", "'^'", "'.'", "UAND", "'('", "'['", "'\\n'", "';'",
  "')'", "'!'", "']'", "','", "'{'", "'}'", "$accept", "prog",
  "expression", "arrowAssign", "tildeAssign", "equationAssign",
  "controlAssign", "referenceAssign", "additionAssign",
  "subtractionAssign", "multiplicationAssign", "divisionAssign",
  "variable", "optElements", "elementList", "fxnCall", "functionCall",
  "optArguments", "argumentList", "argument", "functionDef",
  "procedureDef", "optFormals", "formalList", "formal", "typeSpec",
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,    63,
     304,    43,    45,    42,    47,    58,    37,   305,   306,    94,
      46,   307,    40,    91,    10,    59,    41,    33,    93,    44,
     123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    84,    84,    84,    84,    85,    85,    86,
      86,    86,    86,    87,    88,    88,    89,    89,    90,    90,
      91,    91,    92,    92,    93,    93,    94,    94,    95,    95,
      96,    96,    96,    96,    97,    97,    97,    97,    97,    98,
      98,    99,    99,   100,   100,   101,   101,   101,   101,   101,
     101,   102,   102,   102,   102,   102,   102,   103,   103,   104,
     104,   104,   104,   105,   105,   105,   105,   105,   105,   106,
     106,   106,   106,   107,   108,   108,   109,   110,   111,   112,
     113,   114,   115,   115,   116,   117,   118,   118,   119,   119,
     119,   119,   119,   119
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     2,     2,     1,     1,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     5,     7,     4,     7,     0,     1,     3,
       2,     4,     3,     4,     1,     3,     0,     1,     1,     3,
       1,     3,     6,     8,     6,     8,     0,     1,     1,     3,
       1,     3,     2,     4,     2,     3,     3,     3,     3,     0,
       1,     2,     3,     3,     1,     0,     1,     3,     2,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     3,     2,     3,     2,     1,
       2,     1,     1,     7,     3,     5,     3,     3,     5,     3,
       1,     1,     1,     2,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   153,   151,   144,   152,   150,   148,   149,     0,
       0,     0,     0,     0,     0,   140,   141,   142,     0,     0,
       0,     2,     0,     0,     0,     0,     0,     3,     0,     0,
     118,    42,    44,    43,    45,    46,    47,    48,    49,    50,
      52,    74,    51,   120,   121,   117,     0,     0,   119,   111,
     112,   113,   114,   115,   116,    67,    15,    14,    12,    13,
      99,    99,     0,     0,     0,     0,     0,     0,   143,    67,
       0,    21,     0,    23,    20,     0,     0,    67,    18,    17,
       0,   147,     0,    19,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,    24,     0,     0,     4,     5,     6,     7,    76,
       0,    62,    68,    86,     0,     0,   100,    86,     0,     0,
       0,   105,   137,   104,     0,   134,   139,    62,     0,     0,
      10,    11,     8,     9,    16,   145,     0,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    38,    39,    40,    41,
      37,    36,    32,    33,    34,    35,    26,    27,    28,    29,
      25,    31,    30,    75,    67,     0,    80,     0,    77,    78,
      67,    70,     0,   122,     0,     0,     0,     0,     0,     0,
      87,    88,     0,    90,   101,     0,     0,     0,     0,     0,
       0,     0,   106,   136,     0,     0,     0,     0,   146,     0,
      65,    67,    73,     0,     0,    69,    72,     0,    99,    99,
      99,    99,     0,     0,    92,     0,    94,    86,   102,     0,
      86,   123,     0,   110,   108,   103,   135,     0,     0,    63,
      79,    81,    71,    95,    96,    97,    98,    82,    89,     0,
      91,     0,    84,     0,     0,   131,   132,   129,     0,   124,
     138,   109,   107,    67,    67,    93,     0,     0,   130,   128,
     126,   133,    64,    66,    83,    85,   127,   125
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,   137,   122,    41,    42,   177,   178,   179,
     255,   256,   189,   190,   191,   192,   125,   126,   132,   201,
      45,   133,    47,   258,   259,    48,    49,    66,    50,    64,
      51,    52,    53,    54,    69,    56,    82,    57
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -200
static const yytype_int16 yypact[] =
{
     310,   -24,  -200,  -200,  -200,  -200,  -200,  -200,  -200,     9,
       9,     9,   -13,    -4,    -4,  -200,  -200,   430,    -3,    -3,
     430,  -200,    -3,   430,   430,   430,   430,  -200,   430,    61,
     851,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
     -10,    -9,  -200,  -200,  -200,  -200,   -12,    -8,  -200,  -200,
    -200,  -200,  -200,  -200,  -200,    15,  -200,  -200,  -200,  -200,
      54,    71,    24,     9,   242,   430,   242,   242,   851,    15,
     430,     4,    -9,     4,  -200,    46,    87,    11,    49,    49,
     687,   851,    85,  1099,  -200,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,  -200,  -200,     9,   430,  -200,  -200,  -200,  -200,   430,
     328,     9,    55,    22,    44,     9,    57,    22,     9,     9,
     110,   412,  -200,  -200,   728,   115,  -200,  -200,   769,     9,
    -200,  -200,  -200,  -200,    63,  -200,   430,   851,   884,   916,
     947,   977,  1006,  1034,  1058,  1081,  1099,   244,  1099,   244,
    1117,  1117,  1117,  1117,  1117,  1117,    13,    13,   -17,   -17,
      49,    49,    49,    75,    15,   472,   851,    74,    73,  -200,
     -29,  -200,   515,  -200,   346,     9,     9,     9,     9,    78,
      88,  -200,     9,    -1,  -200,    96,    92,    97,   100,    94,
     430,   -36,  -200,  -200,   242,    63,    75,   430,   851,   430,
    -200,   102,  -200,   430,   430,  -200,  -200,   558,   103,   103,
     103,   103,   242,    22,   130,   430,  -200,    22,  -200,   242,
      22,   125,   810,   412,   412,  -200,  -200,   601,   644,  -200,
    -200,   851,  -200,  -200,  -200,  -200,  -200,  -200,  -200,   430,
     851,   108,  -200,   109,    22,  -200,  -200,  -200,   -34,  -200,
    -200,  -200,  -200,   102,   102,   851,   242,   242,  -200,   125,
     125,  -200,  -200,  -200,  -200,  -200,  -200,  -200
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -200,  -200,    -5,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,    29,   -52,  -200,    -2,   147,  -200,  -200,   -37,
     177,   178,  -120,  -200,  -199,  -200,   -48,  -200,   -61,  -200,
    -200,     1,  -200,  -200,  -114,  -200,  -200,   166,  -200,  -200,
    -200,  -200,  -200,  -200,     0,  -200,  -200,  -200
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -100
static const yytype_int16 yytable[] =
{
      55,    46,     4,   121,   -99,   135,   136,   197,   214,    60,
      61,    62,    68,   128,     4,    74,    72,    72,    78,    79,
      80,    81,    77,    83,   248,   111,   112,     4,   233,   234,
     269,   270,   257,   119,   120,   235,   225,   271,   108,   109,
      58,    59,   110,   185,   186,   187,   188,    71,    73,    63,
     113,    75,   115,   116,   114,   268,   117,   118,    65,    70,
     134,    84,   124,   130,   139,   138,   106,   107,   108,   109,
     257,   257,   110,   119,   120,   142,   143,   119,   120,   129,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   113,   251,   110,   175,
     253,   173,   194,   174,   176,   182,   123,   124,   184,   180,
     196,   183,   210,   193,   200,   195,   207,   193,   198,   199,
       4,   204,   202,   127,   124,     9,    10,   206,   209,   174,
     212,   208,   213,   236,   222,   226,   185,   186,   187,   188,
     254,   140,   141,   145,   146,   276,   277,   223,   227,   239,
     228,   247,   230,   229,   231,   120,   124,   249,   252,    76,
     243,   244,   245,   246,   266,   267,   240,    43,    44,   217,
      67,     0,     0,     0,     0,   218,   219,   220,   221,     0,
       0,     0,   224,     0,     0,   232,     0,     0,     0,     0,
       0,     0,   237,     0,   238,   274,   275,     0,   176,   241,
       0,   272,   273,   180,     0,     0,     0,     0,     0,     0,
     250,     0,     0,   193,     0,     0,     0,   193,     0,     0,
     193,   193,     0,     0,   261,   262,     0,     0,     0,     0,
       0,     0,     0,     0,   265,     2,     3,     4,     5,     6,
       7,     8,     0,     0,   193,    12,     0,    13,     0,    14,
      15,    16,    17,     0,     0,     0,     0,     0,     0,   193,
     193,     0,     0,     0,     0,     0,     0,    18,    19,     0,
      20,     0,    94,     0,    96,     0,    98,    99,   100,   101,
     102,   103,     0,    23,    24,   104,   105,   106,   107,   108,
     109,     0,     0,   110,    25,    26,     0,     0,     0,    28,
       0,     1,   131,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     0,    13,     0,    14,    15,    16,
      17,     2,     3,     4,     5,     6,     7,     8,     0,     0,
       0,     0,     0,     0,     0,    18,    19,     0,    20,     2,
       3,     4,     5,     6,     7,     8,     0,     0,    21,    22,
       0,    23,    24,    18,    19,     0,    20,     0,     0,     0,
       0,     0,    25,    26,    27,     0,     0,    28,     0,    23,
      24,    18,    19,     0,    20,     0,     0,     0,     0,     0,
      25,    26,     0,     0,     0,    28,   181,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,    26,
       0,     0,     0,    28,   216,     2,     3,     4,     5,     6,
       7,     8,     0,     0,     0,    12,     0,    13,     0,    14,
      15,    16,    17,     2,     3,     4,     5,     6,     7,     8,
       0,     0,     0,     0,     0,     0,     0,    18,    19,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,    24,    18,    19,     0,    20,     0,
       0,     0,     0,     0,    25,    26,     0,     0,     0,    28,
       0,    23,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,     0,     0,     0,    28,    85,    86,
      87,    88,    89,    90,    91,    92,    93,     0,     0,     0,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,     0,     0,   104,   105,   106,   107,   108,   109,     0,
       0,   110,     0,     0,     0,     0,     0,     0,     0,     0,
     211,    85,    86,    87,    88,    89,    90,    91,    92,    93,
       0,     0,     0,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,     0,     0,   104,   105,   106,   107,
     108,   109,     0,     0,   110,     0,     0,     0,     0,     0,
       0,     0,     0,   215,    85,    86,    87,    88,    89,    90,
      91,    92,    93,     0,     0,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,     0,     0,   104,
     105,   106,   107,   108,   109,     0,     0,   110,     0,     0,
       0,     0,     0,     0,     0,     0,   242,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,     0,
       0,     0,   104,   105,   106,   107,   108,   109,     0,     0,
     110,     0,     0,     0,     0,     0,     0,     0,     0,   263,
      85,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,     0,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,     0,     0,   104,   105,   106,   107,   108,
     109,     0,     0,   110,     0,     0,     0,     0,     0,     0,
       0,     0,   264,    85,    86,    87,    88,    89,    90,    91,
      92,    93,     0,     0,     0,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,     0,     0,   104,   105,
     106,   107,   108,   109,     0,     0,   110,     0,     0,     0,
       0,     0,     0,   144,    85,    86,    87,    88,    89,    90,
      91,    92,    93,     0,     0,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,     0,     0,   104,
     105,   106,   107,   108,   109,     0,     0,   110,     0,     0,
       0,     0,     0,     0,   203,    85,    86,    87,    88,    89,
      90,    91,    92,    93,     0,     0,     0,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,     0,     0,     0,
     104,   105,   106,   107,   108,   109,     0,     0,   110,     0,
       0,     0,     0,     0,     0,   205,    85,    86,    87,    88,
      89,    90,    91,    92,    93,     0,     0,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,     0,     0,
       0,   104,   105,   106,   107,   108,   109,     0,     0,   110,
       0,     0,     0,     0,     0,     0,   260,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,     0,
       0,     0,   104,   105,   106,   107,   108,   109,     0,     0,
     110,    86,    87,    88,    89,    90,    91,    92,    93,     0,
       0,     0,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,     0,     0,   104,   105,   106,   107,   108,
     109,     0,     0,   110,    87,    88,    89,    90,    91,    92,
      93,     0,     0,     0,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,     0,     0,   104,   105,   106,
     107,   108,   109,     0,     0,   110,    88,    89,    90,    91,
      92,    93,     0,     0,     0,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,     0,     0,   104,   105,
     106,   107,   108,   109,     0,     0,   110,    89,    90,    91,
      92,    93,     0,     0,     0,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,     0,     0,   104,   105,
     106,   107,   108,   109,     0,     0,   110,    90,    91,    92,
      93,     0,     0,     0,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,     0,     0,   104,   105,   106,
     107,   108,   109,     0,     0,   110,    91,    92,    93,     0,
       0,     0,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     0,     0,     0,   104,   105,   106,   107,   108,
     109,    92,    93,   110,     0,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,     0,     0,   104,
     105,   106,   107,   108,   109,    93,     0,   110,     0,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,     0,
       0,     0,   104,   105,   106,   107,   108,   109,     0,     0,
     110,    98,    99,   100,   101,   102,   103,     0,     0,     0,
     104,   105,   106,   107,   108,   109,     0,     0,   110,  -100,
    -100,  -100,  -100,  -100,  -100,     0,     0,     0,   104,   105,
     106,   107,   108,   109,     0,     0,   110
};

static const yytype_int16 yycheck[] =
{
       0,     0,     5,    55,     5,    66,    67,   127,    37,     9,
      10,    11,    17,    61,     5,    20,    18,    19,    23,    24,
      25,    26,    22,    28,   223,    35,    36,     5,    64,    65,
      64,    65,   231,    62,    63,    71,    37,    71,    55,    56,
      64,    65,    59,    21,    22,    23,    24,    18,    19,    62,
      60,    22,    64,    65,    63,   254,    64,    65,    62,    62,
      65,     0,    63,    63,    60,    70,    53,    54,    55,    56,
     269,   270,    59,    62,    63,    64,    65,    62,    63,    55,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    60,   227,    59,   114,
     230,   113,    68,   113,   119,   120,    62,    63,    63,   119,
      63,   121,   174,   123,    14,   125,    63,   127,   128,   129,
       5,    16,   131,    62,    63,    10,    11,   139,    63,   139,
      66,   146,    69,   204,    66,   193,    21,    22,    23,    24,
      25,    64,    65,    68,    69,   269,   270,    69,    62,   211,
      68,   222,    62,    66,    70,    63,    63,    37,   229,    22,
     218,   219,   220,   221,    66,    66,   213,     0,     0,   184,
      14,    -1,    -1,    -1,    -1,   185,   186,   187,   188,    -1,
      -1,    -1,   192,    -1,    -1,   200,    -1,    -1,    -1,    -1,
      -1,    -1,   207,    -1,   209,   266,   267,    -1,   213,   214,
      -1,   263,   264,   213,    -1,    -1,    -1,    -1,    -1,    -1,
     225,    -1,    -1,   223,    -1,    -1,    -1,   227,    -1,    -1,
     230,   231,    -1,    -1,   233,   234,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   249,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,   254,    13,    -1,    15,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,   269,
     270,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    -1,    38,    -1,    40,    -1,    42,    43,    44,    45,
      46,    47,    -1,    51,    52,    51,    52,    53,    54,    55,
      56,    -1,    -1,    59,    62,    63,    -1,    -1,    -1,    67,
      -1,     1,    70,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    -1,    15,    -1,    17,    18,    19,
      20,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    48,    49,
      -1,    51,    52,    35,    36,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    -1,    67,    -1,    51,
      52,    35,    36,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    -1,    67,    68,    51,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      -1,    -1,    -1,    67,    68,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    13,    -1,    15,    -1,    17,
      18,    19,    20,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    52,    35,    36,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    62,    63,    -1,    -1,    -1,    67,
      -1,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    -1,    -1,    -1,    67,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    -1,    -1,    51,    52,    53,    54,    55,    56,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    -1,    -1,    51,    52,    53,    54,
      55,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    -1,    -1,    51,
      52,    53,    54,    55,    56,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      -1,    -1,    51,    52,    53,    54,    55,    56,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    -1,    -1,    51,    52,    53,    54,    55,
      56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    -1,    -1,    51,    52,
      53,    54,    55,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    -1,    -1,    51,
      52,    53,    54,    55,    56,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    -1,
      -1,    51,    52,    53,    54,    55,    56,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      -1,    -1,    51,    52,    53,    54,    55,    56,    -1,    -1,
      59,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    -1,    -1,    51,    52,    53,    54,    55,
      56,    -1,    -1,    59,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    -1,    -1,    51,    52,    53,
      54,    55,    56,    -1,    -1,    59,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    -1,    -1,    51,    52,
      53,    54,    55,    56,    -1,    -1,    59,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    -1,    -1,    51,    52,
      53,    54,    55,    56,    -1,    -1,    59,    31,    32,    33,
      34,    -1,    -1,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    -1,    -1,    51,    52,    53,
      54,    55,    56,    -1,    -1,    59,    32,    33,    34,    -1,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    -1,    -1,    51,    52,    53,    54,    55,
      56,    33,    34,    59,    -1,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    -1,    -1,    51,
      52,    53,    54,    55,    56,    34,    -1,    59,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      -1,    -1,    51,    52,    53,    54,    55,    56,    -1,    -1,
      59,    42,    43,    44,    45,    46,    47,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    -1,    -1,    59,    42,
      43,    44,    45,    46,    47,    -1,    -1,    -1,    51,    52,
      53,    54,    55,    56,    -1,    -1,    59
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    15,    17,    18,    19,    20,    35,    36,
      38,    48,    49,    51,    52,    62,    63,    64,    67,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    87,    88,    92,    93,   102,   103,   104,   107,   108,
     110,   112,   113,   114,   115,   116,   117,   119,    64,    65,
     116,   116,   116,    62,   111,    62,   109,   109,    74,   116,
      62,    84,    87,    84,    74,    84,    88,   116,    74,    74,
      74,    74,   118,    74,     0,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    51,    52,    53,    54,    55,    56,
      59,    35,    36,    60,    63,    64,    65,    64,    65,    62,
      63,    85,    86,    62,    63,    98,    99,    62,    98,    55,
     116,    70,   100,   103,    74,   100,   100,    85,    74,    60,
      64,    65,    64,    65,    66,    68,    69,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    87,   116,    74,    74,    89,    90,    91,
     116,    68,    74,   116,    63,    21,    22,    23,    24,    94,
      95,    96,    97,   116,    68,   116,    63,    94,   116,   116,
      14,   101,   103,    66,    16,    66,    87,    63,    74,    63,
      85,    68,    66,    69,    37,    68,    68,    74,   116,   116,
     116,   116,    66,    69,   116,    37,    98,    62,    68,    66,
      62,    70,    74,    64,    65,    71,   100,    74,    74,    85,
      91,    74,    68,    98,    98,    98,    98,   100,    96,    37,
      74,    94,   100,    94,    25,    92,    93,    96,   105,   106,
      66,   103,   103,    68,    68,    74,    66,    66,    96,    64,
      65,    71,    85,    85,   100,   100,   106,   106
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
      case 74: /* "expression" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1668 "./grammar.tab.c"
	break;
      case 75: /* "arrowAssign" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1673 "./grammar.tab.c"
	break;
      case 76: /* "tildeAssign" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1678 "./grammar.tab.c"
	break;
      case 77: /* "equationAssign" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1683 "./grammar.tab.c"
	break;
      case 78: /* "controlAssign" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1688 "./grammar.tab.c"
	break;
      case 79: /* "referenceAssign" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1693 "./grammar.tab.c"
	break;
      case 80: /* "additionAssign" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1698 "./grammar.tab.c"
	break;
      case 81: /* "subtractionAssign" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1703 "./grammar.tab.c"
	break;
      case 82: /* "multiplicationAssign" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1708 "./grammar.tab.c"
	break;
      case 83: /* "divisionAssign" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1713 "./grammar.tab.c"
	break;
      case 84: /* "variable" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1718 "./grammar.tab.c"
	break;
      case 85: /* "optElements" */
#line 133 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1723 "./grammar.tab.c"
	break;
      case 86: /* "elementList" */
#line 133 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1728 "./grammar.tab.c"
	break;
      case 87: /* "fxnCall" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); };
#line 1733 "./grammar.tab.c"
	break;
      case 88: /* "functionCall" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); };
#line 1738 "./grammar.tab.c"
	break;
      case 89: /* "optArguments" */
#line 134 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1743 "./grammar.tab.c"
	break;
      case 90: /* "argumentList" */
#line 134 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1748 "./grammar.tab.c"
	break;
      case 91: /* "argument" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxLabeledExpr)); };
#line 1753 "./grammar.tab.c"
	break;
      case 92: /* "functionDef" */
#line 142 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1758 "./grammar.tab.c"
	break;
      case 93: /* "procedureDef" */
#line 142 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1763 "./grammar.tab.c"
	break;
      case 94: /* "optFormals" */
#line 135 "./grammar.y"
	{ for (std::list<SyntaxFormal*>::iterator it=(yyvaluep->formalList)->begin(); it != (yyvaluep->formalList)->end(); it++) { SyntaxFormal* theElement = *it; delete theElement; }; delete ((yyvaluep->formalList)); };
#line 1768 "./grammar.tab.c"
	break;
      case 95: /* "formalList" */
#line 135 "./grammar.y"
	{ for (std::list<SyntaxFormal*>::iterator it=(yyvaluep->formalList)->begin(); it != (yyvaluep->formalList)->end(); it++) { SyntaxFormal* theElement = *it; delete theElement; }; delete ((yyvaluep->formalList)); };
#line 1773 "./grammar.tab.c"
	break;
      case 96: /* "formal" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxFormal)); };
#line 1778 "./grammar.tab.c"
	break;
      case 97: /* "typeSpec" */
#line 136 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1783 "./grammar.tab.c"
	break;
      case 98: /* "optDims" */
#line 136 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1788 "./grammar.tab.c"
	break;
      case 99: /* "dimList" */
#line 136 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1793 "./grammar.tab.c"
	break;
      case 100: /* "stmts" */
#line 133 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1798 "./grammar.tab.c"
	break;
      case 101: /* "stmtList" */
#line 133 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1803 "./grammar.tab.c"
	break;
      case 102: /* "statement" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1808 "./grammar.tab.c"
	break;
      case 103: /* "stmt_or_expr" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1813 "./grammar.tab.c"
	break;
      case 104: /* "declaration" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1818 "./grammar.tab.c"
	break;
      case 105: /* "memberDefs" */
#line 133 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); };
#line 1823 "./grammar.tab.c"
	break;
      case 106: /* "memberDef" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1828 "./grammar.tab.c"
	break;
      case 107: /* "classDef" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1833 "./grammar.tab.c"
	break;
      case 108: /* "ifStatement" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1838 "./grammar.tab.c"
	break;
      case 109: /* "cond" */
#line 144 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1843 "./grammar.tab.c"
	break;
      case 110: /* "forStatement" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1848 "./grammar.tab.c"
	break;
      case 111: /* "forCond" */
#line 144 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1853 "./grammar.tab.c"
	break;
      case 112: /* "whileStatement" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1858 "./grammar.tab.c"
	break;
      case 113: /* "nextStatement" */
#line 145 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1863 "./grammar.tab.c"
	break;
      case 114: /* "breakStatement" */
#line 145 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1868 "./grammar.tab.c"
	break;
      case 115: /* "returnStatement" */
#line 144 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1873 "./grammar.tab.c"
	break;
      case 116: /* "identifier" */
#line 136 "./grammar.y"
	{ delete ((yyvaluep->string)); };
#line 1878 "./grammar.tab.c"
	break;
      case 117: /* "vector" */
#line 134 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1883 "./grammar.tab.c"
	break;
      case 118: /* "vectorList" */
#line 134 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); };
#line 1888 "./grammar.tab.c"
	break;
      case 119: /* "constant" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); };
#line 1893 "./grammar.tab.c"
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
#line 232 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison encountered end_of_input; ignored\n");
#endif
                    return 0;
                }
    break;

  case 3:
#line 239 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison encountered newline; ignored\n");
#endif
                    return 0;
                }
    break;

  case 4:
#line 246 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute statement or expression\n");
#endif
                    int rv = parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    delete (yyvsp[(1) - (2)].syntaxElement);
                    return rv;
                }
    break;

  case 5:
#line 255 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute statement or expression\n");
#endif
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    delete (yyvsp[(1) - (2)].syntaxElement);
                    return rv;
                }
    break;

  case 6:
#line 264 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute declaration\n");
#endif
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    delete (yyvsp[(1) - (2)].syntaxElement);
                    return rv;
                }
    break;

  case 7:
#line 273 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to execute declaration\n");
#endif
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement), *executionEnvironment);
                    delete (yyvsp[(1) - (2)].syntaxElement);
                    return rv;
                }
    break;

  case 8:
#line 282 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to get help for symbol\n");
#endif
                    int rv =  parser.help(*(yyvsp[(2) - (3)].string));
                    delete (yyvsp[(2) - (3)].string);
                    return rv;
                }
    break;

  case 9:
#line 291 "./grammar.y"
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
#line 300 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to get help for function call\n");
#endif
                    int rv =  parser.help((yyvsp[(2) - (3)].syntaxFunctionCall));
                    delete (yyvsp[(2) - (3)].syntaxFunctionCall);
                    return rv;
                }
    break;

  case 11:
#line 309 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison trying to get help for function call\n");
#endif
                    int rv =  parser.help((yyvsp[(2) - (3)].syntaxFunctionCall));
                    delete (yyvsp[(2) - (3)].syntaxFunctionCall);
                    return rv;
                }
    break;

  case 12:
#line 318 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
#endif
                    YYABORT;
                }
    break;

  case 13:
#line 325 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
#endif
                    YYABORT;
                }
    break;

  case 14:
#line 333 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 15:
#line 335 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxFunctionCall("v", (yyvsp[(1) - (1)].argumentList)); }
    break;

  case 16:
#line 337 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 17:
#line 339 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UMinus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 18:
#line 340 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UPlus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 19:
#line 341 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UNot, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 20:
#line 342 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UAnd, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 21:
#line 344 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxDecrement( (yyvsp[(2) - (2)].syntaxElement) ); }
    break;

  case 22:
#line 345 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxDecrement( (yyvsp[(1) - (2)].syntaxElement) ); }
    break;

  case 23:
#line 346 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxIncrement( (yyvsp[(2) - (2)].syntaxElement) ); }
    break;

  case 24:
#line 347 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxIncrement( (yyvsp[(1) - (2)].syntaxElement) ); }
    break;

  case 25:
#line 349 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Range, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 26:
#line 351 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Add, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 27:
#line 352 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Sub, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 28:
#line 353 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Mul, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 29:
#line 354 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Div, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 30:
#line 355 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Exp, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 31:
#line 356 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Mod, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 32:
#line 358 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Lt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 33:
#line 359 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Le, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 34:
#line 360 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Eq, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 35:
#line 361 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ne, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 36:
#line 362 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ge, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 37:
#line 363 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Gt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 38:
#line 365 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 39:
#line 366 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 40:
#line 367 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 41:
#line 368 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 42:
#line 370 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 43:
#line 371 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 44:
#line 372 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 45:
#line 373 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 46:
#line 374 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 47:
#line 376 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 48:
#line 377 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 49:
#line 378 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 50:
#line 379 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 51:
#line 381 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFunctionCall); }
    break;

  case 52:
#line 383 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 53:
#line 387 "./grammar.y"
    { 
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxConstantAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 54:
#line 396 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxStochasticAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 55:
#line 405 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxDeterministicAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 56:
#line 414 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting control assignment (CONTROL_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxControlAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 57:
#line 423 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting reference assignment (REFERENCE_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxReferenceAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 58:
#line 432 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting addition assignment (ADDITION_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxAdditionAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 59:
#line 441 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                            printf("Parser inserting subtraction assignment (SUBTRACTION_ASSIGN) in syntax tree\n");
#endif
                            (yyval.syntaxElement) = new SyntaxSubtractionAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                        }
    break;

  case 60:
#line 450 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                                printf("Parser inserting multiplication assignment (MULTIPLICATION_ASSIGN) in syntax tree\n");
#endif
                                (yyval.syntaxElement) = new SyntaxMultiplicationAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                            }
    break;

  case 61:
#line 459 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting division assignment (DIVISION_ASSIGN) in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxDivisionAssignment((yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 62:
#line 468 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting variable (NAMED_VAR) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxVariable(*(yyvsp[(1) - (2)].string));
                    for (std::list<SyntaxElement*>::iterator it=(yyvsp[(2) - (2)].syntaxElementList)->begin(); it!=(yyvsp[(2) - (2)].syntaxElementList)->end(); ++it)
                    {
                        (yyval.syntaxElement) = new SyntaxIndexOperation((yyval.syntaxElement),*it);
                    }
                    delete (yyvsp[(1) - (2)].string);
                    delete (yyvsp[(2) - (2)].syntaxElementList);
                }
    break;

  case 63:
#line 481 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting variable (FUNCTION_VAR) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxIndexOperation((yyvsp[(1) - (5)].syntaxFunctionCall),(yyvsp[(3) - (5)].syntaxElement));
                    for (std::list<SyntaxElement*>::iterator it=(yyvsp[(5) - (5)].syntaxElementList)->begin(); it!=(yyvsp[(5) - (5)].syntaxElementList)->end(); ++it)
                    {
                        (yyval.syntaxElement) = new SyntaxIndexOperation((yyval.syntaxElement),*it);
                    }
                    delete (yyvsp[(5) - (5)].syntaxElementList);
                }
    break;

  case 64:
#line 493 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting variable (EXPRESSION_VAR) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxIndexOperation((yyvsp[(2) - (7)].syntaxElement),(yyvsp[(5) - (7)].syntaxElement));
                    for (std::list<SyntaxElement*>::iterator it=(yyvsp[(7) - (7)].syntaxElementList)->begin(); it!=(yyvsp[(7) - (7)].syntaxElementList)->end(); ++it)
                    {
                        (yyval.syntaxElement) = new SyntaxIndexOperation((yyval.syntaxElement),*it);
                    }
                    delete (yyvsp[(7) - (7)].syntaxElementList);
                }
    break;

  case 65:
#line 505 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting member variable (NAMED_VAR) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxVariable((yyvsp[(1) - (4)].syntaxElement),*(yyvsp[(3) - (4)].string));
                    for (std::list<SyntaxElement*>::iterator it=(yyvsp[(4) - (4)].syntaxElementList)->begin(); it!=(yyvsp[(4) - (4)].syntaxElementList)->end(); ++it)
                    {
                        (yyval.syntaxElement) = new SyntaxIndexOperation((yyval.syntaxElement),*it);
                    }
                    delete (yyvsp[(3) - (4)].string);
                    delete (yyvsp[(4) - (4)].syntaxElementList);
                }
    break;

  case 66:
#line 518 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting member variable (FUNCTION_VAR) in syntax tree\n");
#endif
                    (yyvsp[(3) - (7)].syntaxFunctionCall)->setBaseVariable((yyvsp[(1) - (7)].syntaxElement));
                    (yyval.syntaxElement) = new SyntaxIndexOperation((yyvsp[(3) - (7)].syntaxFunctionCall),(yyvsp[(5) - (7)].syntaxElement));
                    for (std::list<SyntaxElement*>::iterator it=(yyvsp[(7) - (7)].syntaxElementList)->begin(); it!=(yyvsp[(7) - (7)].syntaxElementList)->end(); ++it)
                    {
                        (yyval.syntaxElement) = new SyntaxIndexOperation((yyval.syntaxElement),*it);
                    }
                    delete (yyvsp[(7) - (7)].syntaxElementList);
                }
    break;

  case 67:
#line 532 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 68:
#line 533 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (1)].syntaxElementList); }
    break;

  case 69:
#line 536 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(2) - (3)].syntaxElement)); }
    break;

  case 70:
#line 537 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 71:
#line 538 "./grammar.y"
    { (yyvsp[(1) - (4)].syntaxElementList)->push_back((yyvsp[(3) - (4)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (4)].syntaxElementList); }
    break;

  case 72:
#line 539 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back( NULL ); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 73:
#line 543 "./grammar.y"
    {
                    (yyval.syntaxFunctionCall) = new SyntaxFunctionCall(*(yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].argumentList));
                    delete (yyvsp[(1) - (4)].string);
                }
    break;

  case 74:
#line 550 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting function call in syntax tree\n");
#endif
                        (yyval.syntaxFunctionCall) = (yyvsp[(1) - (1)].syntaxFunctionCall);
                    }
    break;

  case 75:
#line 557 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting member call in syntax tree\n");
#endif
                        (yyvsp[(3) - (3)].syntaxFunctionCall)->setBaseVariable((yyvsp[(1) - (3)].syntaxElement));
                        (yyval.syntaxFunctionCall) = (yyvsp[(3) - (3)].syntaxFunctionCall);
                    }
    break;

  case 76:
#line 566 "./grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(); }
    break;

  case 77:
#line 567 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(1) - (1)].argumentList); }
    break;

  case 78:
#line 570 "./grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(1,(yyvsp[(1) - (1)].syntaxLabeledExpr)); }
    break;

  case 79:
#line 571 "./grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back((yyvsp[(3) - (3)].syntaxLabeledExpr)); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 80:
#line 575 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting unlabeled argument in syntax tree\n");
#endif
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr( "", (yyvsp[(1) - (1)].syntaxElement));
                }
    break;

  case 81:
#line 582 "./grammar.y"
    { 
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting labeled argument in syntax tree\n");
#endif
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr(*(yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].syntaxElement));
                    delete (yyvsp[(1) - (3)].string);
                }
    break;

  case 82:
#line 592 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting function definition in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxFunctionDef("", *(yyvsp[(2) - (6)].string), (yyvsp[(4) - (6)].formalList), (yyvsp[(6) - (6)].syntaxElementList));
                    delete (yyvsp[(2) - (6)].string);
                }
    break;

  case 83:
#line 601 "./grammar.y"
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

  case 84:
#line 614 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting procedure definition in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxFunctionDef("", *(yyvsp[(2) - (6)].string), (yyvsp[(4) - (6)].formalList), (yyvsp[(6) - (6)].syntaxElementList), true);
                    delete (yyvsp[(2) - (6)].string);
                }
    break;

  case 85:
#line 623 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting typed procedure definition in syntax tree\n");
#endif
                    (yyvsp[(2) - (8)].string)->append(*(yyvsp[(3) - (8)].string));
                    (yyval.syntaxElement) = new SyntaxFunctionDef(*(yyvsp[(2) - (8)].string), *(yyvsp[(4) - (8)].string), (yyvsp[(6) - (8)].formalList), (yyvsp[(8) - (8)].syntaxElementList), true);
                    delete (yyvsp[(2) - (8)].string);
                    delete (yyvsp[(3) - (8)].string);
                    delete (yyvsp[(4) - (8)].string);
                }
    break;

  case 86:
#line 635 "./grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal*>(); }
    break;

  case 87:
#line 636 "./grammar.y"
    { (yyval.formalList) = (yyvsp[(1) - (1)].formalList); }
    break;

  case 88:
#line 639 "./grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal*>(1, (yyvsp[(1) - (1)].syntaxFormal)); }
    break;

  case 89:
#line 640 "./grammar.y"
    { (yyvsp[(1) - (3)].formalList)->push_back((yyvsp[(3) - (3)].syntaxFormal)); (yyval.formalList) = (yyvsp[(1) - (3)].formalList); }
    break;

  case 90:
#line 644 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting labeled formal argument without default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (1)].string), NULL );
                    delete (yyvsp[(1) - (1)].string);
                }
    break;

  case 91:
#line 652 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting labeled formal argument with default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].syntaxElement));
                    delete (yyvsp[(1) - (3)].string);
                }
    break;

  case 92:
#line 660 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting typed labeled formal argument without default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (2)].string), *(yyvsp[(2) - (2)].string), NULL);
                    delete (yyvsp[(1) - (2)].string);
                    delete (yyvsp[(2) - (2)].string);
                }
    break;

  case 93:
#line 669 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Inserting typed labeled formal argument with default in syntax tree\n");
#endif
                    (yyval.syntaxFormal) = new SyntaxFormal(*(yyvsp[(1) - (4)].string), *(yyvsp[(2) - (4)].string), (yyvsp[(4) - (4)].syntaxElement));
                    delete (yyvsp[(1) - (4)].string);
                    delete (yyvsp[(2) - (4)].string);
                }
    break;

  case 94:
#line 679 "./grammar.y"
    { (yyvsp[(1) - (2)].string)->append(*((yyvsp[(2) - (2)].string))); delete (yyvsp[(2) - (2)].string); (yyval.string) = (yyvsp[(1) - (2)].string); }
    break;

  case 95:
#line 680 "./grammar.y"
    { (yyvsp[(2) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); (yyvsp[(2) - (3)].string)->insert(0, "const ");           delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 96:
#line 681 "./grammar.y"
    { (yyvsp[(2) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); (yyvsp[(2) - (3)].string)->insert(0, "dynamic ");         delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 97:
#line 682 "./grammar.y"
    { (yyvsp[(2) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); (yyvsp[(2) - (3)].string)->insert(0, "stochastic ");      delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 98:
#line 683 "./grammar.y"
    { (yyvsp[(2) - (3)].string)->append(*((yyvsp[(3) - (3)].string))); (yyvsp[(2) - (3)].string)->insert(0, "deterministic ");   delete (yyvsp[(3) - (3)].string); (yyval.string) = (yyvsp[(2) - (3)].string); }
    break;

  case 99:
#line 686 "./grammar.y"
    { (yyval.string) = new std::string(""); }
    break;

  case 100:
#line 687 "./grammar.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); }
    break;

  case 101:
#line 690 "./grammar.y"
    { (yyval.string) = new std::string("[]"); }
    break;

  case 102:
#line 691 "./grammar.y"
    { (yyvsp[(1) - (3)].string)->append("[]"); (yyval.string) = (yyvsp[(1) - (3)].string); }
    break;

  case 103:
#line 694 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 104:
#line 696 "./grammar.y"
    {
                    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();
                    stmts->push_back((yyvsp[(1) - (1)].syntaxElement));
                    (yyval.syntaxElementList) = stmts;
                }
    break;

  case 105:
#line 703 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 106:
#line 704 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 107:
#line 705 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 108:
#line 706 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 109:
#line 707 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 110:
#line 708 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 111:
#line 711 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 112:
#line 712 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 113:
#line 713 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 114:
#line 714 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 115:
#line 715 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 116:
#line 716 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 117:
#line 719 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 118:
#line 720 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 119:
#line 723 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 120:
#line 724 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 121:
#line 725 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 122:
#line 727 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting variable declaration in syntax tree\n");
#endif
                        (yyval.syntaxElement) = new SyntaxVariableDecl(*(yyvsp[(1) - (3)].string), (yyvsp[(2) - (3)].syntaxElementList), *(yyvsp[(3) - (3)].string));
                        delete (yyvsp[(1) - (3)].string);
                        delete (yyvsp[(3) - (3)].string);
                    }
    break;

  case 123:
#line 737 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 124:
#line 738 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 125:
#line 739 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 126:
#line 740 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 127:
#line 741 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 128:
#line 742 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 129:
#line 745 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFormal); }
    break;

  case 130:
#line 746 "./grammar.y"
    { (yyvsp[(2) - (2)].syntaxFormal)->setIsProtected(); (yyval.syntaxElement) = (yyvsp[(2) - (2)].syntaxFormal); }
    break;

  case 131:
#line 747 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 132:
#line 748 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 133:
#line 752 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                printf("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxClassDef(*(yyvsp[(2) - (7)].string), *(yyvsp[(4) - (7)].string), (yyvsp[(6) - (7)].syntaxElementList));
                    delete (yyvsp[(2) - (7)].string);
                    delete (yyvsp[(4) - (7)].string);
                }
    break;

  case 134:
#line 762 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::If, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 135:
#line 763 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::IfElse, (yyvsp[(2) - (5)].syntaxElement), (yyvsp[(3) - (5)].syntaxElementList), (yyvsp[(5) - (5)].syntaxElementList)); }
    break;

  case 136:
#line 765 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 137:
#line 768 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::For, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 138:
#line 771 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxForLoop(*(yyvsp[(2) - (5)].string), (yyvsp[(4) - (5)].syntaxElement)); delete (yyvsp[(2) - (5)].string); }
    break;

  case 139:
#line 774 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::While, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 140:
#line 777 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Next); }
    break;

  case 141:
#line 780 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Break); }
    break;

  case 142:
#line 783 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return); }
    break;

  case 143:
#line 784 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 144:
#line 787 "./grammar.y"
    { (yyval.string) = new std::string((yyvsp[(1) - (1)].c_string)); }
    break;

  case 145:
#line 791 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(2) - (3)].argumentList); }
    break;

  case 146:
#line 794 "./grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back(new SyntaxLabeledExpr( "", (yyvsp[(3) - (3)].syntaxElement)) ); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 147:
#line 796 "./grammar.y"
    {
                (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(1, new SyntaxLabeledExpr("", (yyvsp[(1) - (1)].syntaxElement)) );
                }
    break;

  case 148:
#line 802 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting bool constant (false) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant(new RlBoolean(false) );
                }
    break;

  case 149:
#line 809 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting bool constant (true) in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant(new RlBoolean(true) );
                }
    break;

  case 150:
#line 816 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting null constant in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant( NULL );
                }
    break;

  case 151:
#line 823 "./grammar.y"
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

  case 152:
#line 838 "./grammar.y"
    {
#ifdef DEBUG_BISON_FLEX
                    printf("Parser inserting String constant in syntax tree\n");
#endif
                    (yyval.syntaxElement) = new SyntaxConstant(new RlString((yyvsp[(1) - (1)].c_string)) );
                }
    break;

  case 153:
#line 845 "./grammar.y"
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
                    
                    if ((yyvsp[(1) - (1)].realValue) >= 0.0 && (yyvsp[(1) - (1)].realValue) <= 1.0) {
                        #ifdef DEBUG_BISON_FLEX
                        printf("Parser inserting RealPos constant in syntax tree \n");
                        #endif
                        (yyval.syntaxElement) = new SyntaxConstant(new Probability((yyvsp[(1) - (1)].realValue)) );
                    }
                    else if ((yyvsp[(1) - (1)].realValue) >= 0.0) {
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
#line 3342 "./grammar.tab.c"
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


#line 905 "./grammar.y"



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



