%{
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

%}

/* Bison needs a union of the types handled by the parser */
%union {
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
};

/* Return types of the elements handled by the parser */
%type <string> NAME STRING
%type <realValue> REAL
%type <intValue> INT RBNULL
%type <boolValue> FALSE TRUE
%type <idString> identifier typeSpec optDims dimList optRef
%type <syntaxVariable> variable
%type <syntaxFunctionCall> functionCall fxnCall
%type <syntaxLabeledExpr> argument
%type <syntaxFormal> formal
%type <syntaxElement> constant
%type <syntaxElement> statement expression stmt_or_expr
%type <syntaxElement> arrowAssign tildeAssign tildeIidAssign equationAssign
%type <syntaxElement> declaration classDef memberDef
%type <syntaxElement> functionDef
%type <syntaxElement> forStatement ifStatement whileStatement
%type <syntaxElement> forCond cond returnStatement
%type <syntaxElement> nextStatement breakStatement
%type <syntaxElementList> elementList optElements
%type <syntaxElementList> stmts stmtList
%type <syntaxElementList> memberDefs
%type <argumentList> argumentList optArguments vectorList vector
%type <formalList> formalList optFormals

/* Tokens returned by the lexer and handled by the parser */
%token REAL INT NAME STRING RBNULL FALSE TRUE COMMENT
%token FUNCTION CLASS FOR IN IF ELSE WHILE NEXT BREAK RETURN
%token ARROW_ASSIGN TILDE_ASSIGN TILDEIID_ASSIGN EQUATION_ASSIGN EQUAL 
%token AND OR AND2 OR2 GT GE LT LE EQ NE
%token END_OF_INPUT

/* Destructors */
%destructor { delete ($$); PRINTF("Deleting element list\n"); } elementList optElements stmts stmtList memberDefs 
%destructor { delete ($$); PRINTF("Deleting argument list\n"); } argumentList optArguments vectorList vector
%destructor { delete ($$); PRINTF("Deleting formal list\n"); } formalList optFormals
%destructor { delete ($$); PRINTF("Deleting identifier  ...\n"); } identifier typeSpec optDims dimList optRef
%destructor { delete ($$); PRINTF("Deleting variable    ...\n"); } variable functionCall fxnCall argument formal constant
%destructor { delete ($$); PRINTF("Deleting expression  ...\n"); } statement expression stmt_or_expr 
%destructor { delete ($$); PRINTF("Deleting assignment  ...\n"); } arrowAssign tildeAssign tildeIidAssign equationAssign 
%destructor { delete ($$); PRINTF("Deleting declaration ...\n"); } declaration classDef memberDef 
%destructor { delete ($$); PRINTF("Deleting functiondef ...\n"); } functionDef 
%destructor { delete ($$); PRINTF("Deleting for/if/while...\n"); } forStatement ifStatement whileStatement 
%destructor { delete ($$); PRINTF("Deleting cond/return ...\n"); } forCond cond returnStatement
%destructor { delete ($$); PRINTF("Deleting next/break  ...\n"); } nextStatement breakStatement

/* Use location tracking */
%locations

/*
 * Precedence table, low to high, with order of evaluation.
 */
%left       '?'
%left       WHILE FOR
%right      IF
%left       ELSE
%right      ARROW_ASSIGN
%right      TILDE_ASSIGN TILDEIID_ASSIGN
%right      EQUATION_ASSIGN
%right      EQUAL
%left       OR OR2
%left       AND AND2
%left       UNOT
%nonassoc   GT GE LT LE EQ NE
%left       '+' '-'
%left       '*' '/'
%left       ':'
%left       UMINUS UPLUS
%right      '^'
%left       '.' UAND
%nonassoc   '(' '['

%%

/*
 * The grammar is based both on R and on more modern object-
 * oriented programming languages like C++ and Java. Like R,
 * it is a language intended for interactive use. It does
 * not support implicit looping or vector indexing like R
 * does. This means that explicit loops will have to be used
 * in more situations than in R. On the other hand, this
 * makes the language less ambiguous and more intuitive for
 * users familiar with Java or C++. All language objects
 * inherit from "object". RevBayes allows types of variables
 * and function arguments, as well as return types of func-
 * tions, to be specified. However, types default to "object",
 * allowing apparently untyped expressions to be evaluated.
 * Global variables cannot be declared. They are created by
 * assigning to them.
 *
 * RevBayes allows a natural description of graphical represen-
 * tations of complex probability models. The most essential
 * components are tilde assignment ('~') for creating stochastic
 * nodes and equation assignment (':=') for creating deterministic
 * nodes in model DAGs. Constant nodes are created using
 * arrow assignment ('<-') as in R.
 *
 * The Rev language passes by value except in equation and tilde
 * assignment, in which case variables are passed by reference.
 * 
 * Unlike R, right assignment ('->') is not supported. Also,
 * a number of constructs that are interpreted as expressions
 * in R are treated as statements or declarations in RevBayes
 * and have no return value (or the return value is discarded).
 * These constructs include control statements and function def-
 * initions, among others.
 *
 * For more discussion of the language, see the RevBayes docu-
 * mentation.
 */

prog    :       END_OF_INPUT
                {
                    PRINTF("Bison encountered end_of_input; ignored\n");
                    return 0;
                }
        |       '\n'
                {
                    PRINTF("Bison encountered newline; ignored\n");
                    return 0;
                }
        |       COMMENT '\n'
                {
                    PRINTF("Bison encountered comment; ignored\n");
                    return 0;
                }
        |       stmt_or_expr '\n'
                {
                    PRINTF("Bison trying to execute statement or expression\n");
                    return parser.execute(*$1);
                }
        |       stmt_or_expr ';'
                {
                    PRINTF("Bison trying to execute statement or expression\n");
                    return parser.execute(*$1);
                }
        |       declaration '\n'
                {
                    PRINTF("Bison trying to execute declaration\n");
                    return parser.execute(*$1);
                }
        |       declaration ';'
                {
                    PRINTF("Bison trying to execute declaration\n");
                    return parser.execute(*$1);
                }
        |       '?' identifier '\n'
                {
                    PRINTF("Bison trying to get help for symbol\n");
                    return parser.help(*$2);
                }
        |       '?' identifier ';'
                {
                    PRINTF("Bison trying to get help for symbol\n");
                    return parser.help(*$2);
                }
        |       '?' functionCall '\n'
                {
                    PRINTF("Bison trying to get help for function call\n");
                    return parser.help(*$2);
                }
        |       '?' functionCall ';'
                {
                    PRINTF("Bison trying to get help for function call\n");
                    return parser.help(*$2);
                }
        |       error '\n'
                {
                    PRINTF("Bison error when reading line %d position %d to line %d position %d\n", @$.first_line, @$.first_column, @$.last_line, @$.last_column);
                    YYABORT;
                }
        |       error ';'
                {
                    PRINTF("Bison error when reading line %d position %d to line %d position %d\n", @$.first_line, @$.first_column, @$.last_line, @$.last_column);
                    YYABORT;
                }
        ;

expression  :   constant                    { $$ = $1; }

            |   vector                      { $$ = new RbPtr<SyntaxElement>( new SyntaxFunctionCall(RbPtr<RbString>( new RbString("v") ), *$1) ); }

            |   '(' expression ')'          { $$ = $2; }

            |   '-' expression %prec UMINUS { $$ = new RbPtr<SyntaxElement>( new SyntaxUnaryExpr(SyntaxUnaryExpr::UMinus, *$2) ); }
            |   '+' expression %prec UPLUS  { $$ = new RbPtr<SyntaxElement>( new SyntaxUnaryExpr(SyntaxUnaryExpr::UPlus, *$2) ); }
            |   '!' expression %prec UNOT   { $$ = new RbPtr<SyntaxElement>( new SyntaxUnaryExpr(SyntaxUnaryExpr::UNot, *$2) ); }
            |   AND expression %prec UAND   { $$ = new RbPtr<SyntaxElement>( new SyntaxUnaryExpr(SyntaxUnaryExpr::UAnd, *$2) ); }

            |   expression ':' expression   { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Range, *$1, *$3) ); }

            |   expression '+' expression   { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Add, *$1, *$3) ); }
            |   expression '-' expression   { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Sub, *$1, *$3) ); }
            |   expression '*' expression   { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Mul, *$1, *$3) ); }
            |   expression '/' expression   { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Div, *$1, *$3) ); }
            |   expression '^' expression   { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Exp, *$1, *$3) ); }

            |   expression LT expression    { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Lt, *$1, *$3) ); }
            |   expression LE expression    { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Le, *$1, *$3) ); }
            |   expression EQ expression    { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Eq, *$1, *$3) ); }
            |   expression NE expression    { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Ne, *$1, *$3) ); }
            |   expression GE expression    { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Ge, *$1, *$3) ); }
            |   expression GT expression    { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Gt, *$1, *$3) ); }

            |   expression AND expression   { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::And, *$1, *$3) ); }
            |   expression OR expression    { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Or, *$1, *$3) ); }
            |   expression AND2 expression  { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::And2, *$1, *$3) ); }
            |   expression OR2 expression   { $$ = new RbPtr<SyntaxElement>( new SyntaxBinaryExpr(SyntaxBinaryExpr::Or2, *$1, *$3) ); }

            |   arrowAssign                 { $$ = $1; }
            |   equationAssign              { $$ = $1; }
            |   tildeAssign                 { $$ = $1; }
            |   tildeIidAssign              { $$ = $1; }

            |   functionCall                { $$ = new RbPtr<SyntaxElement>($1->get()); }

            |   variable                    { $$ = new RbPtr<SyntaxElement>($1->get()); }
            ;

arrowAssign     :   variable ARROW_ASSIGN expression
                    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::ArrowAssign, *$1, *$3) );
                    }
                |   functionCall ARROW_ASSIGN expression
                    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::ArrowAssign, *$1, *$3) );
                    }
                ;

tildeAssign     :   variable TILDE_ASSIGN functionCall
                    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::TildeAssign, *$1, *$3) );
                    }
                |   functionCall TILDE_ASSIGN functionCall
                    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::TildeAssign, *$1, *$3) );
                    }
                ;

tildeIidAssign  :   variable TILDEIID_ASSIGN functionCall
                    {
                        PRINTF("Parser inserting tilde iid assignment (TILDEIID_ASSIGN) in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::TildeIidAssign, *$1, *$3) );
                    }
                 |  functionCall TILDEIID_ASSIGN functionCall
                    {
                        PRINTF("Parser inserting tilde iid assignment (TILDEIID_ASSIGN) in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::TildeIidAssign, *$1, *$3) );
                    }
                ;

equationAssign  :   variable EQUATION_ASSIGN expression
                    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::EquationAssign, *$1, *$3) ); 
                    }
                |   functionCall EQUATION_ASSIGN expression
                    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxAssignExpr(SyntaxAssignExpr::EquationAssign, *$1, *$3) ); 
                    }
                ;

variable    :   identifier optElements
                {
                    PRINTF("Parser inserting variable (NAMED_VAR)in syntax tree\n");
                    $$ = new RbPtr<SyntaxVariable>( new SyntaxVariable(*$1, *$2) );
                }
            |   fxnCall '[' expression ']' optElements
                {
                    PRINTF("Parser inserting variable (FUNCTION_VAR) in syntax tree\n");
                    (*$5)->push_front(*$3);
                    $$ = new RbPtr<SyntaxVariable>( new SyntaxVariable(*$1, *$5) );
                }
            |   variable '.' identifier optElements
                {
                    PRINTF("Parser inserting member variable (NAMED_VAR)in syntax tree\n");
                    $$ = new RbPtr<SyntaxVariable>( new SyntaxVariable(*$1, *$3, *$4) );
                }
            |   variable '.' fxnCall '[' expression ']' optElements
                {
                    PRINTF("Parser inserting member variable (FUNCTION_VAR) in syntax tree\n");
                    (*$7)->push_front(*$5);
                    $$ = new RbPtr<SyntaxVariable>( new SyntaxVariable(*$1, *$3, *$7) );
                }
            ;

optElements :   /* empty */                     { $$ = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >() ); }
            |   elementList                     { $$ = $1; }
            ;
            
elementList :   '[' expression ']'              { $$ = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >(1, *$2) ); }
            |   '[' ']'                         { $$ = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >(1, RbPtr<SyntaxElement>( NULL ) ) ); }
            |   elementList '[' expression ']'  { (*$1)->push_back(*$3); $$ = $1; }
            |   elementList '[' ']'             { (*$1)->push_back(RbPtr<SyntaxElement>( NULL ) ); $$ = $1; }
            ;

fxnCall     :   identifier '(' optArguments ')' 
                {
                    $$ = new RbPtr<SyntaxFunctionCall>( new SyntaxFunctionCall(*$1, *$3) );
                }
            ;

functionCall    :   fxnCall
                    {
                        PRINTF("Parser inserting function call in syntax tree\n");
                        $$ = $1;
                    }
                |   variable '.' fxnCall 
                    {
                        PRINTF("Parser inserting member call in syntax tree\n");
                        (*$3)->setBaseVariable(*$1);
                        $$ = $3;
                    }
                ;

optArguments    :   /* empty */             { $$ = new RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > >( new std::list<RbPtr<SyntaxLabeledExpr> >() ); }
                |   argumentList            { $$ = $1; }
                ;

argumentList    :   argument                    { $$ = new RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > >( new std::list<RbPtr<SyntaxLabeledExpr> >(1,*$1) ); }
                |   argumentList ',' argument   { (*$1)->push_back(*$3); $$ = $1; }
                ;

argument   :    expression
                {
                    PRINTF("Parser inserting unlabeled argument in syntax tree\n");
                    $$ = new RbPtr<SyntaxLabeledExpr>( new SyntaxLabeledExpr(RbPtr<RbString>( new RbString("") ), *$1) );
                }
            |   identifier EQUAL expression
                { 
                    PRINTF("Parser inserting labeled argument in syntax tree\n");
                    $$ = new RbPtr<SyntaxLabeledExpr>( new SyntaxLabeledExpr(*$1, *$3) );
                }
            ;

functionDef :   FUNCTION identifier '(' optFormals ')' stmts
                {
                PRINTF("Parser inserting function definition in syntax tree\n");
                $$ = new RbPtr<SyntaxElement>( new SyntaxFunctionDef(RbPtr<RbString>( NULL ), *$2, *$4, *$6) );
                }
 
            |   FUNCTION typeSpec identifier '(' optFormals ')' stmts
                {
                PRINTF("Parser inserting typed function definition in syntax tree\n");
                $$ = new RbPtr<SyntaxElement>( new SyntaxFunctionDef(*$2, *$3, *$5, *$7) );
                }
            ;

optFormals  :   /* empty */             { $$ = new RbPtr<std::list<RbPtr<SyntaxFormal> > >( new std::list<RbPtr<SyntaxFormal> >() ); }
            |   formalList              { $$ = $1; }
            ;

formalList  :   formal                  { $$ = new RbPtr<std::list<RbPtr<SyntaxFormal> > >( new std::list<RbPtr<SyntaxFormal> >(1, *$1) ); }
            |   formalList ',' formal   { (*$1)->push_back(*$3); $$ = $1; }
            ;

formal      :   identifier
                {
                    PRINTF("Inserting labeled formal argument without default in syntax tree\n");
                    $$ = new RbPtr<SyntaxFormal>( new SyntaxFormal(*$1, RbPtr<SyntaxElement>( NULL ) ) );
                }
            |   identifier EQUAL expression
                {
                    PRINTF("Inserting labeled formal argument with default in syntax tree\n");
                    $$ = new RbPtr<SyntaxFormal>( new SyntaxFormal(*$1, *$3) );
                }
            |   typeSpec identifier
                {
                    PRINTF("Inserting typed labeled formal argument without default in syntax tree\n");
                    $$ = new RbPtr<SyntaxFormal>( new SyntaxFormal(*$1, *$2, RbPtr<SyntaxElement>(NULL) ) );
                }
            |   typeSpec identifier EQUAL expression
                {
                    PRINTF("Inserting typed labeled formal argument with default in syntax tree\n");
                    $$ = new RbPtr<SyntaxFormal>( new SyntaxFormal(*$1, *$2, *$4) );
                }
            ;

typeSpec    :   identifier optDims optRef   { (*$1)->append(*(*($2))); (*$1)->append(*(*($3))); $$ = $1; }
            ;

optDims     :   /* empty */                 { $$ = new RbPtr<RbString>( new RbString() ); }
            |   dimList                     { $$ = $1; }
            ;

dimList     :   '[' ']'                     { $$ = new RbPtr<RbString>( new RbString("[]") ); }
            |   '[' INT ']'                 { $$ = new RbPtr<RbString>( new RbString("[") ); (*$$)->append(INT); (*$$)->append("]"); }
            |   dimList '[' ']'             { (*$1)->append("[]"); $$ = $1; }
            |   dimList '[' INT ']'         { (*$1)->append("["); (*$1)->append(INT); (*$1)->append("]"); $$ = $1; }
            ;

optRef      :   /* empty */                 { $$ = new RbPtr<RbString>( new RbString() ); }
            |   '&'                         { $$ = new RbPtr<RbString>( new RbString("&") ); }
            ;

stmts       :   '{' stmtList '}'                { $$ = $2; }
            |   stmt_or_expr
                {
                    RbPtr<std::list<RbPtr<SyntaxElement> > >* stmts = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >() );
                    (*stmts)->push_back(*$1);
                    $$ = stmts;
                }
            ;

stmtList    :   /* empty */                 { $$ = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >() ); }
            |   stmt_or_expr                { $$ = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >(1, *$1) ); }
            |   stmtList ';' stmt_or_expr   { (*$1)->push_back(*$3); $$ = $1; }
            |   stmtList ';'                { $$ = $1; }
            |   stmtList '\n' stmt_or_expr  { (*$1)->push_back(*$3); $$ = $1; }
            |   stmtList '\n'               { $$ = $1; }
            ;

statement   :   ifStatement             { $$ = $1; }
            |   forStatement            { $$ = $1; }
            |   whileStatement          { $$ = $1; }
            |   nextStatement           { $$ = $1; }
            |   breakStatement          { $$ = $1; }
            |   returnStatement         { $$ = $1; }
            ;

stmt_or_expr    :   statement           { $$ = $1; }
                |   expression          { $$ = $1; }
                ;

declaration     :   classDef            { $$ = $1; }
                |   functionDef         { $$ = $1; }
                |   identifier optElements optRef identifier
                    {
                        PRINTF("Parser inserting variable declaration in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxVariableDecl(*$1, *$2, *$3, *$4) );
                    }
                ;

memberDefs      :   /* empty */                 { $$ = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >() ); }
                |   memberDef                   { $$ = new RbPtr<std::list<RbPtr<SyntaxElement> > >( new std::list<RbPtr<SyntaxElement> >(1, *$1) ); }
                |   memberDefs ';' memberDef    { (*$1)->push_back(*$3); $$ = $1; }
                |   memberDefs ';'              { $$ = $1; }
                |   memberDefs '\n' memberDef   { (*$1)->push_back(*$3); $$ = $1; }
                |   memberDefs '\n'             { $$ = $1; }
                ;

memberDef   :   formal      { $$ = new RbPtr<SyntaxElement>( $1->get() ); }
            |   functionDef { $$ = $1; }
            ;

classDef    :   CLASS identifier ':' identifier '{' memberDefs '}'
                {
                PRINTF("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
                $$ = new RbPtr<SyntaxElement>( new SyntaxClassDef(*$2, *$4, *$6) );
                }
            ;

ifStatement :   IF cond stmts               { $$ = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::If, *$2, *$3) ); }
            |   IF cond stmts ELSE stmts    { $$ = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::IfElse, *$2, *$3, *$5) ); }

cond    :   '(' expression ')'    { $$ = $2; }
        ;

forStatement    :   FOR forCond stmts   { $$ = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::For, *$2, *$3) ); }
                ;

forCond     :   '(' identifier IN expression ')'    { $$ = new RbPtr<SyntaxElement>( new SyntaxForCondition(*$2, *$4) ); }
            ;

whileStatement  :   WHILE cond stmts    { $$ = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::While, *$2, *$3) ); }
                ;

nextStatement   :   NEXT    { $$ = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::Next) ); }
                ;

breakStatement  :   BREAK   { $$ = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::Break) ); }
                ;

returnStatement :   RETURN              { $$ = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::Return) ); }
                |   RETURN expression   { $$ = new RbPtr<SyntaxElement>( new SyntaxStatement(SyntaxStatement::Return, *$2) ); }
                ;

identifier  :   NAME    { $$ = new RbPtr<RbString>( new RbString($1) ); }
            ;


vector      :   '[' vectorList ']'      { $$ = $2; }
            ;

vectorList  :   vectorList ',' expression   { (*$1)->push_back(RbPtr<SyntaxLabeledExpr>( new SyntaxLabeledExpr( RbPtr<RbString>( new RbString("") ), *$3) ) ); $$ = $1; }
            |   expression                  { $$ = new RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > >( new std::list<RbPtr<SyntaxLabeledExpr> >(1, RbPtr<SyntaxLabeledExpr>( new SyntaxLabeledExpr(RbPtr<RbString>( new RbString("") ), *$1) ) ) ); }
            ;

constant    :   FALSE
                {
                    PRINTF("Parser inserting bool constant (false) in syntax tree\n");
                    $$ = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new RbBoolean(false) ) ) );
                }
            |   TRUE
                {
                    PRINTF("Parser inserting bool constant (true) in syntax tree\n");
                    $$ = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new RbBoolean(true) ) ) );
                }
            |   RBNULL
                {
                    PRINTF("Parser inserting null constant in syntax tree\n");
                    $$ = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( NULL ) ) );
                }
            |   INT
                {
                    if ( $1 < 0 ) {
                        PRINTF("Parser inserting Integer constant in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new Integer($1) ) ) );
                    }
                    else { 
                        PRINTF("Parser inserting Natural constant in syntax tree\n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new Natural($1) ) ) );
                    }
                }
            |   STRING
                {
                    PRINTF("Parser inserting String constant in syntax tree\n");
                    $$ = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new RbString($1) ) ) );
                }
            |   REAL
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
                    if ($1 > 0.0) {
                        PRINTF("Parser inserting RealPos constant in syntax tree \n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new RealPos($1) ) ) );
                    }
                    else {
                        PRINTF("Parser inserting Real constant in syntax tree \n");
                        $$ = new RbPtr<SyntaxElement>( new SyntaxConstant(RbPtr<RbLanguageObject>( new Real($1) ) ) );
                    }
                }
            ;

%%


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


