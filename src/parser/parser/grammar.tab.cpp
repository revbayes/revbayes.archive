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
     TILDEIID_ASSIGN = 277,
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
#define TILDEIID_ASSIGN 277
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

#include "RbBoolean.h"
#include "Integer.h"
#include "Natural.h"
#include "Parser.h"
#include "RbString.h"
#include "Real.h"
#include "RealPos.h"
#include "SyntaxElement.h"
#include "SyntaxAssignExpr.h"
#include "SyntaxBinaryExpr.h"
#include "SyntaxClassDef.h"
#include "SyntaxConstant.h"
#include "SyntaxVariableDecl.h"
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
    char*                                           string;
    double                                          realValue;
    int                                             intValue;
    bool                                            boolValue;
    RbString*                                       idString;
    SyntaxElement*                                  syntaxElement;
    SyntaxVariable*                                 syntaxVariable;
    SyntaxFunctionCall*                             syntaxFunctionCall;
    SyntaxLabeledExpr*                              syntaxLabeledExpr;
    SyntaxFormal*                                   syntaxFormal;
    std::list<SyntaxElement*>*                      syntaxElementList;
    std::list<SyntaxLabeledExpr*>*                  argumentList;
    std::list<SyntaxFormal*>*                       formalList;
}
/* Line 193 of yacc.c.  */
#line 268 "./grammar.tab.c"
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
#line 293 "./grammar.tab.c"

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
#define YYFINAL  71
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   611

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  136
/* YYNRULES -- Number of states.  */
#define YYNSTATES  232

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
       0,     0,     3,     5,     7,    10,    13,    16,    19,    23,
      27,    31,    35,    38,    41,    43,    45,    49,    52,    55,
      58,    61,    65,    69,    73,    77,    81,    85,    89,    93,
      97,   101,   105,   109,   113,   117,   121,   125,   127,   129,
     131,   133,   135,   137,   141,   145,   149,   153,   157,   161,
     165,   169,   172,   178,   183,   191,   192,   194,   198,   201,
     206,   210,   215,   217,   221,   222,   224,   226,   230,   232,
     236,   243,   251,   252,   254,   256,   260,   262,   266,   269,
     274,   278,   279,   281,   284,   288,   292,   297,   298,   300,
     304,   306,   307,   309,   313,   316,   320,   323,   325,   327,
     329,   331,   333,   335,   337,   339,   341,   343,   348,   349,
     351,   355,   358,   362,   365,   367,   369,   377,   381,   387,
     391,   395,   401,   405,   407,   409,   411,   414,   416,   420,
     424,   426,   428,   430,   432,   434,   436
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      60,     0,    -1,    35,    -1,    50,    -1,    85,    50,    -1,
      85,    51,    -1,    86,    50,    -1,    86,    51,    -1,    36,
      98,    50,    -1,    36,    98,    51,    -1,    36,    70,    50,
      -1,    36,    70,    51,    -1,     1,    50,    -1,     1,    51,
      -1,   101,    -1,    99,    -1,    48,    61,    52,    -1,    39,
      61,    -1,    38,    61,    -1,    53,    61,    -1,    25,    61,
      -1,    61,    42,    61,    -1,    61,    38,    61,    -1,    61,
      39,    61,    -1,    61,    40,    61,    -1,    61,    41,    61,
      -1,    61,    45,    61,    -1,    61,    31,    61,    -1,    61,
      32,    61,    -1,    61,    33,    61,    -1,    61,    34,    61,
      -1,    61,    30,    61,    -1,    61,    29,    61,    -1,    61,
      25,    61,    -1,    61,    26,    61,    -1,    61,    27,    61,
      -1,    61,    28,    61,    -1,    62,    -1,    65,    -1,    63,
      -1,    64,    -1,    70,    -1,    66,    -1,    66,    20,    61,
      -1,    70,    20,    61,    -1,    66,    21,    70,    -1,    70,
      21,    70,    -1,    66,    22,    70,    -1,    70,    22,    70,
      -1,    66,    23,    61,    -1,    70,    23,    61,    -1,    98,
      67,    -1,    69,    49,    61,    54,    67,    -1,    66,    46,
      98,    67,    -1,    66,    46,    69,    49,    61,    54,    67,
      -1,    -1,    68,    -1,    49,    61,    54,    -1,    49,    54,
      -1,    68,    49,    61,    54,    -1,    68,    49,    54,    -1,
      98,    48,    71,    52,    -1,    69,    -1,    66,    46,    69,
      -1,    -1,    72,    -1,    73,    -1,    72,    55,    73,    -1,
      61,    -1,    98,    24,    61,    -1,    10,    98,    48,    75,
      52,    82,    -1,    10,    78,    98,    48,    75,    52,    82,
      -1,    -1,    76,    -1,    77,    -1,    76,    55,    77,    -1,
      98,    -1,    98,    24,    61,    -1,    78,    98,    -1,    78,
      98,    24,    61,    -1,    98,    79,    81,    -1,    -1,    80,
      -1,    49,    54,    -1,    49,     4,    54,    -1,    80,    49,
      54,    -1,    80,    49,     4,    54,    -1,    -1,    56,    -1,
      57,    83,    58,    -1,    85,    -1,    -1,    85,    -1,    83,
      51,    85,    -1,    83,    51,    -1,    83,    50,    85,    -1,
      83,    50,    -1,    90,    -1,    92,    -1,    94,    -1,    95,
      -1,    96,    -1,    97,    -1,    84,    -1,    61,    -1,    89,
      -1,    74,    -1,    98,    67,    81,    98,    -1,    -1,    88,
      -1,    87,    51,    88,    -1,    87,    51,    -1,    87,    50,
      88,    -1,    87,    50,    -1,    77,    -1,    74,    -1,    11,
      98,    42,    98,    57,    87,    58,    -1,    14,    91,    82,
      -1,    14,    91,    82,    15,    82,    -1,    48,    61,    52,
      -1,    12,    93,    82,    -1,    48,    98,    13,    61,    52,
      -1,    16,    91,    82,    -1,    17,    -1,    18,    -1,    19,
      -1,    19,    61,    -1,     5,    -1,    49,   100,    54,    -1,
     100,    55,    61,    -1,    61,    -1,     8,    -1,     9,    -1,
       7,    -1,     4,    -1,     6,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   207,   207,   212,   217,   223,   229,   235,   241,   248,
     255,   262,   269,   274,   281,   283,   285,   287,   288,   289,
     290,   292,   294,   295,   296,   297,   298,   300,   301,   302,
     303,   304,   305,   307,   308,   309,   310,   312,   313,   314,
     315,   317,   319,   322,   327,   334,   339,   346,   351,   358,
     363,   370,   375,   381,   386,   394,   395,   398,   399,   400,
     401,   404,   410,   415,   423,   424,   427,   428,   431,   436,
     443,   449,   456,   457,   460,   461,   464,   469,   474,   479,
     486,   489,   490,   493,   494,   495,   496,   499,   500,   503,
     504,   512,   513,   514,   515,   516,   517,   520,   521,   522,
     523,   524,   525,   528,   529,   532,   533,   534,   541,   542,
     543,   544,   545,   546,   549,   550,   553,   560,   561,   563,
     566,   569,   572,   575,   578,   581,   582,   585,   589,   592,
     593,   599,   604,   609,   614,   625,   630
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
  "TILDE_ASSIGN", "TILDEIID_ASSIGN", "EQUATION_ASSIGN", "EQUAL", "AND",
  "OR", "AND2", "OR2", "GT", "GE", "LT", "LE", "EQ", "NE", "END_OF_INPUT",
  "'?'", "UNOT", "'+'", "'-'", "'*'", "'/'", "':'", "UPLUS", "UMINUS",
  "'^'", "'.'", "UAND", "'('", "'['", "'\\n'", "';'", "')'", "'!'", "']'",
  "','", "'&'", "'{'", "'}'", "$accept", "prog", "expression",
  "arrowAssign", "tildeAssign", "tildeIidAssign", "equationAssign",
  "variable", "optElements", "elementList", "fxnCall", "functionCall",
  "optArguments", "argumentList", "argument", "functionDef", "optFormals",
  "formalList", "formal", "typeSpec", "optDims", "dimList", "optRef",
  "stmts", "stmtList", "statement", "stmt_or_expr", "declaration",
  "memberDefs", "memberDef", "classDef", "ifStatement", "cond",
  "forStatement", "forCond", "whileStatement", "nextStatement",
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
      61,    61,    61,    62,    62,    63,    63,    64,    64,    65,
      65,    66,    66,    66,    66,    67,    67,    68,    68,    68,
      68,    69,    70,    70,    71,    71,    72,    72,    73,    73,
      74,    74,    75,    75,    76,    76,    77,    77,    77,    77,
      78,    79,    79,    80,    80,    80,    80,    81,    81,    82,
      82,    83,    83,    83,    83,    83,    83,    84,    84,    84,
      84,    84,    84,    85,    85,    86,    86,    86,    87,    87,
      87,    87,    87,    87,    88,    88,    89,    90,    90,    91,
      92,    93,    94,    95,    96,    97,    97,    98,    99,   100,
     100,   101,   101,   101,   101,   101,   101
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     2,     2,     1,     1,     3,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     5,     4,     7,     0,     1,     3,     2,     4,
       3,     4,     1,     3,     0,     1,     1,     3,     1,     3,
       6,     7,     0,     1,     1,     3,     1,     3,     2,     4,
       3,     0,     1,     2,     3,     3,     4,     0,     1,     3,
       1,     0,     1,     3,     2,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     0,     1,
       3,     2,     3,     2,     1,     1,     7,     3,     5,     3,
       3,     5,     3,     1,     1,     1,     2,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   136,   134,   127,   135,   133,   131,   132,     0,
       0,     0,     0,     0,   123,   124,   125,     0,     2,     0,
       0,     0,     0,     0,     3,     0,     0,   104,    37,    39,
      40,    38,    42,    62,    41,   106,   103,     0,     0,   105,
      97,    98,    99,   100,   101,   102,    55,    15,    14,    12,
      13,     0,    81,     0,     0,     0,     0,     0,     0,   126,
      55,    20,     0,     0,    55,    18,    17,     0,   130,     0,
      19,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,    64,     0,    51,    56,     0,    72,     0,    87,
      82,     0,     0,    91,   120,    90,     0,   117,   122,    51,
      10,    11,     8,     9,    16,   128,     0,    33,    34,    35,
      36,    32,    31,    27,    28,    29,    30,    22,    23,    24,
      25,    21,    26,    43,    45,    47,    49,    63,    55,     0,
      44,    46,    48,    50,    68,     0,    65,    66,    55,    58,
       0,    88,     0,     0,    72,     0,    73,    74,     0,    76,
       0,    83,    80,     0,     0,     0,     0,    92,   119,     0,
     129,     0,    53,    55,    61,     0,     0,    57,   107,    60,
       0,     0,     0,     0,    78,     0,    84,     0,    85,   108,
       0,    96,    94,    89,   118,     0,    52,    67,    69,    59,
       0,    70,    75,     0,    77,    86,   115,   114,     0,   109,
     121,    95,    93,    55,    71,    79,   113,   111,   116,    54,
     112,   110
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,    27,    28,    29,    30,    31,    32,   119,   105,
      33,    34,   155,   156,   157,   216,   165,   166,   217,   168,
     109,   110,   162,   114,   176,    36,   115,    38,   218,   219,
      39,    40,    57,    41,    55,    42,    43,    44,    45,    60,
      47,    69,    48
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -103
static const yytype_int16 yypact[] =
{
     258,   -19,  -103,  -103,  -103,  -103,  -103,  -103,  -103,    35,
      35,    -2,    13,    13,  -103,  -103,   372,   372,  -103,    35,
     372,   372,   372,   372,  -103,   372,    48,   297,  -103,  -103,
    -103,  -103,     3,    46,    37,  -103,  -103,   -16,    39,  -103,
    -103,  -103,  -103,  -103,  -103,  -103,    -5,  -103,  -103,  -103,
    -103,    35,     4,     8,    35,   203,   372,   203,   203,   297,
      -5,  -103,    40,    62,    92,    58,    58,   205,   297,    78,
      89,  -103,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   372,    35,
      35,   372,    35,   372,   372,    35,    35,   372,  -103,  -103,
    -103,  -103,   372,   142,    25,    59,    76,    35,     2,    70,
      88,    35,   131,   347,  -103,  -103,   521,   137,  -103,  -103,
    -103,  -103,  -103,  -103,  -103,  -103,   372,    89,   374,    89,
     374,   566,   566,   566,   566,   566,   566,    56,    56,   -27,
     -27,    58,    58,   297,  -103,  -103,   297,   104,    -5,   401,
     297,  -103,  -103,   297,   297,   103,   102,  -103,    -7,  -103,
     431,  -103,    35,   295,    35,   106,   105,  -103,    35,    31,
     107,  -103,  -103,    29,   108,   372,   -30,  -103,  -103,   203,
     297,   372,  -103,   110,  -103,   372,   372,  -103,  -103,  -103,
     461,   111,   203,    35,   145,   372,  -103,   116,  -103,    17,
     549,   347,   347,  -103,  -103,   491,  -103,  -103,   297,  -103,
     203,  -103,  -103,   372,   297,  -103,  -103,  -103,   -13,  -103,
    -103,  -103,  -103,   110,  -103,   297,    17,    17,  -103,  -103,
    -103,  -103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -103,  -103,    -9,  -103,  -103,  -103,  -103,    10,   -44,  -103,
      79,    20,  -103,  -103,   -12,   174,    11,  -103,  -102,   169,
    -103,  -103,    73,   -54,  -103,  -103,     1,  -103,  -103,   -91,
    -103,  -103,   170,  -103,  -103,  -103,  -103,  -103,  -103,     0,
    -103,  -103,  -103
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -88
static const yytype_int16 yytable[] =
{
      46,    37,   104,   117,   118,   167,   170,    59,    61,    52,
      53,    65,    66,    67,    68,    86,    70,   186,    87,    64,
     201,   202,     4,    88,    89,    90,    91,     9,   203,    62,
     -87,    49,    50,   197,    98,    99,   -81,   226,   227,    63,
       4,   102,   103,   102,   103,   228,    54,   116,    71,    92,
     111,   106,   107,   108,   112,   195,   171,    94,    95,    96,
      97,    56,   167,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     108,   161,   146,   198,   149,   150,    92,   -81,   153,   100,
     101,   212,   148,   154,   160,    93,    84,    85,    86,    62,
      62,    87,   158,    87,   182,    62,    62,   169,   163,   144,
     145,   174,   120,   121,   177,   151,   152,   180,    76,    77,
      78,    79,    80,    81,   164,   204,   161,    82,    83,    84,
      85,    86,   125,   126,    87,   230,   231,   173,   211,   206,
     102,   103,   122,   123,   175,     2,     3,     4,     5,     6,
       7,     8,   179,   181,   190,   184,   224,   185,   192,   103,
     193,   196,   188,   210,   169,   199,   200,    17,   194,   213,
     215,   147,   205,   207,    35,   191,   154,   208,    51,   229,
      20,    21,   172,    58,     0,   158,   214,     0,     0,     0,
      22,    23,     0,   169,     0,    25,   159,     0,     0,   169,
       0,     0,   221,   222,   225,     0,     2,     3,     4,     5,
       6,     7,     8,     0,     0,    11,     0,    12,     0,    13,
      14,    15,    16,     0,     0,     0,   169,   169,    17,     0,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
       0,    20,    21,    82,    83,    84,    85,    86,     0,     0,
      87,    22,    23,     0,     0,     0,    25,   124,     0,     1,
     113,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,     0,    12,     0,    13,    14,    15,    16,     0,     0,
       0,     0,     0,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,    19,     0,    20,    21,     2,     3,
       4,     5,     6,     7,     8,     0,    22,    23,    24,     0,
       0,    25,     0,     0,     0,     0,     0,     0,     0,     0,
      17,     0,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,     0,    20,    21,    82,    83,    84,    85,    86,
       0,     0,    87,    22,    23,     0,     0,     0,    25,   189,
       2,     3,     4,     5,     6,     7,     8,     0,     0,    11,
       0,    12,     0,    13,    14,    15,    16,     0,     0,     0,
       0,     0,    17,     0,     0,     2,     3,     4,     5,     6,
       7,     8,     0,     0,     0,    20,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    23,    17,     0,    72,
      25,    74,     0,    76,    77,    78,    79,    80,    81,     0,
      20,    21,    82,    83,    84,    85,    86,     0,     0,    87,
      22,    23,     0,     0,     0,    25,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,    82,
      83,    84,    85,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,   183,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,    82,
      83,    84,    85,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,   187,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,    82,
      83,    84,    85,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,   209,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,    82,
      83,    84,    85,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,   223,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,    82,
      83,    84,    85,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,   178,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,    82,    83,    84,
      85,    86,     0,     0,    87,   -88,   -88,   -88,   -88,   -88,
     -88,   220,     0,     0,    82,    83,    84,    85,    86,     0,
       0,    87
};

static const yytype_int16 yycheck[] =
{
       0,     0,    46,    57,    58,   107,     4,    16,    17,     9,
      10,    20,    21,    22,    23,    42,    25,    24,    45,    19,
      50,    51,     5,    20,    21,    22,    23,    10,    58,    19,
       5,    50,    51,     4,    50,    51,     5,    50,    51,    19,
       5,    48,    49,    48,    49,    58,    48,    56,     0,    46,
      42,    51,    48,    49,    54,    24,    54,    20,    21,    22,
      23,    48,   164,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      49,    56,    91,    54,    93,    94,    46,    56,    97,    50,
      51,   193,    92,   102,   103,    49,    40,    41,    42,    89,
      90,    45,   102,    45,   148,    95,    96,   107,    49,    89,
      90,   111,    50,    51,   113,    95,    96,   126,    29,    30,
      31,    32,    33,    34,    48,   179,    56,    38,    39,    40,
      41,    42,    54,    55,    45,   226,   227,    49,   192,   183,
      48,    49,    50,    51,    13,     3,     4,     5,     6,     7,
       8,     9,    15,    49,   163,    52,   210,    55,    52,    49,
      55,    54,   162,    52,   164,    57,   175,    25,   168,    24,
      54,    92,   181,   185,     0,   164,   185,   186,     9,   223,
      38,    39,   109,    13,    -1,   185,   195,    -1,    -1,    -1,
      48,    49,    -1,   193,    -1,    53,    54,    -1,    -1,   199,
      -1,    -1,   201,   202,   213,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    12,    -1,    14,    -1,    16,
      17,    18,    19,    -1,    -1,    -1,   226,   227,    25,    -1,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    38,    39,    38,    39,    40,    41,    42,    -1,    -1,
      45,    48,    49,    -1,    -1,    -1,    53,    52,    -1,     1,
      57,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,    14,    -1,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,     3,     4,
       5,     6,     7,     8,     9,    -1,    48,    49,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    38,    39,    38,    39,    40,    41,    42,
      -1,    -1,    45,    48,    49,    -1,    -1,    -1,    53,    54,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    12,
      -1,    14,    -1,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    25,    -1,    25,
      53,    27,    -1,    29,    30,    31,    32,    33,    34,    -1,
      38,    39,    38,    39,    40,    41,    42,    -1,    -1,    45,
      48,    49,    -1,    -1,    -1,    53,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    38,    39,    40,
      41,    42,    -1,    -1,    45,    29,    30,    31,    32,    33,
      34,    52,    -1,    -1,    38,    39,    40,    41,    42,    -1,
      -1,    45
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    14,    16,    17,    18,    19,    25,    35,    36,
      38,    39,    48,    49,    50,    53,    60,    61,    62,    63,
      64,    65,    66,    69,    70,    74,    84,    85,    86,    89,
      90,    92,    94,    95,    96,    97,    98,    99,   101,    50,
      51,    78,    98,    98,    48,    93,    48,    91,    91,    61,
      98,    61,    66,    70,    98,    61,    61,    61,    61,   100,
      61,     0,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    38,    39,    40,    41,    42,    45,    20,    21,
      22,    23,    46,    49,    20,    21,    22,    23,    50,    51,
      50,    51,    48,    49,    67,    68,    98,    48,    49,    79,
      80,    42,    98,    57,    82,    85,    61,    82,    82,    67,
      50,    51,    50,    51,    52,    54,    55,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    70,    70,    61,    69,    98,    61,
      61,    70,    70,    61,    61,    71,    72,    73,    98,    54,
      61,    56,    81,    49,    48,    75,    76,    77,    78,    98,
       4,    54,    81,    49,    98,    13,    83,    85,    52,    15,
      61,    49,    67,    54,    52,    55,    24,    54,    98,    54,
      61,    75,    52,    55,    98,    24,    54,     4,    54,    57,
      61,    50,    51,    58,    82,    61,    67,    73,    61,    54,
      52,    82,    77,    24,    61,    54,    74,    77,    87,    88,
      52,    85,    85,    54,    82,    61,    50,    51,    58,    67,
      88,    88
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
#line 133 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting expression  ...\n"); };
#line 1495 "./grammar.tab.c"
	break;
      case 62: /* "arrowAssign" */
#line 134 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1500 "./grammar.tab.c"
	break;
      case 63: /* "tildeAssign" */
#line 134 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1505 "./grammar.tab.c"
	break;
      case 64: /* "tildeIidAssign" */
#line 134 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1510 "./grammar.tab.c"
	break;
      case 65: /* "equationAssign" */
#line 134 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1515 "./grammar.tab.c"
	break;
      case 66: /* "variable" */
#line 132 "./grammar.y"
	{ delete ((yyvaluep->syntaxVariable)); PRINTF("Deleting variable    ...\n"); };
#line 1520 "./grammar.tab.c"
	break;
      case 67: /* "optElements" */
#line 128 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1525 "./grammar.tab.c"
	break;
      case 68: /* "elementList" */
#line 128 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1530 "./grammar.tab.c"
	break;
      case 69: /* "fxnCall" */
#line 132 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); PRINTF("Deleting variable    ...\n"); };
#line 1535 "./grammar.tab.c"
	break;
      case 70: /* "functionCall" */
#line 132 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); PRINTF("Deleting variable    ...\n"); };
#line 1540 "./grammar.tab.c"
	break;
      case 71: /* "optArguments" */
#line 129 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1545 "./grammar.tab.c"
	break;
      case 72: /* "argumentList" */
#line 129 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1550 "./grammar.tab.c"
	break;
      case 73: /* "argument" */
#line 132 "./grammar.y"
	{ delete ((yyvaluep->syntaxLabeledExpr)); PRINTF("Deleting variable    ...\n"); };
#line 1555 "./grammar.tab.c"
	break;
      case 74: /* "functionDef" */
#line 136 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting functiondef ...\n"); };
#line 1560 "./grammar.tab.c"
	break;
      case 75: /* "optFormals" */
#line 130 "./grammar.y"
	{ for (std::list<SyntaxFormal*>::iterator it=(yyvaluep->formalList)->begin(); it != (yyvaluep->formalList)->end(); it++) { SyntaxFormal* theElement = *it; delete theElement; }; delete ((yyvaluep->formalList)); PRINTF("Deleting formal list\n"); };
#line 1565 "./grammar.tab.c"
	break;
      case 76: /* "formalList" */
#line 130 "./grammar.y"
	{ for (std::list<SyntaxFormal*>::iterator it=(yyvaluep->formalList)->begin(); it != (yyvaluep->formalList)->end(); it++) { SyntaxFormal* theElement = *it; delete theElement; }; delete ((yyvaluep->formalList)); PRINTF("Deleting formal list\n"); };
#line 1570 "./grammar.tab.c"
	break;
      case 77: /* "formal" */
#line 132 "./grammar.y"
	{ delete ((yyvaluep->syntaxFormal)); PRINTF("Deleting variable    ...\n"); };
#line 1575 "./grammar.tab.c"
	break;
      case 78: /* "typeSpec" */
#line 131 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1580 "./grammar.tab.c"
	break;
      case 79: /* "optDims" */
#line 131 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1585 "./grammar.tab.c"
	break;
      case 80: /* "dimList" */
#line 131 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1590 "./grammar.tab.c"
	break;
      case 81: /* "optRef" */
#line 131 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1595 "./grammar.tab.c"
	break;
      case 82: /* "stmts" */
#line 128 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1600 "./grammar.tab.c"
	break;
      case 83: /* "stmtList" */
#line 128 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1605 "./grammar.tab.c"
	break;
      case 84: /* "statement" */
#line 133 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting expression  ...\n"); };
#line 1610 "./grammar.tab.c"
	break;
      case 85: /* "stmt_or_expr" */
#line 133 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting expression  ...\n"); };
#line 1615 "./grammar.tab.c"
	break;
      case 86: /* "declaration" */
#line 135 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting declaration ...\n"); };
#line 1620 "./grammar.tab.c"
	break;
      case 87: /* "memberDefs" */
#line 128 "./grammar.y"
	{ for (std::list<SyntaxElement*>::iterator it=(yyvaluep->syntaxElementList)->begin(); it != (yyvaluep->syntaxElementList)->end(); it++) { SyntaxElement* theElement = *it; delete theElement; }; delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1625 "./grammar.tab.c"
	break;
      case 88: /* "memberDef" */
#line 135 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting declaration ...\n"); };
#line 1630 "./grammar.tab.c"
	break;
      case 89: /* "classDef" */
#line 135 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting declaration ...\n"); };
#line 1635 "./grammar.tab.c"
	break;
      case 90: /* "ifStatement" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting for/if/while...\n"); };
#line 1640 "./grammar.tab.c"
	break;
      case 91: /* "cond" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting cond/return ...\n"); };
#line 1645 "./grammar.tab.c"
	break;
      case 92: /* "forStatement" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting for/if/while...\n"); };
#line 1650 "./grammar.tab.c"
	break;
      case 93: /* "forCond" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting cond/return ...\n"); };
#line 1655 "./grammar.tab.c"
	break;
      case 94: /* "whileStatement" */
#line 137 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting for/if/while...\n"); };
#line 1660 "./grammar.tab.c"
	break;
      case 95: /* "nextStatement" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting next/break  ...\n"); };
#line 1665 "./grammar.tab.c"
	break;
      case 96: /* "breakStatement" */
#line 139 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting next/break  ...\n"); };
#line 1670 "./grammar.tab.c"
	break;
      case 97: /* "returnStatement" */
#line 138 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting cond/return ...\n"); };
#line 1675 "./grammar.tab.c"
	break;
      case 98: /* "identifier" */
#line 131 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1680 "./grammar.tab.c"
	break;
      case 99: /* "vector" */
#line 129 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1685 "./grammar.tab.c"
	break;
      case 100: /* "vectorList" */
#line 129 "./grammar.y"
	{ for (std::list<SyntaxLabeledExpr*>::iterator it=(yyvaluep->argumentList)->begin(); it != (yyvaluep->argumentList)->end(); it++) { SyntaxLabeledExpr* theElement = *it; delete theElement; }; delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1690 "./grammar.tab.c"
	break;
      case 101: /* "constant" */
#line 132 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting variable    ...\n"); };
#line 1695 "./grammar.tab.c"
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
#line 208 "./grammar.y"
    {
                    PRINTF("Bison encountered end_of_input; ignored\n");
                    return 0;
                }
    break;

  case 3:
#line 213 "./grammar.y"
    {
                    PRINTF("Bison encountered newline; ignored\n");
                    return 0;
                }
    break;

  case 4:
#line 218 "./grammar.y"
    {
                    PRINTF("Bison trying to execute statement or expression\n");
                    int rv = parser.execute((yyvsp[(1) - (2)].syntaxElement));
                    return rv;
                }
    break;

  case 5:
#line 224 "./grammar.y"
    {
                    PRINTF("Bison trying to execute statement or expression\n");
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement));
                    return rv;
                }
    break;

  case 6:
#line 230 "./grammar.y"
    {
                    PRINTF("Bison trying to execute declaration\n");
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement));
                    return rv;
                }
    break;

  case 7:
#line 236 "./grammar.y"
    {
                    PRINTF("Bison trying to execute declaration\n");
                    int rv =  parser.execute((yyvsp[(1) - (2)].syntaxElement));
                    return rv;
                }
    break;

  case 8:
#line 242 "./grammar.y"
    {
                    PRINTF("Bison trying to get help for symbol\n");
                    int rv =  parser.help(*((yyvsp[(2) - (3)].idString)));
                    delete (yyvsp[(2) - (3)].idString);
                    return rv;
                }
    break;

  case 9:
#line 249 "./grammar.y"
    {
                    PRINTF("Bison trying to get help for symbol\n");
                    int rv =  parser.help(*(yyvsp[(2) - (3)].idString));
                    delete (yyvsp[(2) - (3)].idString);
                    return rv;
                }
    break;

  case 10:
#line 256 "./grammar.y"
    {
                    PRINTF("Bison trying to get help for function call\n");
                    int rv =  parser.help(*(yyvsp[(2) - (3)].syntaxFunctionCall));
                    delete (yyvsp[(2) - (3)].syntaxFunctionCall);
                    return rv;
                }
    break;

  case 11:
#line 263 "./grammar.y"
    {
                    PRINTF("Bison trying to get help for function call\n");
                    int rv =  parser.help(*(yyvsp[(2) - (3)].syntaxFunctionCall));
                    delete (yyvsp[(2) - (3)].syntaxFunctionCall);
                    return rv;
                }
    break;

  case 12:
#line 270 "./grammar.y"
    {
                    PRINTF("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
                    YYABORT;
                }
    break;

  case 13:
#line 275 "./grammar.y"
    {
                    PRINTF("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
                    YYABORT;
                }
    break;

  case 14:
#line 281 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 15:
#line 283 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxFunctionCall(new RbString("v"), (yyvsp[(1) - (1)].argumentList)); }
    break;

  case 16:
#line 285 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 17:
#line 287 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UMinus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 18:
#line 288 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UPlus, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 19:
#line 289 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UNot, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 20:
#line 290 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxUnaryExpr(SyntaxUnaryExpr::UAnd, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 21:
#line 292 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Range, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 22:
#line 294 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Add, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 23:
#line 295 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Sub, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 24:
#line 296 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Mul, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 25:
#line 297 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Div, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 26:
#line 298 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Exp, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 27:
#line 300 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Lt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 28:
#line 301 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Le, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 29:
#line 302 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Eq, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 30:
#line 303 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ne, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 31:
#line 304 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ge, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 32:
#line 305 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Gt, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 33:
#line 307 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 34:
#line 308 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 35:
#line 309 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::And2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 36:
#line 310 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or2, (yyvsp[(1) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElement)); }
    break;

  case 37:
#line 312 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 38:
#line 313 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 39:
#line 314 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 40:
#line 315 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 41:
#line 317 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFunctionCall); }
    break;

  case 42:
#line 319 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxVariable); }
    break;

  case 43:
#line 323 "./grammar.y"
    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::ArrowAssign, (yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 44:
#line 328 "./grammar.y"
    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::ArrowAssign, (yyvsp[(1) - (3)].syntaxFunctionCall), (yyvsp[(3) - (3)].syntaxElement));
                    }
    break;

  case 45:
#line 335 "./grammar.y"
    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::TildeAssign, (yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxFunctionCall));
                    }
    break;

  case 46:
#line 340 "./grammar.y"
    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::TildeAssign, (yyvsp[(1) - (3)].syntaxFunctionCall), (yyvsp[(3) - (3)].syntaxFunctionCall));
                    }
    break;

  case 47:
#line 347 "./grammar.y"
    {
                        PRINTF("Parser inserting tilde iid assignment (TILDEIID_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::TildeIidAssign, (yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxFunctionCall));
                    }
    break;

  case 48:
#line 352 "./grammar.y"
    {
                        PRINTF("Parser inserting tilde iid assignment (TILDEIID_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::TildeIidAssign, (yyvsp[(1) - (3)].syntaxFunctionCall), (yyvsp[(3) - (3)].syntaxFunctionCall));
                    }
    break;

  case 49:
#line 359 "./grammar.y"
    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::EquationAssign, (yyvsp[(1) - (3)].syntaxVariable), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 50:
#line 364 "./grammar.y"
    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxAssignExpr(SyntaxAssignExpr::EquationAssign, (yyvsp[(1) - (3)].syntaxFunctionCall), (yyvsp[(3) - (3)].syntaxElement)); 
                    }
    break;

  case 51:
#line 371 "./grammar.y"
    {
                    PRINTF("Parser inserting variable (NAMED_VAR)in syntax tree\n");
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (2)].idString), (yyvsp[(2) - (2)].syntaxElementList));
                }
    break;

  case 52:
#line 376 "./grammar.y"
    {
                    PRINTF("Parser inserting variable (FUNCTION_VAR) in syntax tree\n");
                    (yyvsp[(5) - (5)].syntaxElementList)->push_front((yyvsp[(3) - (5)].syntaxElement));
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (5)].syntaxFunctionCall), (yyvsp[(5) - (5)].syntaxElementList));
                }
    break;

  case 53:
#line 382 "./grammar.y"
    {
                    PRINTF("Parser inserting member variable (NAMED_VAR)in syntax tree\n");
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (4)].syntaxVariable), (yyvsp[(3) - (4)].idString), (yyvsp[(4) - (4)].syntaxElementList));
                }
    break;

  case 54:
#line 387 "./grammar.y"
    {
                    PRINTF("Parser inserting member variable (FUNCTION_VAR) in syntax tree\n");
                    (yyvsp[(7) - (7)].syntaxElementList)->push_front((yyvsp[(5) - (7)].syntaxElement));
                    (yyval.syntaxVariable) = new SyntaxVariable((yyvsp[(1) - (7)].syntaxVariable), (yyvsp[(3) - (7)].syntaxFunctionCall), (yyvsp[(7) - (7)].syntaxElementList));
                }
    break;

  case 55:
#line 394 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 56:
#line 395 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (1)].syntaxElementList); }
    break;

  case 57:
#line 398 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(2) - (3)].syntaxElement)); }
    break;

  case 58:
#line 399 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, NULL); }
    break;

  case 59:
#line 400 "./grammar.y"
    { (yyvsp[(1) - (4)].syntaxElementList)->push_back((yyvsp[(3) - (4)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (4)].syntaxElementList); }
    break;

  case 60:
#line 401 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back( NULL ); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 61:
#line 405 "./grammar.y"
    {
                    (yyval.syntaxFunctionCall) = new SyntaxFunctionCall((yyvsp[(1) - (4)].idString), (yyvsp[(3) - (4)].argumentList));
                }
    break;

  case 62:
#line 411 "./grammar.y"
    {
                        PRINTF("Parser inserting function call in syntax tree\n");
                        (yyval.syntaxFunctionCall) = (yyvsp[(1) - (1)].syntaxFunctionCall);
                    }
    break;

  case 63:
#line 416 "./grammar.y"
    {
                        PRINTF("Parser inserting member call in syntax tree\n");
                        (yyvsp[(3) - (3)].syntaxFunctionCall)->setBaseVariable((yyvsp[(1) - (3)].syntaxVariable));
                        (yyval.syntaxFunctionCall) = (yyvsp[(3) - (3)].syntaxFunctionCall);
                    }
    break;

  case 64:
#line 423 "./grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(); }
    break;

  case 65:
#line 424 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(1) - (1)].argumentList); }
    break;

  case 66:
#line 427 "./grammar.y"
    { (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(1,(yyvsp[(1) - (1)].syntaxLabeledExpr)); }
    break;

  case 67:
#line 428 "./grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back((yyvsp[(3) - (3)].syntaxLabeledExpr)); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 68:
#line 432 "./grammar.y"
    {
                    PRINTF("Parser inserting unlabeled argument in syntax tree\n");
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr( new RbString(""), (yyvsp[(1) - (1)].syntaxElement));
                }
    break;

  case 69:
#line 437 "./grammar.y"
    { 
                    PRINTF("Parser inserting labeled argument in syntax tree\n");
                    (yyval.syntaxLabeledExpr) = new SyntaxLabeledExpr((yyvsp[(1) - (3)].idString), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 70:
#line 444 "./grammar.y"
    {
                    PRINTF("Parser inserting function definition in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxFunctionDef(NULL, (yyvsp[(2) - (6)].idString), (yyvsp[(4) - (6)].formalList), (yyvsp[(6) - (6)].syntaxElementList));
                }
    break;

  case 71:
#line 450 "./grammar.y"
    {
                    PRINTF("Parser inserting typed function definition in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxFunctionDef((yyvsp[(2) - (7)].idString), (yyvsp[(3) - (7)].idString), (yyvsp[(5) - (7)].formalList), (yyvsp[(7) - (7)].syntaxElementList));
                }
    break;

  case 72:
#line 456 "./grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal*>(); }
    break;

  case 73:
#line 457 "./grammar.y"
    { (yyval.formalList) = (yyvsp[(1) - (1)].formalList); }
    break;

  case 74:
#line 460 "./grammar.y"
    { (yyval.formalList) = new std::list<SyntaxFormal*>(1, (yyvsp[(1) - (1)].syntaxFormal)); }
    break;

  case 75:
#line 461 "./grammar.y"
    { (yyvsp[(1) - (3)].formalList)->push_back((yyvsp[(3) - (3)].syntaxFormal)); (yyval.formalList) = (yyvsp[(1) - (3)].formalList); }
    break;

  case 76:
#line 465 "./grammar.y"
    {
                    PRINTF("Inserting labeled formal argument without default in syntax tree\n");
                    (yyval.syntaxFormal) = new SyntaxFormal((yyvsp[(1) - (1)].idString), NULL );
                }
    break;

  case 77:
#line 470 "./grammar.y"
    {
                    PRINTF("Inserting labeled formal argument with default in syntax tree\n");
                    (yyval.syntaxFormal) = new SyntaxFormal((yyvsp[(1) - (3)].idString), (yyvsp[(3) - (3)].syntaxElement));
                }
    break;

  case 78:
#line 475 "./grammar.y"
    {
                    PRINTF("Inserting typed labeled formal argument without default in syntax tree\n");
                    (yyval.syntaxFormal) = new SyntaxFormal((yyvsp[(1) - (2)].idString), (yyvsp[(2) - (2)].idString), NULL);
                }
    break;

  case 79:
#line 480 "./grammar.y"
    {
                    PRINTF("Inserting typed labeled formal argument with default in syntax tree\n");
                    (yyval.syntaxFormal) = new SyntaxFormal((yyvsp[(1) - (4)].idString), (yyvsp[(2) - (4)].idString), (yyvsp[(4) - (4)].syntaxElement));
                }
    break;

  case 80:
#line 486 "./grammar.y"
    { (yyvsp[(1) - (3)].idString)->append(*((yyvsp[(2) - (3)].idString))); (yyvsp[(1) - (3)].idString)->append(*((yyvsp[(3) - (3)].idString))); (yyval.idString) = (yyvsp[(1) - (3)].idString); }
    break;

  case 81:
#line 489 "./grammar.y"
    { (yyval.idString) = new RbString(); }
    break;

  case 82:
#line 490 "./grammar.y"
    { (yyval.idString) = (yyvsp[(1) - (1)].idString); }
    break;

  case 83:
#line 493 "./grammar.y"
    { (yyval.idString) = new RbString("[]"); }
    break;

  case 84:
#line 494 "./grammar.y"
    { (yyval.idString) = new RbString("["); (yyval.idString)->append(INT); (yyval.idString)->append("]"); }
    break;

  case 85:
#line 495 "./grammar.y"
    { (yyvsp[(1) - (3)].idString)->append("[]"); (yyval.idString) = (yyvsp[(1) - (3)].idString); }
    break;

  case 86:
#line 496 "./grammar.y"
    { (yyvsp[(1) - (4)].idString)->append("["); (yyvsp[(1) - (4)].idString)->append(INT); (yyvsp[(1) - (4)].idString)->append("]"); (yyval.idString) = (yyvsp[(1) - (4)].idString); }
    break;

  case 87:
#line 499 "./grammar.y"
    { (yyval.idString) = new RbString(); }
    break;

  case 88:
#line 500 "./grammar.y"
    { (yyval.idString) = new RbString("&"); }
    break;

  case 89:
#line 503 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 90:
#line 505 "./grammar.y"
    {
                    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();
                    stmts->push_back((yyvsp[(1) - (1)].syntaxElement));
                    (yyval.syntaxElementList) = stmts;
                }
    break;

  case 91:
#line 512 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 92:
#line 513 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 93:
#line 514 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 94:
#line 515 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 95:
#line 516 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 96:
#line 517 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 97:
#line 520 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 98:
#line 521 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 99:
#line 522 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 100:
#line 523 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 101:
#line 524 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 102:
#line 525 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 103:
#line 528 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 104:
#line 529 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 105:
#line 532 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 106:
#line 533 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 107:
#line 535 "./grammar.y"
    {
                        PRINTF("Parser inserting variable declaration in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxVariableDecl((yyvsp[(1) - (4)].idString), (yyvsp[(2) - (4)].syntaxElementList), (yyvsp[(3) - (4)].idString), (yyvsp[(4) - (4)].idString));
                    }
    break;

  case 108:
#line 541 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(); }
    break;

  case 109:
#line 542 "./grammar.y"
    { (yyval.syntaxElementList) = new std::list<SyntaxElement*>(1, (yyvsp[(1) - (1)].syntaxElement)); }
    break;

  case 110:
#line 543 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 111:
#line 544 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 112:
#line 545 "./grammar.y"
    { (yyvsp[(1) - (3)].syntaxElementList)->push_back((yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 113:
#line 546 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 114:
#line 549 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxFormal); }
    break;

  case 115:
#line 550 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 116:
#line 554 "./grammar.y"
    {
                PRINTF("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxClassDef((yyvsp[(2) - (7)].idString), (yyvsp[(4) - (7)].idString), (yyvsp[(6) - (7)].syntaxElementList));
                }
    break;

  case 117:
#line 560 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::If, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 118:
#line 561 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::IfElse, (yyvsp[(2) - (5)].syntaxElement), (yyvsp[(3) - (5)].syntaxElementList), (yyvsp[(5) - (5)].syntaxElementList)); }
    break;

  case 119:
#line 563 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 120:
#line 566 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::For, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 121:
#line 569 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxForCondition((yyvsp[(2) - (5)].idString), (yyvsp[(4) - (5)].syntaxElement)); }
    break;

  case 122:
#line 572 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::While, (yyvsp[(2) - (3)].syntaxElement), (yyvsp[(3) - (3)].syntaxElementList)); }
    break;

  case 123:
#line 575 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Next); }
    break;

  case 124:
#line 578 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Break); }
    break;

  case 125:
#line 581 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return); }
    break;

  case 126:
#line 582 "./grammar.y"
    { (yyval.syntaxElement) = new SyntaxStatement(SyntaxStatement::Return, (yyvsp[(2) - (2)].syntaxElement)); }
    break;

  case 127:
#line 585 "./grammar.y"
    { (yyval.idString) = new RbString((yyvsp[(1) - (1)].string)); }
    break;

  case 128:
#line 589 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(2) - (3)].argumentList); }
    break;

  case 129:
#line 592 "./grammar.y"
    { (yyvsp[(1) - (3)].argumentList)->push_back(new SyntaxLabeledExpr( new RbString(""), (yyvsp[(3) - (3)].syntaxElement)) ); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); }
    break;

  case 130:
#line 594 "./grammar.y"
    {
                (yyval.argumentList) = new std::list<SyntaxLabeledExpr*>(1, new SyntaxLabeledExpr(new RbString(""), (yyvsp[(1) - (1)].syntaxElement)) );
                }
    break;

  case 131:
#line 600 "./grammar.y"
    {
                    PRINTF("Parser inserting bool constant (false) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxConstant(new RbBoolean(false) );
                }
    break;

  case 132:
#line 605 "./grammar.y"
    {
                    PRINTF("Parser inserting bool constant (true) in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxConstant(new RbBoolean(true) );
                }
    break;

  case 133:
#line 610 "./grammar.y"
    {
                    PRINTF("Parser inserting null constant in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxConstant( NULL );
                }
    break;

  case 134:
#line 615 "./grammar.y"
    {
                    if ( (yyvsp[(1) - (1)].intValue) < 0 ) {
                        PRINTF("Parser inserting Integer constant in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxConstant(new Integer((yyvsp[(1) - (1)].intValue)) );
                    }
                    else { 
                        PRINTF("Parser inserting Natural constant in syntax tree\n");
                        (yyval.syntaxElement) = new SyntaxConstant(new Natural((yyvsp[(1) - (1)].intValue)) );
                    }
                }
    break;

  case 135:
#line 626 "./grammar.y"
    {
                    PRINTF("Parser inserting String constant in syntax tree\n");
                    (yyval.syntaxElement) = new SyntaxConstant(new RbString((yyvsp[(1) - (1)].string)) );
                }
    break;

  case 136:
#line 631 "./grammar.y"
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
                    if ($1 > 0.0)
                        real = new RealPos($1);
                    else
                        real = new Real($1);
                    real->setPrecision(prec);
                    real->setScientific(sci);
                    */
                    if ((yyvsp[(1) - (1)].realValue) > 0.0) {
                        PRINTF("Parser inserting RealPos constant in syntax tree \n");
                        (yyval.syntaxElement) = new SyntaxConstant(new RealPos((yyvsp[(1) - (1)].realValue)) );
                    }
                    else {
                        PRINTF("Parser inserting Real constant in syntax tree \n");
                        (yyval.syntaxElement) = new SyntaxConstant(new Real((yyvsp[(1) - (1)].realValue)) );
                    }
                }
    break;


/* Line 1267 of yacc.c.  */
#line 2892 "./grammar.tab.c"
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


#line 680 "./grammar.y"



/* Definition of yyerror. */
int yyerror(const char *msg)
{
    PRINTF("Bison code said: %s\n", msg);
    if ( foundNewline == true )
        foundErrorBeforeEnd = false;
    else
        foundErrorBeforeEnd = true;

    yylloc.first_column = yycolumn - yyleng;
    yylloc.last_column  = yycolumn - 1;

    return 1;
}



