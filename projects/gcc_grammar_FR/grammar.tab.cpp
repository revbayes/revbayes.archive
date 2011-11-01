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
     TILDEIID_ASSIGN = 278,
     EQUATION_ASSIGN = 279,
     EQUAL = 280,
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
     UNOT = 292,
     UPLUS = 293,
     UMINUS = 294,
     UAND = 295
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
#define TILDEIID_ASSIGN 278
#define EQUATION_ASSIGN 279
#define EQUAL 280
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
#define UNOT 292
#define UPLUS 293
#define UMINUS 294
#define UAND 295




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
 * @author Fredrik Ronquist
 * @date Last modified: $Date$
 *
 * $Id$
 */

/* The following statements go into the resulting C code */

#include "RbBoolean.h"
#include "Integer.h"
#include "Natural.h"
#include "Parser.h"
#include "RbPtr.h"
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
#line 92 "./grammar.y"
{
    char*                                           string;
    double                                          realValue;
    int                                             intValue;
    bool                                            boolValue;
    RbPtr<RbString>*                                idString;
    RbPtr<SyntaxElement>*                           syntaxElement;
    RbPtr<SyntaxVariable>*                          syntaxVariable;
    RbPtr<SyntaxFunctionCall>*                      syntaxFunctionCall;
    RbPtr<SyntaxLabeledExpr>*                       syntaxLabeledExpr;
    RbPtr<SyntaxFormal>*                            syntaxFormal;
    RbPtr<std::list<RbPtr<SyntaxElement> > >*       syntaxElementList;
    RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > >*   argumentList;
    RbPtr<std::list<RbPtr<SyntaxFormal> > >*        formalList;
}
/* Line 193 of yacc.c.  */
#line 282 "./grammar.tab.c"
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
#line 307 "./grammar.tab.c"

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
#define YYFINAL  73
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   615

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  137
/* YYNRULES -- Number of states.  */
#define YYNSTATES  234

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
       2,     2,     2,    54,     2,     2,     2,     2,    57,     2,
      49,    53,    41,    39,    56,    40,    47,    42,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,    52,
       2,     2,     2,    37,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    55,    46,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    58,     2,    59,     2,     2,     2,     2,
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
      35,    36,    38,    44,    45,    48
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    16,    19,    22,
      26,    30,    34,    38,    41,    44,    46,    48,    52,    55,
      58,    61,    64,    68,    72,    76,    80,    84,    88,    92,
      96,   100,   104,   108,   112,   116,   120,   124,   128,   130,
     132,   134,   136,   138,   140,   144,   148,   152,   156,   160,
     164,   168,   172,   175,   181,   186,   194,   195,   197,   201,
     204,   209,   213,   218,   220,   224,   225,   227,   229,   233,
     235,   239,   246,   254,   255,   257,   259,   263,   265,   269,
     272,   277,   281,   282,   284,   287,   291,   295,   300,   301,
     303,   307,   309,   310,   312,   316,   319,   323,   326,   328,
     330,   332,   334,   336,   338,   340,   342,   344,   346,   351,
     352,   354,   358,   361,   365,   368,   370,   372,   380,   384,
     390,   394,   398,   404,   408,   410,   412,   414,   417,   419,
     423,   427,   429,   431,   433,   435,   437,   439
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      61,     0,    -1,    36,    -1,    51,    -1,    10,    51,    -1,
      86,    51,    -1,    86,    52,    -1,    87,    51,    -1,    87,
      52,    -1,    37,    99,    51,    -1,    37,    99,    52,    -1,
      37,    71,    51,    -1,    37,    71,    52,    -1,     1,    51,
      -1,     1,    52,    -1,   102,    -1,   100,    -1,    49,    62,
      53,    -1,    40,    62,    -1,    39,    62,    -1,    54,    62,
      -1,    26,    62,    -1,    62,    43,    62,    -1,    62,    39,
      62,    -1,    62,    40,    62,    -1,    62,    41,    62,    -1,
      62,    42,    62,    -1,    62,    46,    62,    -1,    62,    32,
      62,    -1,    62,    33,    62,    -1,    62,    34,    62,    -1,
      62,    35,    62,    -1,    62,    31,    62,    -1,    62,    30,
      62,    -1,    62,    26,    62,    -1,    62,    27,    62,    -1,
      62,    28,    62,    -1,    62,    29,    62,    -1,    63,    -1,
      66,    -1,    64,    -1,    65,    -1,    71,    -1,    67,    -1,
      67,    21,    62,    -1,    71,    21,    62,    -1,    67,    22,
      71,    -1,    71,    22,    71,    -1,    67,    23,    71,    -1,
      71,    23,    71,    -1,    67,    24,    62,    -1,    71,    24,
      62,    -1,    99,    68,    -1,    70,    50,    62,    55,    68,
      -1,    67,    47,    99,    68,    -1,    67,    47,    70,    50,
      62,    55,    68,    -1,    -1,    69,    -1,    50,    62,    55,
      -1,    50,    55,    -1,    69,    50,    62,    55,    -1,    69,
      50,    55,    -1,    99,    49,    72,    53,    -1,    70,    -1,
      67,    47,    70,    -1,    -1,    73,    -1,    74,    -1,    73,
      56,    74,    -1,    62,    -1,    99,    25,    62,    -1,    11,
      99,    49,    76,    53,    83,    -1,    11,    79,    99,    49,
      76,    53,    83,    -1,    -1,    77,    -1,    78,    -1,    77,
      56,    78,    -1,    99,    -1,    99,    25,    62,    -1,    79,
      99,    -1,    79,    99,    25,    62,    -1,    99,    80,    82,
      -1,    -1,    81,    -1,    50,    55,    -1,    50,     4,    55,
      -1,    81,    50,    55,    -1,    81,    50,     4,    55,    -1,
      -1,    57,    -1,    58,    84,    59,    -1,    86,    -1,    -1,
      86,    -1,    84,    52,    86,    -1,    84,    52,    -1,    84,
      51,    86,    -1,    84,    51,    -1,    91,    -1,    93,    -1,
      95,    -1,    96,    -1,    97,    -1,    98,    -1,    85,    -1,
      62,    -1,    90,    -1,    75,    -1,    99,    68,    82,    99,
      -1,    -1,    89,    -1,    88,    52,    89,    -1,    88,    52,
      -1,    88,    51,    89,    -1,    88,    51,    -1,    78,    -1,
      75,    -1,    12,    99,    43,    99,    58,    88,    59,    -1,
      15,    92,    83,    -1,    15,    92,    83,    16,    83,    -1,
      49,    62,    53,    -1,    13,    94,    83,    -1,    49,    99,
      14,    62,    53,    -1,    17,    92,    83,    -1,    18,    -1,
      19,    -1,    20,    -1,    20,    62,    -1,     5,    -1,    50,
     101,    55,    -1,   101,    56,    62,    -1,    62,    -1,     8,
      -1,     9,    -1,     7,    -1,     4,    -1,     6,    -1,     3,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   218,   218,   223,   228,   233,   240,   247,   254,   261,
     268,   275,   282,   289,   294,   301,   303,   305,   307,   308,
     309,   310,   312,   314,   315,   316,   317,   318,   320,   321,
     322,   323,   324,   325,   327,   328,   329,   330,   332,   333,
     334,   335,   337,   339,   342,   348,   356,   362,   370,   376,
     384,   390,   398,   404,   411,   417,   426,   427,   430,   431,
     432,   433,   436,   443,   448,   457,   458,   461,   462,   465,
     471,   479,   486,   494,   495,   498,   499,   502,   508,   514,
     520,   528,   531,   532,   535,   536,   537,   538,   541,   542,
     545,   546,   555,   556,   557,   558,   559,   560,   563,   564,
     565,   566,   567,   568,   571,   572,   575,   576,   577,   585,
     586,   587,   588,   589,   590,   593,   594,   597,   604,   605,
     607,   610,   613,   616,   619,   622,   625,   626,   629,   633,
     636,   637,   640,   645,   650,   655,   666,   671
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
     285,   286,   287,   288,   289,   290,   291,    63,   292,    43,
      45,    42,    47,    58,   293,   294,    94,    46,   295,    40,
      91,    10,    59,    41,    33,    93,    44,    38,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    60,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    63,    63,    64,    64,    65,    65,
      66,    66,    67,    67,    67,    67,    68,    68,    69,    69,
      69,    69,    70,    71,    71,    72,    72,    73,    73,    74,
      74,    75,    75,    76,    76,    77,    77,    78,    78,    78,
      78,    79,    80,    80,    81,    81,    81,    81,    82,    82,
      83,    83,    84,    84,    84,    84,    84,    84,    85,    85,
      85,    85,    85,    85,    86,    86,    87,    87,    87,    88,
      88,    88,    88,    88,    88,    89,    89,    90,    91,    91,
      92,    93,    94,    95,    96,    97,    98,    98,    99,   100,
     101,   101,   102,   102,   102,   102,   102,   102
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     2,     2,     1,     1,     3,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     5,     4,     7,     0,     1,     3,     2,
       4,     3,     4,     1,     3,     0,     1,     1,     3,     1,
       3,     6,     7,     0,     1,     1,     3,     1,     3,     2,
       4,     3,     0,     1,     2,     3,     3,     4,     0,     1,
       3,     1,     0,     1,     3,     2,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     0,
       1,     3,     2,     3,     2,     1,     1,     7,     3,     5,
       3,     3,     5,     3,     1,     1,     1,     2,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   137,   135,   128,   136,   134,   132,   133,     0,
       0,     0,     0,     0,     0,   124,   125,   126,     0,     2,
       0,     0,     0,     0,     0,     3,     0,     0,   105,    38,
      40,    41,    39,    43,    63,    42,   107,   104,     0,     0,
     106,    98,    99,   100,   101,   102,   103,    56,    16,    15,
      13,    14,     4,     0,    82,     0,     0,     0,     0,     0,
       0,   127,    56,    21,     0,     0,    56,    19,    18,     0,
     131,     0,    20,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     8,    65,     0,    52,    57,     0,    73,
       0,    88,    83,     0,     0,    92,   121,    91,     0,   118,
     123,    52,    11,    12,     9,    10,    17,   129,     0,    34,
      35,    36,    37,    33,    32,    28,    29,    30,    31,    23,
      24,    25,    26,    22,    27,    44,    46,    48,    50,    64,
      56,     0,    45,    47,    49,    51,    69,     0,    66,    67,
      56,    59,     0,    89,     0,     0,    73,     0,    74,    75,
       0,    77,     0,    84,    81,     0,     0,     0,     0,    93,
     120,     0,   130,     0,    54,    56,    62,     0,     0,    58,
     108,    61,     0,     0,     0,     0,    79,     0,    85,     0,
      86,   109,     0,    97,    95,    90,   119,     0,    53,    68,
      70,    60,     0,    71,    76,     0,    78,    87,   116,   115,
       0,   110,   122,    96,    94,    56,    72,    80,   114,   112,
     117,    55,   113,   111
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    27,    28,    29,    30,    31,    32,    33,   121,   107,
      34,    35,   157,   158,   159,   218,   167,   168,   219,   170,
     111,   112,   164,   116,   178,    37,   117,    39,   220,   221,
      40,    41,    59,    42,    57,    43,    44,    45,    46,    62,
      48,    71,    49
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -107
static const yytype_int16 yypact[] =
{
     260,     3,  -107,  -107,  -107,  -107,  -107,  -107,  -107,   -32,
      16,    16,   -43,   -21,   -21,  -107,  -107,   364,   364,  -107,
      16,   364,   364,   364,   364,  -107,   364,    39,   569,  -107,
    -107,  -107,  -107,    14,    38,   120,  -107,  -107,     6,    12,
    -107,  -107,  -107,  -107,  -107,  -107,  -107,    42,  -107,  -107,
    -107,  -107,  -107,    16,    48,     5,    16,   204,   364,   204,
     204,   569,    42,  -107,    15,    51,   109,    47,    47,   206,
     569,    59,    90,  -107,   364,   364,   364,   364,   364,   364,
     364,   364,   364,   364,   364,   364,   364,   364,   364,   364,
     364,    16,    16,   364,    16,   364,   364,    16,    16,   364,
    -107,  -107,  -107,  -107,   364,   143,    25,    49,    61,    16,
      28,    -6,    58,    16,   121,   339,  -107,  -107,   513,   118,
    -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,   364,    90,
     366,    90,   366,   288,   288,   288,   288,   288,   288,     4,
       4,    -2,    -2,    47,    47,   569,  -107,  -107,   569,    87,
      42,   393,   569,  -107,  -107,   569,   569,    85,    89,  -107,
      -7,  -107,   423,  -107,    16,   286,    16,   100,    98,  -107,
      16,     2,   107,  -107,  -107,    30,    97,   364,   -28,  -107,
    -107,   204,   569,   364,  -107,   113,  -107,   364,   364,  -107,
    -107,  -107,   453,   112,   204,    16,   142,   364,  -107,   116,
    -107,    11,   541,   339,   339,  -107,  -107,   483,  -107,  -107,
     569,  -107,   204,  -107,  -107,   364,   569,  -107,  -107,  -107,
     -26,  -107,  -107,  -107,  -107,   113,  -107,   569,    11,    11,
    -107,  -107,  -107,  -107
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -107,  -107,    -9,  -107,  -107,  -107,  -107,     9,   -45,  -107,
      78,    20,  -107,  -107,   -14,   175,    10,  -107,  -106,   167,
    -107,  -107,    70,   -55,  -107,  -107,     1,  -107,  -107,  -101,
    -107,  -107,   170,  -107,  -107,  -107,  -107,  -107,  -107,     0,
    -107,  -107,  -107
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -89
static const yytype_int16 yytable[] =
{
      47,    38,   106,   169,   119,   120,    56,   -82,    61,    63,
      54,    55,    67,    68,    69,    70,     4,    72,   188,    52,
      66,     4,    10,   203,   204,   228,   229,   197,    58,    64,
     -88,   205,   172,   230,   199,    90,    91,    92,    93,    73,
      65,    88,   104,   105,    89,    86,    87,    88,   113,   118,
      89,   163,   110,   108,    50,    51,   114,   100,   101,   -82,
     169,    94,    94,   102,   103,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   163,   173,   148,   200,   151,   152,    95,   214,
     155,   104,   105,    89,   150,   156,   162,   109,   110,   165,
      64,    64,   122,   123,   160,   184,    64,    64,   175,   171,
     166,   146,   147,   176,   127,   128,   179,   153,   154,   182,
      78,    79,    80,    81,    82,    83,   206,   232,   233,    84,
      85,    86,    87,    88,   181,   177,    89,   183,   186,   213,
     208,    96,    97,    98,    99,   187,     2,     3,     4,     5,
       6,     7,     8,   194,   195,   201,   192,   226,   104,   105,
     124,   125,   198,   105,   190,   212,   171,   215,   202,    18,
     196,   217,   149,   209,   207,    36,   193,    53,   156,   210,
     231,   174,    21,    22,    60,     0,     0,   160,   216,     0,
       0,     0,    23,    24,     0,   171,     0,    26,   161,     0,
       0,   171,     0,     0,   223,   224,   227,     2,     3,     4,
       5,     6,     7,     8,     0,     0,     0,    12,     0,    13,
       0,    14,    15,    16,    17,     0,     0,     0,   171,   171,
      18,     0,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,     0,    21,    22,    84,    85,    86,    87,    88,
       0,     0,    89,    23,    24,     0,     0,     0,    26,   126,
       0,     1,   115,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     0,    13,     0,    14,    15,    16,
      17,     0,     0,     0,     0,     0,    18,     0,     0,     2,
       3,     4,     5,     6,     7,     8,    19,    20,     0,    21,
      22,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    18,     0,    26,     0,     0,     0,   -89,   -89,
     -89,   -89,   -89,   -89,     0,    21,    22,    84,    85,    86,
      87,    88,     0,     0,    89,    23,    24,     0,     0,     0,
      26,   191,     2,     3,     4,     5,     6,     7,     8,     0,
       0,     0,    12,     0,    13,     0,    14,    15,    16,    17,
       0,     0,     0,     0,     0,    18,     0,     2,     3,     4,
       5,     6,     7,     8,     0,     0,     0,     0,    21,    22,
       0,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      18,     0,    74,    26,    76,     0,    78,    79,    80,    81,
      82,    83,     0,    21,    22,    84,    85,    86,    87,    88,
       0,     0,    89,    23,    24,     0,     0,     0,    26,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,     0,
       0,     0,    84,    85,    86,    87,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,   185,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,     0,
       0,     0,    84,    85,    86,    87,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,   189,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,     0,
       0,     0,    84,    85,    86,    87,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,   211,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,     0,
       0,     0,    84,    85,    86,    87,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,   225,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,     0,
       0,     0,    84,    85,    86,    87,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,   180,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,     0,     0,     0,
      84,    85,    86,    87,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,   222,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,     0,     0,     0,    84,    85,
      86,    87,    88,     0,     0,    89
};

static const yytype_int16 yycheck[] =
{
       0,     0,    47,   109,    59,    60,    49,     5,    17,    18,
      10,    11,    21,    22,    23,    24,     5,    26,    25,    51,
      20,     5,    11,    51,    52,    51,    52,    25,    49,    20,
       5,    59,     4,    59,     4,    21,    22,    23,    24,     0,
      20,    43,    49,    50,    46,    41,    42,    43,    43,    58,
      46,    57,    50,    53,    51,    52,    56,    51,    52,    57,
     166,    47,    47,    51,    52,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    57,    55,    93,    55,    95,    96,    50,   195,
      99,    49,    50,    46,    94,   104,   105,    49,    50,    50,
      91,    92,    51,    52,   104,   150,    97,    98,    50,   109,
      49,    91,    92,   113,    55,    56,   115,    97,    98,   128,
      30,    31,    32,    33,    34,    35,   181,   228,   229,    39,
      40,    41,    42,    43,    16,    14,    46,    50,    53,   194,
     185,    21,    22,    23,    24,    56,     3,     4,     5,     6,
       7,     8,     9,    53,    56,    58,   165,   212,    49,    50,
      51,    52,    55,    50,   164,    53,   166,    25,   177,    26,
     170,    55,    94,   187,   183,     0,   166,    10,   187,   188,
     225,   111,    39,    40,    14,    -1,    -1,   187,   197,    -1,
      -1,    -1,    49,    50,    -1,   195,    -1,    54,    55,    -1,
      -1,   201,    -1,    -1,   203,   204,   215,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    15,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,   228,   229,
      26,    -1,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    -1,    39,    40,    39,    40,    41,    42,    43,
      -1,    -1,    46,    49,    50,    -1,    -1,    -1,    54,    53,
      -1,     1,    58,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    -1,    15,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    36,    37,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,    26,    -1,    54,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    -1,    39,    40,    39,    40,    41,
      42,    43,    -1,    -1,    46,    49,    50,    -1,    -1,    -1,
      54,    55,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    13,    -1,    15,    -1,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    26,    -1,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    -1,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      26,    -1,    26,    54,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    39,    40,    39,    40,    41,    42,    43,
      -1,    -1,    46,    49,    50,    -1,    -1,    -1,    54,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      39,    40,    41,    42,    43,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    39,    40,
      41,    42,    43,    -1,    -1,    46
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    15,    17,    18,    19,    20,    26,    36,
      37,    39,    40,    49,    50,    51,    54,    61,    62,    63,
      64,    65,    66,    67,    70,    71,    75,    85,    86,    87,
      90,    91,    93,    95,    96,    97,    98,    99,   100,   102,
      51,    52,    51,    79,    99,    99,    49,    94,    49,    92,
      92,    62,    99,    62,    67,    71,    99,    62,    62,    62,
      62,   101,    62,     0,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    39,    40,    41,    42,    43,    46,
      21,    22,    23,    24,    47,    50,    21,    22,    23,    24,
      51,    52,    51,    52,    49,    50,    68,    69,    99,    49,
      50,    80,    81,    43,    99,    58,    83,    86,    62,    83,
      83,    68,    51,    52,    51,    52,    53,    55,    56,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    71,    71,    62,    70,
      99,    62,    62,    71,    71,    62,    62,    72,    73,    74,
      99,    55,    62,    57,    82,    50,    49,    76,    77,    78,
      79,    99,     4,    55,    82,    50,    99,    14,    84,    86,
      53,    16,    62,    50,    68,    55,    53,    56,    25,    55,
      99,    55,    62,    76,    53,    56,    99,    25,    55,     4,
      55,    58,    62,    51,    52,    59,    83,    62,    68,    74,
      62,    55,    53,    83,    78,    25,    62,    55,    75,    78,
      88,    89,    53,    86,    86,    55,    83,    62,    51,    52,
      59,    68,    89,    89
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
      case 62: /* "expression" */
#line 145 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting expression  ...\n"); };
#line 1510 "./grammar.tab.c"
	break;
      case 63: /* "arrowAssign" */
#line 146 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1515 "./grammar.tab.c"
	break;
      case 64: /* "tildeAssign" */
#line 146 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1520 "./grammar.tab.c"
	break;
      case 65: /* "tildeIidAssign" */
#line 146 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1525 "./grammar.tab.c"
	break;
      case 66: /* "equationAssign" */
#line 146 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting assignment  ...\n"); };
#line 1530 "./grammar.tab.c"
	break;
      case 67: /* "variable" */
#line 144 "./grammar.y"
	{ delete ((yyvaluep->syntaxVariable)); PRINTF("Deleting variable    ...\n"); };
#line 1535 "./grammar.tab.c"
	break;
      case 68: /* "optElements" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1540 "./grammar.tab.c"
	break;
      case 69: /* "elementList" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1545 "./grammar.tab.c"
	break;
      case 70: /* "fxnCall" */
#line 144 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); PRINTF("Deleting variable    ...\n"); };
#line 1550 "./grammar.tab.c"
	break;
      case 71: /* "functionCall" */
#line 144 "./grammar.y"
	{ delete ((yyvaluep->syntaxFunctionCall)); PRINTF("Deleting variable    ...\n"); };
#line 1555 "./grammar.tab.c"
	break;
      case 72: /* "optArguments" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1560 "./grammar.tab.c"
	break;
      case 73: /* "argumentList" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1565 "./grammar.tab.c"
	break;
      case 74: /* "argument" */
#line 144 "./grammar.y"
	{ delete ((yyvaluep->syntaxLabeledExpr)); PRINTF("Deleting variable    ...\n"); };
#line 1570 "./grammar.tab.c"
	break;
      case 75: /* "functionDef" */
#line 148 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting functiondef ...\n"); };
#line 1575 "./grammar.tab.c"
	break;
      case 76: /* "optFormals" */
#line 142 "./grammar.y"
	{ delete ((yyvaluep->formalList)); PRINTF("Deleting formal list\n"); };
#line 1580 "./grammar.tab.c"
	break;
      case 77: /* "formalList" */
#line 142 "./grammar.y"
	{ delete ((yyvaluep->formalList)); PRINTF("Deleting formal list\n"); };
#line 1585 "./grammar.tab.c"
	break;
      case 78: /* "formal" */
#line 144 "./grammar.y"
	{ delete ((yyvaluep->syntaxFormal)); PRINTF("Deleting variable    ...\n"); };
#line 1590 "./grammar.tab.c"
	break;
      case 79: /* "typeSpec" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1595 "./grammar.tab.c"
	break;
      case 80: /* "optDims" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1600 "./grammar.tab.c"
	break;
      case 81: /* "dimList" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1605 "./grammar.tab.c"
	break;
      case 82: /* "optRef" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1610 "./grammar.tab.c"
	break;
      case 83: /* "stmts" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1615 "./grammar.tab.c"
	break;
      case 84: /* "stmtList" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1620 "./grammar.tab.c"
	break;
      case 85: /* "statement" */
#line 145 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting expression  ...\n"); };
#line 1625 "./grammar.tab.c"
	break;
      case 86: /* "stmt_or_expr" */
#line 145 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting expression  ...\n"); };
#line 1630 "./grammar.tab.c"
	break;
      case 87: /* "declaration" */
#line 147 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting declaration ...\n"); };
#line 1635 "./grammar.tab.c"
	break;
      case 88: /* "memberDefs" */
#line 140 "./grammar.y"
	{ delete ((yyvaluep->syntaxElementList)); PRINTF("Deleting element list\n"); };
#line 1640 "./grammar.tab.c"
	break;
      case 89: /* "memberDef" */
#line 147 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting declaration ...\n"); };
#line 1645 "./grammar.tab.c"
	break;
      case 90: /* "classDef" */
#line 147 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting declaration ...\n"); };
#line 1650 "./grammar.tab.c"
	break;
      case 91: /* "ifStatement" */
#line 149 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting for/if/while...\n"); };
#line 1655 "./grammar.tab.c"
	break;
      case 92: /* "cond" */
#line 150 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting cond/return ...\n"); };
#line 1660 "./grammar.tab.c"
	break;
      case 93: /* "forStatement" */
#line 149 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting for/if/while...\n"); };
#line 1665 "./grammar.tab.c"
	break;
      case 94: /* "forCond" */
#line 150 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting cond/return ...\n"); };
#line 1670 "./grammar.tab.c"
	break;
      case 95: /* "whileStatement" */
#line 149 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting for/if/while...\n"); };
#line 1675 "./grammar.tab.c"
	break;
      case 96: /* "nextStatement" */
#line 151 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting next/break  ...\n"); };
#line 1680 "./grammar.tab.c"
	break;
      case 97: /* "breakStatement" */
#line 151 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting next/break  ...\n"); };
#line 1685 "./grammar.tab.c"
	break;
      case 98: /* "returnStatement" */
#line 150 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting cond/return ...\n"); };
#line 1690 "./grammar.tab.c"
	break;
      case 99: /* "identifier" */
#line 143 "./grammar.y"
	{ delete ((yyvaluep->idString)); PRINTF("Deleting identifier  ...\n"); };
#line 1695 "./grammar.tab.c"
	break;
      case 100: /* "vector" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1700 "./grammar.tab.c"
	break;
      case 101: /* "vectorList" */
#line 141 "./grammar.y"
	{ delete ((yyvaluep->argumentList)); PRINTF("Deleting argument list\n"); };
#line 1705 "./grammar.tab.c"
	break;
      case 102: /* "constant" */
#line 144 "./grammar.y"
	{ delete ((yyvaluep->syntaxElement)); PRINTF("Deleting variable    ...\n"); };
#line 1710 "./grammar.tab.c"
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
#line 219 "./grammar.y"
    {
                    PRINTF("Bison encountered end_of_input; ignored\n");
                    return 0;
                }
    break;

  case 3:
#line 224 "./grammar.y"
    {
                    PRINTF("Bison encountered newline; ignored\n");
                    return 0;
                }
    break;

  case 4:
#line 229 "./grammar.y"
    {
                    PRINTF("Bison encountered comment; ignored\n");
                    return 0;
                }
    break;

  case 5:
#line 234 "./grammar.y"
    {
                    PRINTF("Bison trying to execute statement or expression\n");
                    int rv = parser.execute(*(yyvsp[(1) - (2)].syntaxElement));
                    delete (yyvsp[(1) - (2)].syntaxElement);
                    return rv;
                }
    break;

  case 6:
#line 241 "./grammar.y"
    {
                    PRINTF("Bison trying to execute statement or expression\n");
                    int rv =  parser.execute(*(yyvsp[(1) - (2)].syntaxElement));
                    delete (yyvsp[(1) - (2)].syntaxElement);
                    return rv;
                }
    break;

  case 7:
#line 248 "./grammar.y"
    {
                    PRINTF("Bison trying to execute declaration\n");
                    int rv =  parser.execute(*(yyvsp[(1) - (2)].syntaxElement));
                    delete (yyvsp[(1) - (2)].syntaxElement);
                    return rv;
                }
    break;

  case 8:
#line 255 "./grammar.y"
    {
                    PRINTF("Bison trying to execute declaration\n");
                    int rv =  parser.execute(*(yyvsp[(1) - (2)].syntaxElement));
                    delete (yyvsp[(1) - (2)].syntaxElement);
                    return rv;
                }
    break;

  case 9:
#line 262 "./grammar.y"
    {
                    PRINTF("Bison trying to get help for symbol\n");
                    int rv =  parser.help(*(yyvsp[(2) - (3)].idString));
                    delete (yyvsp[(2) - (3)].idString);
                    return rv;
                }
    break;

  case 10:
#line 269 "./grammar.y"
    {
                    PRINTF("Bison trying to get help for symbol\n");
                    int rv =  parser.help(*(yyvsp[(2) - (3)].idString));
                    delete (yyvsp[(2) - (3)].idString);
                    return rv;
                }
    break;

  case 11:
#line 276 "./grammar.y"
    {
                    PRINTF("Bison trying to get help for function call\n");
                    int rv =  parser.help(*(yyvsp[(2) - (3)].syntaxFunctionCall));
                    delete (yyvsp[(2) - (3)].syntaxFunctionCall);
                    return rv;
                }
    break;

  case 12:
#line 283 "./grammar.y"
    {
                    PRINTF("Bison trying to get help for function call\n");
                    int rv =  parser.help(*(yyvsp[(2) - (3)].syntaxFunctionCall));
                    delete (yyvsp[(2) - (3)].syntaxFunctionCall);
                    return rv;
                }
    break;

  case 13:
#line 290 "./grammar.y"
    {
                    PRINTF("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
                    YYABORT;
                }
    break;

  case 14:
#line 295 "./grammar.y"
    {
                    PRINTF("Bison error when reading line %d position %d to line %d position %d\n", (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
                    YYABORT;
                }
    break;

  case 15:
#line 301 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 16:
#line 303 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxFunctionCall(RbPtr<RbString>( new RbString("v") ), *(yyvsp[(1) - (1)].argumentList)) ); delete (yyvsp[(1) - (1)].argumentList); }
    break;

  case 17:
#line 305 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 18:
#line 307 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxUnaryExpr(SyntaxUnaryExpr::UMinus, *(yyvsp[(2) - (2)].syntaxElement)) ); delete (yyvsp[(2) - (2)].syntaxElement); }
    break;

  case 19:
#line 308 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxUnaryExpr(SyntaxUnaryExpr::UPlus, *(yyvsp[(2) - (2)].syntaxElement)) ); delete (yyvsp[(2) - (2)].syntaxElement); }
    break;

  case 20:
#line 309 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxUnaryExpr(SyntaxUnaryExpr::UNot, *(yyvsp[(2) - (2)].syntaxElement)) ); delete (yyvsp[(2) - (2)].syntaxElement); }
    break;

  case 21:
#line 310 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxUnaryExpr(SyntaxUnaryExpr::UAnd, *(yyvsp[(2) - (2)].syntaxElement)) ); delete (yyvsp[(2) - (2)].syntaxElement); }
    break;

  case 22:
#line 312 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Range, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 23:
#line 314 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Add, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 24:
#line 315 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Sub, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 25:
#line 316 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Mul, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 26:
#line 317 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Div, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 27:
#line 318 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Exp, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 28:
#line 320 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Lt, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 29:
#line 321 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Le, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 30:
#line 322 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Eq, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 31:
#line 323 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Ne, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 32:
#line 324 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Ge, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 33:
#line 325 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Gt, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 34:
#line 327 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::And, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 35:
#line 328 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Or, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 36:
#line 329 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::And2, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 37:
#line 330 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Or2, *(yyvsp[(1) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElement)) ); delete (yyvsp[(1) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 38:
#line 332 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 39:
#line 333 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 40:
#line 334 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 41:
#line 335 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 42:
#line 337 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>((SyntaxFunctionCall*)(*(yyvsp[(1) - (1)].syntaxFunctionCall))); delete (yyvsp[(1) - (1)].syntaxFunctionCall); }
    break;

  case 43:
#line 339 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>((SyntaxVariable*)(*(yyvsp[(1) - (1)].syntaxVariable))); delete (yyvsp[(1) - (1)].syntaxVariable); }
    break;

  case 44:
#line 343 "./grammar.y"
    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::ArrowAssign, *(yyvsp[(1) - (3)].syntaxVariable), *(yyvsp[(3) - (3)].syntaxElement)) );
                        delete (yyvsp[(1) - (3)].syntaxVariable); delete (yyvsp[(3) - (3)].syntaxElement);
                    }
    break;

  case 45:
#line 349 "./grammar.y"
    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::ArrowAssign, *(yyvsp[(1) - (3)].syntaxFunctionCall), *(yyvsp[(3) - (3)].syntaxElement)) );
                        delete (yyvsp[(1) - (3)].syntaxFunctionCall); delete (yyvsp[(3) - (3)].syntaxElement);
                    }
    break;

  case 46:
#line 357 "./grammar.y"
    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::TildeAssign, *(yyvsp[(1) - (3)].syntaxVariable), *(yyvsp[(3) - (3)].syntaxFunctionCall)) );
                        delete (yyvsp[(1) - (3)].syntaxVariable); delete (yyvsp[(3) - (3)].syntaxFunctionCall);
                    }
    break;

  case 47:
#line 363 "./grammar.y"
    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::TildeAssign, *(yyvsp[(1) - (3)].syntaxFunctionCall), *(yyvsp[(3) - (3)].syntaxFunctionCall)) );
                        delete (yyvsp[(1) - (3)].syntaxFunctionCall); delete (yyvsp[(3) - (3)].syntaxFunctionCall);
                    }
    break;

  case 48:
#line 371 "./grammar.y"
    {
                        PRINTF("Parser inserting tilde iid assignment (TILDEIID_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::TildeIidAssign, *(yyvsp[(1) - (3)].syntaxVariable), *(yyvsp[(3) - (3)].syntaxFunctionCall)) );
                        delete (yyvsp[(1) - (3)].syntaxVariable); delete (yyvsp[(3) - (3)].syntaxFunctionCall);
                    }
    break;

  case 49:
#line 377 "./grammar.y"
    {
                        PRINTF("Parser inserting tilde iid assignment (TILDEIID_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::TildeIidAssign, *(yyvsp[(1) - (3)].syntaxFunctionCall), *(yyvsp[(3) - (3)].syntaxFunctionCall)) );
                        delete (yyvsp[(1) - (3)].syntaxFunctionCall); delete (yyvsp[(3) - (3)].syntaxFunctionCall);
                    }
    break;

  case 50:
#line 385 "./grammar.y"
    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::EquationAssign, *(yyvsp[(1) - (3)].syntaxVariable), *(yyvsp[(3) - (3)].syntaxElement)) ); 
                        delete (yyvsp[(1) - (3)].syntaxVariable); delete (yyvsp[(3) - (3)].syntaxElement);
                    }
    break;

  case 51:
#line 391 "./grammar.y"
    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::EquationAssign, *(yyvsp[(1) - (3)].syntaxFunctionCall), *(yyvsp[(3) - (3)].syntaxElement)) ); 
                        delete (yyvsp[(1) - (3)].syntaxFunctionCall); delete (yyvsp[(3) - (3)].syntaxElement);
                    }
    break;

  case 52:
#line 399 "./grammar.y"
    {
                    PRINTF("Parser inserting variable (NAMED_VAR)in syntax tree\n");
                    (yyval.syntaxVariable) = new RbPtr<SyntaxVariable>( new SyntaxVariable(*(yyvsp[(1) - (2)].idString), *(yyvsp[(2) - (2)].syntaxElementList)) );
                    delete (yyvsp[(1) - (2)].idString); delete (yyvsp[(2) - (2)].syntaxElementList);
                }
    break;

  case 53:
#line 405 "./grammar.y"
    {
                    PRINTF("Parser inserting variable (FUNCTION_VAR) in syntax tree\n");
                    (*(yyvsp[(5) - (5)].syntaxElementList))->push_front(*(yyvsp[(3) - (5)].syntaxElement));
                    (yyval.syntaxVariable) = new RbPtr<SyntaxVariable>( new SyntaxVariable(*(yyvsp[(1) - (5)].syntaxFunctionCall), *(yyvsp[(5) - (5)].syntaxElementList)) );
                    delete (yyvsp[(1) - (5)].syntaxFunctionCall); delete (yyvsp[(3) - (5)].syntaxElement); delete (yyvsp[(5) - (5)].syntaxElementList);
                }
    break;

  case 54:
#line 412 "./grammar.y"
    {
                    PRINTF("Parser inserting member variable (NAMED_VAR)in syntax tree\n");
                    (yyval.syntaxVariable) = new RbPtr<SyntaxVariable>( new SyntaxVariable(*(yyvsp[(1) - (4)].syntaxVariable), *(yyvsp[(3) - (4)].idString), *(yyvsp[(4) - (4)].syntaxElementList)) );
                    delete (yyvsp[(1) - (4)].syntaxVariable); delete (yyvsp[(3) - (4)].idString); delete (yyvsp[(4) - (4)].syntaxElementList);
                }
    break;

  case 55:
#line 418 "./grammar.y"
    {
                    PRINTF("Parser inserting member variable (FUNCTION_VAR) in syntax tree\n");
                    (*(yyvsp[(7) - (7)].syntaxElementList))->push_front(*(yyvsp[(5) - (7)].syntaxElement));
                    (yyval.syntaxVariable) = new RbPtr<SyntaxVariable>( new SyntaxVariable(*(yyvsp[(1) - (7)].syntaxVariable), *(yyvsp[(3) - (7)].syntaxFunctionCall), *(yyvsp[(7) - (7)].syntaxElementList)) );
                    delete (yyvsp[(1) - (7)].syntaxVariable); delete (yyvsp[(3) - (7)].syntaxFunctionCall); delete (yyvsp[(5) - (7)].syntaxElement); delete (yyvsp[(7) - (7)].syntaxElementList);
                }
    break;

  case 56:
#line 426 "./grammar.y"
    { (yyval.syntaxElementList) = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >() ); }
    break;

  case 57:
#line 427 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (1)].syntaxElementList); }
    break;

  case 58:
#line 430 "./grammar.y"
    { (yyval.syntaxElementList) = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >(1, *(yyvsp[(2) - (3)].syntaxElement)) ); delete (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 59:
#line 431 "./grammar.y"
    { (yyval.syntaxElementList) = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >(1, RbPtr<SyntaxElement>( NULL ) ) ); }
    break;

  case 60:
#line 432 "./grammar.y"
    { (*(yyvsp[(1) - (4)].syntaxElementList))->push_back(*(yyvsp[(3) - (4)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (4)].syntaxElementList); delete (yyvsp[(3) - (4)].syntaxElement); }
    break;

  case 61:
#line 433 "./grammar.y"
    { (*(yyvsp[(1) - (3)].syntaxElementList))->push_back(RbPtr<SyntaxElement>( NULL ) ); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); }
    break;

  case 62:
#line 437 "./grammar.y"
    {
                    (yyval.syntaxFunctionCall) = new RbPtr<SyntaxFunctionCall>( new SyntaxFunctionCall(*(yyvsp[(1) - (4)].idString), *(yyvsp[(3) - (4)].argumentList)) );
                    delete (yyvsp[(1) - (4)].idString); delete (yyvsp[(3) - (4)].argumentList);
                }
    break;

  case 63:
#line 444 "./grammar.y"
    {
                        PRINTF("Parser inserting function call in syntax tree\n");
                        (yyval.syntaxFunctionCall) = (yyvsp[(1) - (1)].syntaxFunctionCall);
                    }
    break;

  case 64:
#line 449 "./grammar.y"
    {
                        PRINTF("Parser inserting member call in syntax tree\n");
                        (*(yyvsp[(3) - (3)].syntaxFunctionCall))->setBaseVariable(*(yyvsp[(1) - (3)].syntaxVariable));
                        (yyval.syntaxFunctionCall) = (yyvsp[(3) - (3)].syntaxFunctionCall);
                        delete (yyvsp[(1) - (3)].syntaxVariable);
                    }
    break;

  case 65:
#line 457 "./grammar.y"
    { (yyval.argumentList) = new RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > >( new std::list<RbPtr<SyntaxLabeledExpr> >() ); }
    break;

  case 66:
#line 458 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(1) - (1)].argumentList); }
    break;

  case 67:
#line 461 "./grammar.y"
    { (yyval.argumentList) = new RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > >( new std::list<RbPtr<SyntaxLabeledExpr> >(1,*(yyvsp[(1) - (1)].syntaxLabeledExpr)) ); delete (yyvsp[(1) - (1)].syntaxLabeledExpr); }
    break;

  case 68:
#line 462 "./grammar.y"
    { (*(yyvsp[(1) - (3)].argumentList))->push_back(*(yyvsp[(3) - (3)].syntaxLabeledExpr)); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); delete (yyvsp[(3) - (3)].syntaxLabeledExpr);}
    break;

  case 69:
#line 466 "./grammar.y"
    {
                    PRINTF("Parser inserting unlabeled argument in syntax tree\n");
                    (yyval.syntaxLabeledExpr) = new RbPtr<SyntaxLabeledExpr>( new SyntaxLabeledExpr(RbPtr<RbString>( new RbString("") ), *(yyvsp[(1) - (1)].syntaxElement)) );
                    delete (yyvsp[(1) - (1)].syntaxElement);
                }
    break;

  case 70:
#line 472 "./grammar.y"
    { 
                    PRINTF("Parser inserting labeled argument in syntax tree\n");
                    (yyval.syntaxLabeledExpr) = new RbPtr<SyntaxLabeledExpr>( new SyntaxLabeledExpr(*(yyvsp[(1) - (3)].idString), *(yyvsp[(3) - (3)].syntaxElement)) );
                    delete (yyvsp[(1) - (3)].idString); delete (yyvsp[(3) - (3)].syntaxElement);
                }
    break;

  case 71:
#line 480 "./grammar.y"
    {
                    PRINTF("Parser inserting function definition in syntax tree\n");
                    (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxFunctionDef(RbPtr<RbString>( NULL ), *(yyvsp[(2) - (6)].idString), *(yyvsp[(4) - (6)].formalList), *(yyvsp[(6) - (6)].syntaxElementList)) );
                    delete (yyvsp[(2) - (6)].idString); delete (yyvsp[(4) - (6)].formalList); delete (yyvsp[(6) - (6)].syntaxElementList);
                }
    break;

  case 72:
#line 487 "./grammar.y"
    {
                    PRINTF("Parser inserting typed function definition in syntax tree\n");
                    (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxFunctionDef(*(yyvsp[(2) - (7)].idString), *(yyvsp[(3) - (7)].idString), *(yyvsp[(5) - (7)].formalList), *(yyvsp[(7) - (7)].syntaxElementList)) );
                    delete (yyvsp[(2) - (7)].idString); delete (yyvsp[(3) - (7)].idString); delete (yyvsp[(5) - (7)].formalList); delete (yyvsp[(7) - (7)].syntaxElementList);
                }
    break;

  case 73:
#line 494 "./grammar.y"
    { (yyval.formalList) = new RbPtr<std::list<RbPtr<SyntaxFormal> > >( new std::list<RbPtr<SyntaxFormal> >() ); }
    break;

  case 74:
#line 495 "./grammar.y"
    { (yyval.formalList) = (yyvsp[(1) - (1)].formalList); }
    break;

  case 75:
#line 498 "./grammar.y"
    { (yyval.formalList) = new RbPtr<std::list<RbPtr<SyntaxFormal> > >( new std::list<RbPtr<SyntaxFormal> >(1, *(yyvsp[(1) - (1)].syntaxFormal)) ); delete (yyvsp[(1) - (1)].syntaxFormal); }
    break;

  case 76:
#line 499 "./grammar.y"
    { (*(yyvsp[(1) - (3)].formalList))->push_back(*(yyvsp[(3) - (3)].syntaxFormal)); (yyval.formalList) = (yyvsp[(1) - (3)].formalList); delete (yyvsp[(3) - (3)].syntaxFormal);}
    break;

  case 77:
#line 503 "./grammar.y"
    {
                    PRINTF("Inserting labeled formal argument without default in syntax tree\n");
                    (yyval.syntaxFormal) = new RbPtr<SyntaxFormal>( new SyntaxFormal(*(yyvsp[(1) - (1)].idString), RbPtr<SyntaxElement>( NULL ) ) );
                    delete (yyvsp[(1) - (1)].idString);
                }
    break;

  case 78:
#line 509 "./grammar.y"
    {
                    PRINTF("Inserting labeled formal argument with default in syntax tree\n");
                    (yyval.syntaxFormal) = new RbPtr<SyntaxFormal>( new SyntaxFormal(*(yyvsp[(1) - (3)].idString), *(yyvsp[(3) - (3)].syntaxElement)) );
                    delete (yyvsp[(1) - (3)].idString); delete (yyvsp[(3) - (3)].syntaxElement);
                }
    break;

  case 79:
#line 515 "./grammar.y"
    {
                    PRINTF("Inserting typed labeled formal argument without default in syntax tree\n");
                    (yyval.syntaxFormal) = new RbPtr<SyntaxFormal>( new SyntaxFormal(*(yyvsp[(1) - (2)].idString), *(yyvsp[(2) - (2)].idString), RbPtr<SyntaxElement>(NULL) ) );
                    delete (yyvsp[(1) - (2)].idString); delete (yyvsp[(2) - (2)].idString);
                }
    break;

  case 80:
#line 521 "./grammar.y"
    {
                    PRINTF("Inserting typed labeled formal argument with default in syntax tree\n");
                    (yyval.syntaxFormal) = new RbPtr<SyntaxFormal>( new SyntaxFormal(*(yyvsp[(1) - (4)].idString), *(yyvsp[(2) - (4)].idString), *(yyvsp[(4) - (4)].syntaxElement)) );
                    delete (yyvsp[(1) - (4)].idString); delete (yyvsp[(2) - (4)].idString); delete (yyvsp[(4) - (4)].syntaxElement);
                }
    break;

  case 81:
#line 528 "./grammar.y"
    { (*(yyvsp[(1) - (3)].idString))->append(*(*((yyvsp[(2) - (3)].idString)))); (*(yyvsp[(1) - (3)].idString))->append(*(*((yyvsp[(3) - (3)].idString)))); (yyval.idString) = (yyvsp[(1) - (3)].idString); delete (yyvsp[(2) - (3)].idString); delete (yyvsp[(3) - (3)].idString); }
    break;

  case 82:
#line 531 "./grammar.y"
    { (yyval.idString) = new RbPtr<RbString>( new RbString() ); }
    break;

  case 83:
#line 532 "./grammar.y"
    { (yyval.idString) = (yyvsp[(1) - (1)].idString); }
    break;

  case 84:
#line 535 "./grammar.y"
    { (yyval.idString) = new RbPtr<RbString>( new RbString("[]") ); }
    break;

  case 85:
#line 536 "./grammar.y"
    { (yyval.idString) = new RbPtr<RbString>( new RbString("[") ); (*(yyval.idString))->append(INT); (*(yyval.idString))->append("]"); }
    break;

  case 86:
#line 537 "./grammar.y"
    { (*(yyvsp[(1) - (3)].idString))->append("[]"); (yyval.idString) = (yyvsp[(1) - (3)].idString); }
    break;

  case 87:
#line 538 "./grammar.y"
    { (*(yyvsp[(1) - (4)].idString))->append("["); (*(yyvsp[(1) - (4)].idString))->append(INT); (*(yyvsp[(1) - (4)].idString))->append("]"); (yyval.idString) = (yyvsp[(1) - (4)].idString); }
    break;

  case 88:
#line 541 "./grammar.y"
    { (yyval.idString) = new RbPtr<RbString>( new RbString() ); }
    break;

  case 89:
#line 542 "./grammar.y"
    { (yyval.idString) = new RbPtr<RbString>( new RbString("&") ); }
    break;

  case 90:
#line 545 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(2) - (3)].syntaxElementList); }
    break;

  case 91:
#line 547 "./grammar.y"
    {
                    RbPtr<std::list<RbPtr<SyntaxElement> > >* stmts = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >() );
                    (*stmts)->push_back(*(yyvsp[(1) - (1)].syntaxElement));
                    (yyval.syntaxElementList) = stmts;
                    delete (yyvsp[(1) - (1)].syntaxElement);
                }
    break;

  case 92:
#line 555 "./grammar.y"
    { (yyval.syntaxElementList) = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >() ); }
    break;

  case 93:
#line 556 "./grammar.y"
    { (yyval.syntaxElementList) = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >(1, *(yyvsp[(1) - (1)].syntaxElement)) ); delete (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 94:
#line 557 "./grammar.y"
    { (*(yyvsp[(1) - (3)].syntaxElementList))->push_back(*(yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 95:
#line 558 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 96:
#line 559 "./grammar.y"
    { (*(yyvsp[(1) - (3)].syntaxElementList))->push_back(*(yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 97:
#line 560 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 98:
#line 563 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 99:
#line 564 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 100:
#line 565 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 101:
#line 566 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 102:
#line 567 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 103:
#line 568 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 104:
#line 571 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 105:
#line 572 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 106:
#line 575 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 107:
#line 576 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 108:
#line 578 "./grammar.y"
    {
                        PRINTF("Parser inserting variable declaration in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxVariableDecl(*(yyvsp[(1) - (4)].idString), *(yyvsp[(2) - (4)].syntaxElementList), *(yyvsp[(3) - (4)].idString), *(yyvsp[(4) - (4)].idString)) );
                        delete (yyvsp[(1) - (4)].idString); delete (yyvsp[(2) - (4)].syntaxElementList); delete (yyvsp[(3) - (4)].idString); delete (yyvsp[(4) - (4)].idString);
                    }
    break;

  case 109:
#line 585 "./grammar.y"
    { (yyval.syntaxElementList) = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >() ); }
    break;

  case 110:
#line 586 "./grammar.y"
    { (yyval.syntaxElementList) = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >(1, *(yyvsp[(1) - (1)].syntaxElement)) ); delete (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 111:
#line 587 "./grammar.y"
    { (*(yyvsp[(1) - (3)].syntaxElementList))->push_back(*(yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 112:
#line 588 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 113:
#line 589 "./grammar.y"
    { (*(yyvsp[(1) - (3)].syntaxElementList))->push_back(*(yyvsp[(3) - (3)].syntaxElement)); (yyval.syntaxElementList) = (yyvsp[(1) - (3)].syntaxElementList); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 114:
#line 590 "./grammar.y"
    { (yyval.syntaxElementList) = (yyvsp[(1) - (2)].syntaxElementList); }
    break;

  case 115:
#line 593 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( (SyntaxFormal*)(yyvsp[(1) - (1)].syntaxFormal) ); }
    break;

  case 116:
#line 594 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 117:
#line 598 "./grammar.y"
    {
                PRINTF("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
                    (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxClassDef(*(yyvsp[(2) - (7)].idString), *(yyvsp[(4) - (7)].idString), *(yyvsp[(6) - (7)].syntaxElementList)) ); delete (yyvsp[(2) - (7)].idString); delete (yyvsp[(4) - (7)].idString); delete(yyvsp[(6) - (7)].syntaxElementList);
                }
    break;

  case 118:
#line 604 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::If, *(yyvsp[(2) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElementList)) ); delete (yyvsp[(2) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElementList); }
    break;

  case 119:
#line 605 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::IfElse, *(yyvsp[(2) - (5)].syntaxElement), *(yyvsp[(3) - (5)].syntaxElementList), *(yyvsp[(5) - (5)].syntaxElementList)) ); delete (yyvsp[(2) - (5)].syntaxElement); delete (yyvsp[(3) - (5)].syntaxElementList); delete (yyvsp[(5) - (5)].syntaxElementList); }
    break;

  case 120:
#line 607 "./grammar.y"
    { (yyval.syntaxElement) = (yyvsp[(2) - (3)].syntaxElement); }
    break;

  case 121:
#line 610 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::For, *(yyvsp[(2) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElementList)) ); delete (yyvsp[(2) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElementList); }
    break;

  case 122:
#line 613 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxForCondition(*(yyvsp[(2) - (5)].idString), *(yyvsp[(4) - (5)].syntaxElement)) ); delete (yyvsp[(2) - (5)].idString); delete (yyvsp[(4) - (5)].syntaxElement); }
    break;

  case 123:
#line 616 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::While, *(yyvsp[(2) - (3)].syntaxElement), *(yyvsp[(3) - (3)].syntaxElementList)) ); delete (yyvsp[(2) - (3)].syntaxElement); delete (yyvsp[(3) - (3)].syntaxElementList); }
    break;

  case 124:
#line 619 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::Next) ); }
    break;

  case 125:
#line 622 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::Break) ); }
    break;

  case 126:
#line 625 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::Return) ); }
    break;

  case 127:
#line 626 "./grammar.y"
    { (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::Return, *(yyvsp[(2) - (2)].syntaxElement)) ); delete (yyvsp[(2) - (2)].syntaxElement); }
    break;

  case 128:
#line 629 "./grammar.y"
    { (yyval.idString) = new RbPtr<RbString>( new RbString((yyvsp[(1) - (1)].string)) ); }
    break;

  case 129:
#line 633 "./grammar.y"
    { (yyval.argumentList) = (yyvsp[(2) - (3)].argumentList); }
    break;

  case 130:
#line 636 "./grammar.y"
    { (*(yyvsp[(1) - (3)].argumentList))->push_back(RbPtr<SyntaxLabeledExpr>( new SyntaxLabeledExpr( RbPtr<RbString>( new RbString("") ), *(yyvsp[(3) - (3)].syntaxElement)) ) ); (yyval.argumentList) = (yyvsp[(1) - (3)].argumentList); delete (yyvsp[(3) - (3)].syntaxElement); }
    break;

  case 131:
#line 637 "./grammar.y"
    { (yyval.argumentList) = new RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > >( new std::list<RbPtr<SyntaxLabeledExpr> >(1, RbPtr<SyntaxLabeledExpr>( new SyntaxLabeledExpr(RbPtr<RbString>( new RbString("") ), *(yyvsp[(1) - (1)].syntaxElement)) ) ) ); delete (yyvsp[(1) - (1)].syntaxElement); }
    break;

  case 132:
#line 641 "./grammar.y"
    {
                    PRINTF("Parser inserting bool constant (false) in syntax tree\n");
                    (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new RbBoolean(false) ) ) );
                }
    break;

  case 133:
#line 646 "./grammar.y"
    {
                    PRINTF("Parser inserting bool constant (true) in syntax tree\n");
                    (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new RbBoolean(true) ) ) );
                }
    break;

  case 134:
#line 651 "./grammar.y"
    {
                    PRINTF("Parser inserting null constant in syntax tree\n");
                    (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( NULL ) ) );
                }
    break;

  case 135:
#line 656 "./grammar.y"
    {
                    if ( (yyvsp[(1) - (1)].intValue) < 0 ) {
                        PRINTF("Parser inserting Integer constant in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new Integer((yyvsp[(1) - (1)].intValue)) ) ) );
                    }
                    else { 
                        PRINTF("Parser inserting Natural constant in syntax tree\n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new Natural((yyvsp[(1) - (1)].intValue)) ) ) );
                    }
                }
    break;

  case 136:
#line 667 "./grammar.y"
    {
                    PRINTF("Parser inserting String constant in syntax tree\n");
                    (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new RbString((yyvsp[(1) - (1)].string)) ) ) );
                }
    break;

  case 137:
#line 672 "./grammar.y"
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
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new RealPos((yyvsp[(1) - (1)].realValue)) ) ) );
                    }
                    else {
                        PRINTF("Parser inserting Real constant in syntax tree \n");
                        (yyval.syntaxElement) = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new Real((yyvsp[(1) - (1)].realValue)) ) ) );
                    }
                }
    break;


/* Line 1267 of yacc.c.  */
#line 2941 "./grammar.tab.c"
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


#line 721 "./grammar.y"



/* Definition of yyerror. */
int yyerror(const char *msg)
{
    PRINTF("Bison code said: %s\n", msg);
    if ( foundNewline == true )
        foundErrorBeforeEnd = false;
    else
        foundErrorBeforeEnd = true;

    return 1;
}



