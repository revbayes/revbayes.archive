%{
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

%}

/* Bison needs a union of the types handled by the parser */
%union {
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
};

/* Return types of the elements handled by the parser */
%type <string> NAME STRING
%type <realValue> REAL
%type <intValue> INT RBNULL
%type <boolValue> FALSE TRUE
%type <idString> identifier typeSpec optDims dimList optRef
%type <syntaxVariable> variable
%type <syntaxFunctionCall> functionCall
%type <syntaxLabeledExpr> argument
%type <syntaxFormal> formal
%type <syntaxElement> constant
%type <syntaxElement> statement expression stmt_or_expr
%type <syntaxElement> arrowAssign tildeAssign equationAssign
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
%token ARROW_ASSIGN TILDE_ASSIGN EQUATION_ASSIGN EQUAL 
%token AND OR AND2 OR2 GT GE LT LE EQ NE
%token END_OF_INPUT

/* Destructors */
%destructor { for (std::list<SyntaxElement*    >::iterator i=$$->begin(); i!=$$->end(); i++) delete (*i); delete ($$); PRINTF("Deleting element list\n"); } elementList optElements stmts stmtList memberDefs 
%destructor { for (std::list<SyntaxLabeledExpr*>::iterator i=$$->begin(); i!=$$->end(); i++) delete (*i); delete ($$); PRINTF("Deleting argument list\n"); } argumentList optArguments vectorList vector
%destructor { for (std::list<SyntaxFormal*     >::iterator i=$$->begin(); i!=$$->end(); i++) delete (*i); delete ($$); PRINTF("Deleting formal list\n"); } formalList optFormals
%destructor { delete ($$); PRINTF("Deleting identifier  ...\n"); } identifier typeSpec optDims dimList optRef
%destructor { delete ($$); PRINTF("Deleting variable    ...\n"); } variable functionCall argument formal constant
%destructor { delete ($$); PRINTF("Deleting expression  ...\n"); } statement expression stmt_or_expr 
%destructor { delete ($$); PRINTF("Deleting assignment  ...\n"); } arrowAssign tildeAssign equationAssign 
%destructor { delete ($$); PRINTF("Deleting declaration ...\n"); } declaration classDef memberDef 
%destructor { delete ($$); PRINTF("Deleting functiondef ...\n"); } functionDef 
%destructor { delete ($$); PRINTF("Deleting for/if/while...\n"); } forStatement ifStatement whileStatement 
%destructor { delete ($$); PRINTF("Deleting cond/return ...\n"); } forCond cond returnStatement
%destructor { delete ($$); PRINTF("Deleting next/break  ...\n"); } nextStatement breakStatement

/* Use location tracking */
%locations

/*
 * Precedence table, low to high, with order of evaluation.
 * We follow R as much as possible. 
 */
%left       '?'
%left       WHILE FOR
%right      IF
%left       ELSE
%right      ARROW_ASSIGN
%right      TILDE_ASSIGN
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
 * As a subset of the grammar, RevBayes supports EvoMoDeL,
 * which allows a natural description of graphical represen-
 * tations of complex probability models. The most essential
 * components in EvoMoDeL are tilde assignment ('~') for creating
 * stochastic nodes and equation assignment (':=') for creating
 * deterministic nodes in model DAGs. Constant nodes are created
 * using arrow assignment ('<-') as in R.
 *
 * The language passes by value except in equation and tilde
 * assignment, in which case variables are passed by reference.
 * 
 * Unlike R, right assignment ('->') is not supported. Also,
 * a number of constructs that are interpreted as expressions
 * in R are treated as statements or declarations in RevBayes
 * and have no return value (or the return value is discarded).
 * These constructs include control statements and function def-
 * initions, among others.
 *
 * For more discussion of the language, see the Revbayes docu-
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
                    return parser.execute($1);
                }
        |       stmt_or_expr ';'
                {
                    PRINTF("Bison trying to execute statement or expression\n");
                    return parser.execute($1);
                }
        |       declaration '\n'
                {
                    PRINTF("Bison trying to execute declaration\n");
                    return parser.execute($1);
                }
        |       declaration ';'
                {
                    PRINTF("Bison trying to execute declaration\n");
                    return parser.execute($1);
                }
        |       '?' identifier '\n'
                {
                    PRINTF("Bison trying to get help for symbol\n");
                    return parser.help($2);
                }
        |       '?' identifier ';'
                {
                    PRINTF("Bison trying to get help for symbol\n");
                    return parser.help($2);
                }
        |       '?' functionCall '\n'
                {
                    PRINTF("Bison trying to get help for function call\n");
                    return parser.help($2);
                }
        |       '?' functionCall ';'
                {
                    PRINTF("Bison trying to get help for function call\n");
                    return parser.help($2);
                }
        |       error
                {
                    PRINTF("Bison calling YYABORT\n");
                    PRINTF("Error when reading line %d position %d to line %d position %d\n", @$.first_line, @$.first_column, @$.last_line, @$.last_column);
                    YYABORT;
                }
        ;

expression  :   constant                    { $$ = $1; }

            |   vector                      { $$ = new SyntaxFunctionCall(new RbString("v"), $1); }

            |   '(' expression ')'          { $$ = $2; }

            |   '-' expression %prec UMINUS { $$ = new SyntaxUnaryExpr(SyntaxUnaryExpr::UMinus, $2); }
            |   '+' expression %prec UPLUS  { $$ = new SyntaxUnaryExpr(SyntaxUnaryExpr::UPlus, $2); }
            |   '!' expression %prec UNOT   { $$ = new SyntaxUnaryExpr(SyntaxUnaryExpr::UNot, $2); }
            |   AND expression %prec UAND   { $$ = new SyntaxUnaryExpr(SyntaxUnaryExpr::UAnd, $2); }

            |   expression ':' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Range, $1, $3); }

            |   expression '+' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Add, $1, $3); }
            |   expression '-' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Sub, $1, $3); }
            |   expression '*' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Mul, $1, $3); }
            |   expression '/' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Div, $1, $3); }
            |   expression '^' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Exp, $1, $3); }

            |   expression LT expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Lt, $1, $3); }
            |   expression LE expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Le, $1, $3); }
            |   expression EQ expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Eq, $1, $3); }
            |   expression NE expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ne, $1, $3); }
            |   expression GE expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Ge, $1, $3); }
            |   expression GT expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Gt, $1, $3); }

            |   expression AND expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::And, $1, $3); }
            |   expression OR expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or, $1, $3); }
            |   expression AND2 expression  { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::And2, $1, $3); }
            |   expression OR2 expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::Or2, $1, $3); }

            |   arrowAssign                 { $$ = $1; }
            |   equationAssign              { $$ = $1; }
            |   tildeAssign                 { $$ = $1; }

            |   functionCall                { $$ = $1; }

            |   variable                    { $$ = $1; }
            ;

arrowAssign     :   variable ARROW_ASSIGN expression
                    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        $$ = new SyntaxAssignExpr(SyntaxAssignExpr::ArrowAssign, $1, $3);
                    }
                ;

tildeAssign     :   variable TILDE_ASSIGN functionCall
                    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        $$ = new SyntaxAssignExpr(SyntaxAssignExpr::TildeAssign, $1, $3);
                    }
                ;

equationAssign  :   variable EQUATION_ASSIGN expression
                    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        $$ = new SyntaxAssignExpr(SyntaxAssignExpr::EquationAssign, $1, $3); 
                    }
                ;

variable    :   identifier optElements
                {
                    PRINTF("Parser inserting variable (VARIABLE) in syntax tree\n");
                    $$ = new SyntaxVariable($1, $2);
                }
            |   variable '.' identifier optElements
                {
                    PRINTF("Parser inserting member variable (MEMBER) in syntax tree\n");
                    $$ = new SyntaxVariable($1, $3, $4);
                }
            ;

optElements :   /* empty */                     { $$ = new std::list<SyntaxElement*>(); }
            |   elementList                     { $$ = $1; }
            ;
            
elementList :   '[' expression ']'              { $$ = new std::list<SyntaxElement*>(1, $2); }
            |   elementList '[' expression ']'  { $1->push_back($3); $$ = $1; }
            ;

functionCall    :   identifier '(' optArguments ')' 
                    {
                        PRINTF("Parser inserting function call in syntax tree\n");
                        $$ = new SyntaxFunctionCall($1, $3);
                    }
                |   variable '.' identifier '(' optArguments ')'
                    {
                        PRINTF("Parser inserting member function call in syntax tree\n");
                        $$ = new SyntaxFunctionCall($1, $3, $5);
                    }
                ;

optArguments    :   /* empty */             { $$ = new std::list<SyntaxLabeledExpr *>(); }
                |   argumentList            { $$ = $1; }
                ;

argumentList    :   argument                    { $$ = new std::list<SyntaxLabeledExpr *>(1,$1); }
                |   argumentList ',' argument   { $1->push_back($3); $$ = $1; }
                ;

argument   :    expression
                {
                    PRINTF("Parser inserting unlabeled argument in syntax tree\n");
                    $$ = new SyntaxLabeledExpr(new RbString(""), $1);
                }
            |   identifier EQUAL expression
                { 
                    PRINTF("Parser inserting labeled argument in syntax tree\n");
                    $$ = new SyntaxLabeledExpr($1, $3);
                }
            ;

functionDef :   FUNCTION identifier '(' optFormals ')' stmts
                {
                PRINTF("Parser inserting function definition in syntax tree\n");
                $$ = new SyntaxFunctionDef(NULL, $2, $4, $6);
                }
 
            |   FUNCTION typeSpec identifier '(' optFormals ')' stmts
                {
                PRINTF("Parser inserting typed function definition in syntax tree\n");
                $$ = new SyntaxFunctionDef($2, $3, $5, $7);
                }
            ;

optFormals  :   /* empty */             { $$ = new std::list<SyntaxFormal *>(); }
            |   formalList              { $$ = $1; }
            ;

formalList  :   formal                  { $$ = new std::list<SyntaxFormal *>(1,$1); }
            |   formalList ',' formal   { $1->push_back($3); $$ = $1; }
            ;

formal      :   identifier
                {
                    PRINTF("Inserting labeled formal argument without default in syntax tree\n");
                    $$ = new SyntaxFormal($1, NULL);
                }
            |   identifier EQUAL expression
                {
                    PRINTF("Inserting labeled formal argument with default in syntax tree\n");
                    $$ = new SyntaxFormal($1, $3);
                }
            |   typeSpec identifier
                {
                    PRINTF("Inserting typed labeled formal argument without default in syntax tree\n");
                    $$ = new SyntaxFormal($1, $2, NULL);
                }
            |   typeSpec identifier EQUAL expression
                {
                    PRINTF("Inserting typed labeled formal argument with default in syntax tree\n");
                    $$ = new SyntaxFormal($1, $2, $4);
                }
            ;

typeSpec    :   identifier optDims optRef   { $1->append(*($2)); delete($2); $1->append(*($3)); delete($3); $$ = $1; }
            ;

optDims     :   /* empty */                 { $$ = new RbString(); }
            |   dimList                     { $$ = $1; }
            ;

dimList     :   '[' ']'                     { $$ = new RbString("[]"); }
            |   dimList '[' ']'             { $1->append("[]"); $$ = $1; }
            ;

optRef      :   /* empty */                 { $$ = new RbString(); }
            |   '&'                         { $$ = new RbString("&"); }
            ;

stmts       :   '{' stmtList '}'                { $$ = $2; }
            |   stmt_or_expr
                {
                    std::list<SyntaxElement*> stmts;
                    stmts.push_back($1);
                    $$ = &stmts;
                }
            ;

stmtList    :   /* empty */                 { $$ = new std::list<SyntaxElement*>(); }
            |   stmt_or_expr                { $$ = new std::list<SyntaxElement*>(1, $1); }
            |   stmtList ';' stmt_or_expr   { $1->push_back($3); $$ = $1; }
            |   stmtList ';'                { $$ = $1; }
            |   stmtList '\n' stmt_or_expr  { $1->push_back($3); $$ = $1; }
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

declaration     :   classDef        { $$ = $1; }
                |   functionDef     { $$ = $1; }
                ;

memberDefs      :   /* empty */                 { $$ = new std::list<SyntaxElement*>(); }
                |   memberDef                   { $$ = new std::list<SyntaxElement*>(1, $1); }
                |   memberDefs ';' memberDef    { $1->push_back($3); $$ = $1; }
                |   memberDefs ';'              { $$ = $1; }
                |   memberDefs '\n' memberDef   { $1->push_back($3); $$ = $1; }
                |   memberDefs '\n'             { $$ = $1; }
                ;

memberDef   :   formal      { $$ = $1; }
            |   functionDef { $$ = $1; }
            ;

classDef    :   CLASS identifier ':' identifier '{' memberDefs '}'
                {
                PRINTF("Parser inserting class definition (CLASS_DEF) in syntax tree\n");
                $$ = new SyntaxClassDef($2, $4, $6);
                }
            ;

ifStatement :   IF cond stmts               { $$ = new SyntaxStatement(SyntaxStatement::If, $2, $3); }
            |   IF cond stmts ELSE stmts    { $$ = new SyntaxStatement(SyntaxStatement::IfElse, $2, $3, $5); }

cond    :   '(' expression ')'    { $$ = $2; }
        ;

forStatement    :   FOR forCond stmts   { $$ = new SyntaxStatement(SyntaxStatement::For, $2, $3); }
                ;

forCond     :   '(' identifier IN expression ')'    { $$ = new SyntaxForCondition($2, $4); }
            ;

whileStatement  :   WHILE cond stmts    { $$ = new SyntaxStatement(SyntaxStatement::While, $2, $3); }
                ;

nextStatement   :   NEXT    { $$ = new SyntaxStatement(SyntaxStatement::Next); }
                ;

breakStatement  :   BREAK   { $$ = new SyntaxStatement(SyntaxStatement::Break); }
                ;

returnStatement :   RETURN              { $$ = new SyntaxStatement(SyntaxStatement::Return); }
                |   RETURN expression   { $$ = new SyntaxStatement(SyntaxStatement::Return, $2); }
                ;

identifier  :   NAME    { $$ = new RbString($1); }
            ;


vector      :   '[' vectorList ']'      { $$ = $2; }
            ;

vectorList  :   vectorList ',' expression   { $1->push_back(new SyntaxLabeledExpr(new RbString(""), $3)); $$ = $1; }
            |   expression                  { $$ = new std::list<SyntaxLabeledExpr*>(1, new SyntaxLabeledExpr(new RbString(""), $1)); }
            ;

constant    :   FALSE
                {
                    PRINTF("Parser inserting bool constant (false) in syntax tree\n");
                    $$ = new SyntaxConstant(new RbBool(false));
                }
            |   TRUE
                {
                    PRINTF("Parser inserting bool constant (true) in syntax tree\n");
                    $$ = new SyntaxConstant(new RbBool(true));
                }
            |   RBNULL
                {
                    PRINTF("Parser inserting null constant in syntax tree\n");
                    $$ = new SyntaxConstant(NULL);
                }
            |   INT
                {
                    PRINTF("Parser inserting int constant in syntax tree\n");
                    $$ = new SyntaxConstant(new RbInt($1));
                }
            |   STRING
                {
                    PRINTF("Parser inserting string constant in syntax tree\n");
                    $$ = new SyntaxConstant(new RbString($1));
                }
            |   REAL
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
                    if ($1 > 0.0)
                        real = new PosReal($1);
                    else
                        real = new RbDouble($1);
                    //real->setPrecision(prec);
                    //real->setScientific(sci);
                    PRINTF("Parser inserting double constant in syntax tree \n");
                    $$ = new SyntaxConstant(real);
                }
            ;

%%


/* Definition of yyerror. */
int yyerror(const char *msg)
{
    PRINTF("Bison code said: %s\n", msg);
    return 1;
}
