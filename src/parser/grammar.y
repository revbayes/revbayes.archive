%{
/*
 * \file
 * Grammar specification in bison format for REvBayes, an R-like environment 
 * for Evolutionary analysis using Bayesian inference. REvBayes uses EvoMoDeL,
 * Evolutionary Model Description Language, to describe evolutionary models
 * used in Bayesian MCMC inference, so the EvoMoDeL grammar is a subset of the
 * REvBayes grammar.
 *
 * The grammar borrows heavily from the R grammar specification in the gram.y 
 * file of the R source code. EvoMoDeL itself is inspired by the language used  
 * by BUGS to describe models. Unlike BUGS, and similar programs such as JAGS,  
 * REvBayes allows models to be built in an interpreted environment.
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

#include "RbDataType.h"
#include "RbObject.h"
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

#ifdef YACC_VERBOSE
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
    char                  *string;
    double                 realValue;
    int                    intValue;
    bool                   boolValue;
    RbObject              *object;
    SyntaxElement         *syntaxElement;
    list<SyntaxElement *> *syntaxElementList;
};

/* Return types of the elements handled by the parser */
%type <string> NAME STRING
%type <realValue> REAL
%type <intValue> INT NULL
%type <boolValue> FALSE TRUE
%type <object> constant identifier
%type <object> optAttribute
%type <syntaxElement> statement expression
%type <syntaxElement> arrowAssign tildeAssign equalAssign
%type <syntaxElement> variable optElement sub argument formal
%type <syntaxElement> functionCall functionDef
%type <syntaxElement> forStatement ifStatement whileStatement
%type <syntaxElement> forCond cond
%type <syntaxElement> nextStatement breakStatement
%type <syntaxElementList> argumentList optArguments stmts stmtList optStatements
%type <syntaxElementList> formalList optFormals subList

/* Tokens returned by the lexer and handled by the parser */
%token REAL INT NAME STRING NULL FALSE TRUE COMMENT
%token FUNCTION FOR IN IF ELSE WHILE NEXT BREAK
%token LEFT_ASSIGN TILDE_ASSIGN EQUAL_ASSIGN EQUAL
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
%right      EQUAL_ASSIGN
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
%left       '$'
%nonassoc   '(' '['
%%

/*
 * The grammar is essentially a simplified dialect of R,
 * extended with model statements similar to those in BUGS.
 * An important difference compared to the R grammar is that
 * we enforce the left hand side of an assignment to be a
 * variable or an accessor function call. By enforcing this
 * syntax, we simplify the type and error checking in the parser.
 * Another important difference is that we use equal
 * assignments for creating deterministic nodes in model
 * DAGs. A separate type of assign statement is required for
 * the parser to be able to distinguish between statements
 * that assign values to data nodes and statements that assign
 * expressions to deterministic nodes for dynamic evaluation.
 * R treats model formulas as arguments but we consider them
 * as another type of assignment (tilde), creating stochastic
 * nodes with specified distributions. Unlike regular function
 * calls, the distribution function calls used in the tilde
 * statements use pass-by-reference rather than by value for its
 * arguments. Unlike R, we do not support right assignments
 * because we consider those to be unneessary and potentially
 * confusing for users.
 *
 */
prog    :       END_OF_INPUT            { return 0; }
        |       '\n'                    { return 0; }
        |       COMMENT '\n'            { return 0;}
        |       statement '\n'          { return parser.execute($1); }
        |       expression '\n'         { return parser.execute($1); }
        |       error                   { YYABORT; }
        ;

expression  :   constant                    { $$ = new Constant($1); }

            |   '(' expression ')'          { $$ = $2; }

            |   '-' expression %prec UMINUS { $$ = new UnaryExpr(UnaryExpr::UMINUS, $2); }
            |   '+' expression %prec UPLUS  { $$ = new UnaryExpr(UnaryExpr::UPLUS, $2); }
            |   '!' expression %prec UNOT   { $$ = new UnaryExpr(UnaryExpr::UNOT, $2); }
            |   '?' identifier              { $$ = new HelpExpr($2); }

            |   expression ':' expression   { $$ = new BinaryExpr(BinaryExpr::RANGE, $1, $3); }

            |   expression '+' expression   { $$ = new BinaryExpr(BinaryExpr::ADD, $1, $3); }
            |   expression '-' expression   { $$ = new BinaryExpr(BinaryExpr::SUB, $1, $3); }
            |   expression '*' expression   { $$ = new BinaryExpr(BinaryExpr::MUL, $1, $3); }
            |   expression '/' expression   { $$ = new BinaryExpr(BinaryExpr::DIV, $1, $3); }
            |   expression '^' expression   { $$ = new BinaryExpr(BinaryExpr::EXP, $1, $3); }

            |   expression LT expression    { $$ = new BinaryExpr(BinaryExpr::LT, $1, $3); }
            |   expression LE expression    { $$ = new BinaryExpr(BinaryExpr::LE, $1, $3); }
            |   expression EQ expression    { $$ = new BinaryExpr(BinaryExpr::EQ, $1, $3); }
            |   expression NE expression    { $$ = new BinaryExpr(BinaryExpr::NE, $1, $3); }
            |   expression GE expression    { $$ = new BinaryExpr(BinaryExpr::GE, $1, $3); }
            |   expression GT expression    { $$ = new BinaryExpr(BinaryExpr::GT, $1, $3); }

            |   expression AND expression   { $$ = new BinaryExpr(BinaryExpr::AND, $1, $3); }
            |   expression OR expression    { $$ = new BinaryExpr(BinaryExpr::OR, $1, $3); }
            |   expression AND2 expression  { $$ = new BinaryExpr(BinaryExpr::AND, $1, $3); }
            |   expression OR2 expression   { $$ = new BinaryExpr(BinaryExpr::OR, $1, $3); }

            |   arrowAssign                 { $$ = $1; }
            |   equalAssign                 { $$ = $1; }
            |   tildeAssign                 { $$ = $1; }

            |   functionDef %prec LOW       { $$ = $1; }
            |   functionCall                { $$ = $1; }

            |   variable                    { $$ = $1; }
            ;

arrowAssign     :   variable ARROW_ASSIGN expression
                    { 
                        PRINTF("Parser inserting arrow assignment (ARROW_ASSIGN) in syntax tree\n");
                        $$ = new AssignExpr(AssignExpr::ARROW_ASSIGN, $1, $3);
                    }
                |   functionCall ARROW_ASSIGN expression
                    {
                        PRINTF("Parser inserting attribute assignment (ATTRIBUTE_ASSIGN) in syntax tree\n");
                        $$ = new AssignExpr(AssignExpr::ATTRIBUTE_ASSIGN, $1, $3);
                    }
                ;

tildeAssign     :   variable TILDE_ASSIGN functionCall
                    {
                        PRINTF("Parser inserting tilde assignment (TILDE_ASSIGN) in syntax tree\n");
                        $$ = new AssignExpr(AssignExpr::TILDE_ASSIGN, $1, $3); 
                    }
                ;

equalAssign     :   variable EQUAL_ASSIGN expression
                    {
                        PRINTF("Parser inserting equation assignment (EQUAL_ASSIGN) in syntax tree\n");
                        $$ = new AssignExpr(AssignExpr::EQUAL_ASSIGN, $1, $3); 
                    }
                ;

variable    :   identifier optAttribute optElement
                {
                    PRINTF("Parser inserting variable (VARIABLE) in syntax tree\n");
                    $$ = new Variable($1, $2, $3);
                }
            ;

optAttribute    :   '$' identifier  { $$ = $2; }
                |   /* empty */     { $$ = NULL; }
                ;

optElement  :   '[' subList ']'     { $$ = $2; }
            |   /* empty */         { $$ = NULL; }
            ;

subList     :   sub                 { $$ = new list<SyntaxElement>($1); }
            |   subList ',' sub     { $$ = $1->push_back($3); }
            ;

sub         :   /* empty */         { $$ = new SyntaxElement(); }
            |   expression          { $$ = $1; }
            ;

functionCall    :   identifier '(' argumentList ')' 
                    { 
                        PRINTF("Parser inserting function call (FUNCTION_CALL) in syntax tree\n");
                        $$ = new FunctionCall($1, $3);
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
                    $$ = new LabeledExpr(LabeledExpr::ARGUMENT, new RbDataType<string>(), $1);
                }
            |   identifier EQUAL expression
                { 
                    PRINTF("Parser inserting labeled argument (ARGUMENT) in syntax tree\n");
                    $$ = new LabeledExpr(LabeledExpr::ARGUMENT, $1, $3);
                }
            ;

functionDef :   FUNCTION '(' formalList ')' stmts
                {
                PRINTF("Parser inserting function definition (FUNCTION_DEF) in syntax tree\n");
                $$ = new FunctionDef($3, $5);
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
                    $$ = new LabeledExpr(LabeledExpr::FORMAL, new RbDataType<string>(), $1);
                }
            |   identifier EQUAL expression
                { 
                    PRINTF("Parser inserting labeled formal argument (FORMAL) in syntax tree\n");
                    $$ = new LabeledExpr(LabeledExpr::FORMAL, $1, $3);
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

statement   :   ifStatement     { $$ = $1; }
            |   forStatement    { $$ = $1; }
            |   whileStatement  { $$ = $1; }
            |   nextStatement   { $$ = $1; }
            |   breakStatement  { $$ = $1; }
            |   expression '\n' { $$ = $1; }
            |   expression ';'  { $$ = $1; }
            |   ';'             { $$ = new SyntaxElement(); }
            ;

ifStatement :   IF cond stmts               { $$ = new Statement(Statement::IF, $2, $3); }
            |   IF cond stmts ELSE stmts    { $$ = new Statement(Statement::IF_ELSE, $2, $3, $5); }

cond    :   '(' expression ')'    { $$ = new Condition($2); }
        ;

forStatement    :   FOR forCond stmts   { $$ = new Statement(Statement::FOR, $2, $3); }
                ;

forCond     :   '(' identifier IN expression ')'    { $$ = new ForCondition($2, $4); }
            ;

whileStatement  :   WHILE cond stmts    { $$ = new Statement(Statement::WHILE, $2, $3); }
                ;

nextStatement   :   NEXT    { $$ = new Statement(Statement::NEXT); }
                ;

breakStatement  :   BREAK   { $$ = new Statement(Statement::BREAK); }
                ;

identifier  :   NAME    { $$ = new RbDataType<string>($1); }
            ;

constant    :   FALSE   { $$ = new RbDataType<bool>(false); }
            |   TRUE    { $$ = new RbDataType<bool>(true); }
            |   NULL    { $$ = new RbDataType<int>(0); }
            |   INT     { $$ = new RbDataType<int>($1); }
            |   STRING  { $$ = new RbDataType<string>($1); }
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
                    RbDataType<double> *real = new RbDataType<double>($1);
                    real->setPrecision(prec);
                    real->setScientific(sci);
                    $$ = real;
                }
            ;

%%


/* Definition of yyerror. */
int yyerror(char *msg)
{
    PRINTF("Bison code said: %s\n", msg);
    return 0;     // or return 1 ??
}
