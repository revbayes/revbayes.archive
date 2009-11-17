%{
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

%}

/* Bison needs a union of the types handled by the parser */
%union {
    char*                  string;
    double                 realValue;
    int                    intValue;
    bool                   boolValue;
    SyntaxElement*         syntaxElement;
    list<SyntaxElement *>* syntaxElementList;
};

/* Return types of the elements handled by the parser */
%type <string> NAME STRING
%type <realValue> REAL
%type <intValue> INT NULL
%type <boolValue> FALSE TRUE
%type <string> identifier
%type <syntaxElement> constant
%type <syntaxElement> statement expression
%type <syntaxElement> arrowAssign tildeAssign equationAssign
%type <syntaxElement> variable optElement functionCall
%type <syntaxElement> declaration classDef memberDef
%type <syntaxElement> functionDef argument formal
%type <syntaxElement> forStatement ifStatement whileStatement
%type <syntaxElement> forCond cond returnStatement
%type <syntaxElement> nextStatement breakStatement
%type <syntaxElementList> argumentList optArguments
%type <syntaxElementList> stmts stmtList optStatements
%type <syntaxElementList> formalList optFormals memberDefs

/* Tokens returned by the lexer and handled by the parser */
%token REAL INT NAME STRING NULL FALSE TRUE COMMENT
%token FUNCTION CLASS FOR IN IF ELSE WHILE NEXT BREAK RETURN
%token LEFT_ASSIGN TILDE_ASSIGN EQUATION_ASSIGN EQUAL
%token AND OR AND2 OR2 GT GE LT LE EQ NE
%token END_OF_INPUT

/*
 * Precedence table, low to high, with order of evaluation.
 * We follow R as much as possible. 
 */
%left       '?'
%left       LOW WHILE FOR
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
%left       '.'
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
 * components in EvoMoDeL are tilde assignment ('~')for creating
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

prog    :       END_OF_INPUT            { return 0; }
        |       '\n'                    { return 0; }
        |       COMMENT '\n'            { return 0;}
        |       statement '\n'          { return parser.execute($1); }
        |       declaration '\n'        { return parser.execute($1); }
        |       expression '\n'         { return parser.execute($1); }
        |       error                   { YYABORT; }
        ;

expression  :   constant                    { $$ = $1; }

            |   '(' expression ')'          { $$ = $2; }

            |   '-' expression %prec UMINUS { $$ = new SyntaxUnaryExpr(SyntaxUnaryExpr::UMINUS, $2); }
            |   '+' expression %prec UPLUS  { $$ = new SyntaxUnaryExpr(SyntaxUnaryExpr::UPLUS, $2); }
            |   '!' expression %prec UNOT   { $$ = new SyntaxUnaryExpr(SyntaxUnaryExpr::UNOT, $2); }
            |   '?' identifier              { $$ = new SyntaxHelpExpr($2); }

            |   expression ':' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::RANGE, $1, $3); }

            |   expression '+' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::ADD, $1, $3); }
            |   expression '-' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::SUB, $1, $3); }
            |   expression '*' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::MUL, $1, $3); }
            |   expression '/' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::DIV, $1, $3); }
            |   expression '^' expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::EXP, $1, $3); }

            |   expression LT expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::LT, $1, $3); }
            |   expression LE expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::LE, $1, $3); }
            |   expression EQ expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::EQ, $1, $3); }
            |   expression NE expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::NE, $1, $3); }
            |   expression GE expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::GE, $1, $3); }
            |   expression GT expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::GT, $1, $3); }

            |   expression AND expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::AND, $1, $3); }
            |   expression OR expression    { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::OR, $1, $3); }
            |   expression AND2 expression  { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::AND, $1, $3); }
            |   expression OR2 expression   { $$ = new SyntaxBinaryExpr(SyntaxBinaryExpr::OR, $1, $3); }

            |   arrowAssign                 { $$ = $1; }
            |   equationAssign              { $$ = $1; }
            |   tildeAssign                 { $$ = $1; }

            |   functionCall                { $$ = $1; }

            |   variable                    { $$ = $1; }
            ;

arrowAssign     :   variable ARROW_ASSIGN expression
                    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        $$ = new SyntaxAssignExpr(SyntaxAssignExpr::ARROW_ASSIGN, $1, $3);
                    }
                ;

tildeAssign     :   variable TILDE_ASSIGN identifier '(' argumentList ')'
                    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        $$ = new SyntaxAssignExpr(SyntaxAssignExpr::TILDE_ASSIGN, $1, $3, $5);
                    }
                ;

equationAssign     :   variable EQUATION_ASSIGN expression
                    {
                        PRINTF("Parser inserting equation assignment (EQUATION_ASSIGN) in syntax tree\n");
                        $$ = new SyntaxAssignExpr(SyntaxAssignExpr::EQUATION_ASSIGN, $1, $3); 
                    }
                ;

variable    :   identifier optElement
                {
                    PRINTF("Parser inserting variable (VARIABLE) in syntax tree\n");
                    $$ = new SyntaxVariable($1, $2);
                }
            |   variable '.' identifier optElement
                {
                    PRINTF("Parser inserting member variable (MEMBER) in syntax tree\n");
                    $$ = new SyntaxVariable($1, $3, $4);
                }
            ;

optElement  :   '[' expression ']' optElement   { $$ = $4->push_front($2); }
            |   /* empty */                     { $$ = new list<SyntaxElement*>(); }
            ;

functionCall    :   identifier '(' argumentList ')' 
                    { 
                        PRINTF("Parser inserting function call (FUNCTION_CALL) in syntax tree\n");
                        $$ = new SyntaxFunctionCall(string($1), $3);
                    }
                |   variable '.' identifier '(' argumentList ')'
                    {
                        PRINTF("Parser inserting member function call (MEMBER_FUNCTION_CALL) in syntax tree\n");
                        $$ = new SyntaxFunctionCall($1, string($3), $5);
                    }
                ;

argumentList    :   argument optArguments   { $2->push_front($1); $$ = $2; }
                |   /* empty */             { $$ = new list<SyntaxElement *>(); }
                ;

optArguments    :   ',' argument optArguments   { $3->push_front($2); $$ = $3; }
                |   /* empty */                 { $$ = new list<SyntaxElement *>(); }
                ;

argument   :   expression
                {
                    PRINTF("Parser inserting unlabeled argument (ARGUMENT) in syntax tree\n");
                    $$ = new SyntaxLabeledExpr(NULL, $1);
                }
            |   identifier EQUAL expression
                { 
                    PRINTF("Parser inserting labeled argument (ARGUMENT) in syntax tree\n");
                    $$ = new SyntaxLabeledExpr(string($1), $3);
                }
            ;

functionDef :   FUNCTION identifier '(' formalList ')' stmts
                {
                PRINTF("Parser inserting function definition (FUNCTION_DEF) in syntax tree\n");
                $$ = new SyntaxFunctionDef(NULL, $2, $4, $6);
                }
            |   FUNCTION identifier identifier '(' formalList ')' stmts
                {
                PRINTF("Parser inserting typed function definition (FUNCTION_DEF) in syntax tree\n");
                $$ = new SyntaxFunctionDef($2, $3, $5, $7);
                }
            ;

formalList  :   formal optFormals   { $$ = ($2->push_front($1)); }
            |   /* empty */         { $$ = new list<SyntaxElement *>(); }
            ;

optFormals  :   ',' formal optFormals   { $$ = ($3->push_front($2)); }
            |   /* empty */             { $$ = new list<SyntaxElement *>(); }
            ;

formal      :   identifier
                {
                    PRINTF("Parser inserting unlabeled formal argument (FORMAL) in syntax tree\n");
                    $$ = new SyntaxLabeledExpr(NULL, $1, NULL);
                }
            |   identifier EQUAL expression
                { 
                    PRINTF("Parser inserting labeled formal argument (FORMAL) in syntax tree\n");
                    $$ = new SyntaxLabeledExpr(NULL, $1, $3);
                }
            |   identifier identifier
                {
                    PRINTF("Parser inserting unlabeled typed formal argument (FORMAL) in syntax tree\n");
                    $$ = new SyntaxLabeledExpr($1, $2, NULL);
                }
            |   identifier identifier EQUAL expression
                {
                    PRINTF("Parser inserting labeled typed formal argument (FORMAL) in syntax tree\n");
                    $$ = new SyntaxLabeledExpr($1, $2, $4);
                }
            ;

stmts       :   '{' stmtList '}'                { $$ = $2; }
            |   statement                       { $$ = $1; }
            ;

stmtList        :   statement optStatements     { $$ = ($2->push_front($1)); }
                |   /* empty */                 { $$ = new list<SyntaxElement *>(); }   
                ;

optStatements   :   ';' statement optStatements     { $$ = ($3->push_front($2)); }
                |   '\n' statement optStatements    { $$ = ($3->push_front($2)); }
                |   /* empty */                     { $$ = new list<SyntaxElement *>(); }
                ;

statement   :   ifStatement             { $$ = $1; }
            |   forStatement            { $$ = $1; }
            |   whileStatement          { $$ = $1; }
            |   nextStatement           { $$ = $1; }
            |   breakStatement          { $$ = $1; }
            |   returnStatement         { $$ = $1; }
            |   expression '\n'         { $$ = $1; }
            |   expression ';'          { $$ = $1; }
            |   ';'                     { $$ = NULL; }
            ;

declaration     :   classDef        { $$ = $1; }
                |   functionDef     { $$ = $1; }
                ;

memberDefs      :   memberDef                   { $$ = new list<SyntaxElement*>($1); }
                |   memberDefs ';' memberDef    { $$ = $1->push_back($3); }
                |   memberDefs '\n' memberDef   { $$ = $1->push_back($3); }
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

ifStatement :   IF cond stmts               { $$ = new SyntaxStatement(SyntaxStatement::IF, $2, $3); }
            |   IF cond stmts ELSE stmts    { $$ = new SyntaxStatement(SyntaxStatement::IF_ELSE, $2, $3, $5); }

cond    :   '(' expression ')'    { $$ = new SyntaxCondition($2); }
        ;

forStatement    :   FOR forCond stmts   { $$ = new SyntaxStatement(SyntaxStatement::FOR, $2, $3); }
                ;

forCond     :   '(' identifier IN expression ')'    { $$ = new SyntaxForCondition(string($2), $4); }
            ;

whileStatement  :   WHILE cond stmts    { $$ = new SyntaxStatement(SyntaxStatement::WHILE, $2, $3); }
                ;

nextStatement   :   NEXT    { $$ = new SyntaxStatement(SyntaxStatement::NEXT); }
                ;

breakStatement  :   BREAK   { $$ = new SyntaxStatement(SyntaxStatement::BREAK); }
                ;

returnStatement :   RETURN              { $$ = new SyntaxStatement(SyntaxStatement::RETURN); }
                |   RETURN expression   { $$ = new SyntaxStatement(SyntaxStatement::RETURN, $2); }
                ;

identifier  :   NAME    { $$ = new RbString($1); }
            ;

constant    :   FALSE   { $$ = new SyntaxConstant(false); }
            |   TRUE    { $$ = new SyntaxConstant(true); }
            |   NULL    { $$ = new SyntaxConstant(0); }
            |   INT     { $$ = new SyntaxConstant($1); }
            |   STRING  { $$ = new SyntaxConstant($1)); }
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
                        prec = strlen(yytext) - 1 - i;
                    }
                    Scalar *real = new Scalar($1);
                    //real->setPrecision(prec);
                    //real->setScientific(sci);
                    $$ = new SyntaxConstat(real);
                }
            ;

%%


/* Definition of yyerror. */
int yyerror(char *msg)
{
    PRINTF("Bison code said: %s\n", msg);
    return 0;     // or return 1 ??
}
