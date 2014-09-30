%{
/**
 * @file
 * Lexicographical analysis for the Rev language.
 *
 * @brief Lexicographical analysis using flex
 *
 * @author Fredrik Ronquist and Sebastian Hoehna
 */

/* The following statements go into the resulting cpp code */

/* Define the call-back function for receiving input */
#define YY_INPUT rrinput

#include <cstring>

/* Hack for Windows */
#ifdef WIN32
#define strdup(a) _strdup(a)
#define fileno _fileno
#define YY_SKIP_YYWRAP
#define yywrap() 1
int isatty(int) { return 0; }
#else
extern "C" { 
   char *strdup(const char *s);
}
#define YY_SKIP_YYWRAP
#define yywrap() 1
#endif
    
    
#ifndef WIN32
#include <unistd.h> // for isatty()
#endif

/* Include objects needed to understand grammar.tab.h */
#include "RbOptions.h"     // To catch any parser debug flags
#include "Parser.h"
#include "RlString.h"
#include "SyntaxElement.h"
#include "SyntaxFormal.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxLabeledExpr.h"

#include "grammar.tab.h"

#define YY_NEVER_INTERACTIVE 1
extern void rrinput(char *,size_t &,size_t);
#define YY_NO_UNISTD_H

size_t yycolumn;

#define YY_USER_ACTION yycolumn += yyleng;

%}

DIGIT    [0-9] 
EXP      [Ee][+-]?[0-9]+
ID       [_]*[a-zA-Z][a-zA-Z0-9_]*
STRING   \x22[^"]*\x22
COMMENT  \x23.*

%%
function                                {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'function'\n");
#endif
                                            return FUNCTION;
                                        }
procedure                               {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'procedure'\n");
#endif
                                            return PROCEDURE;
                                        }
class                                   {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'class'\n");
#endif
                                            return CLASS;
                                        }
for                                     {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'for'\n");
#endif
                                            return FOR;
                                        }
in                                      {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'in'\n");
#endif
                                            return IN;
                                        }
if                                      {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'if'\n");
#endif
                                            return IF;
                                        }
else                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'else'\n");
#endif
                                            return ELSE;
                                        }
while                                   {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'while'\n");
#endif
                                            return WHILE;
                                        }
next                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'next'\n");
#endif
                                            return NEXT;
                                        }
break                                   {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'break'\n");
#endif
                                            return BREAK;
                                        }
return                                  {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'return'\n");
#endif
                                            return RETURN;
                                        }
null                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'null'\n");
#endif
                                            return RBNULL;
}
tab                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '\t'\n");
#endif
                                            return RBTAB;
                                        }
false                                   {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'false'\n");
#endif
                                            return FALSE;
                                        }
true                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'true'\n");
#endif
                                            return TRUE;
                                        }
NULL                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'NULL'\n");
#endif
                                            return RBNULL;
}
TAB                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'NULL'\n");
#endif
                                            return RBTAB;
}
FALSE                                   {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'FALSE'\n");
#endif
                                            return FALSE;
                                        }
TRUE                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'TRUE'\n");
#endif
                                            return TRUE;
                                        }
const                                   {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'const'\n");
#endif
                                            return MOD_CONST;
                                        }
dynamic                                 {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'dynamic'\n");
#endif
                                            return MOD_DYNAMIC;
                                        }
stochastic                              {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'stochastic'\n");
#endif
                                            return MOD_STOCHASTIC;
                                        }
deterministic                           {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'deterministic'\n");
#endif
                                            return MOD_DETERMINISTIC;
                                        }
protected                               {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found 'protected'\n");
#endif
                                            return PROTECTED;
                                        }
{DIGIT}+"."{DIGIT}*({EXP})?             {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found real %s -- ", yytext);
#endif
                                            yylval.realValue = strtod(yytext,NULL); 
#ifdef DEBUG_BISON_FLEX
                                            printf("double value = %f\n",yylval.realValue);
#endif
                                            return REAL;
                                        }
{DIGIT}*"."{DIGIT}+({EXP})?             {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found real %s -- ", yytext);
#endif
                                            yylval.realValue = strtod(yytext,NULL); 
#ifdef DEBUG_BISON_FLEX
                                            printf("double value = %f\n",yylval.realValue);
#endif
                                            return REAL;
                                        }
{DIGIT}+{EXP}                           { 
                                            /* Treat ints with exponents as floating point values */
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found int with exponent %s -- ", yytext);
#endif
                                            yylval.realValue = strtod(yytext,NULL); 
#ifdef DEBUG_BISON_FLEX
                                            printf("double value = %f\n",yylval.realValue);
#endif
                                            return REAL;
                                        }
{DIGIT}+                                {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found int %s -- ", yytext);
#endif
                                            yylval.intValue = int(strtol(yytext,NULL,10)); 
#ifdef DEBUG_BISON_FLEX
                                            printf("int value = %d\n",yylval.intValue);
#endif
                                            return INT;
                                        }
{ID}                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found id (name) %s -- ", yytext);
#endif
                                            yylval.c_string = yytext;
#ifdef DEBUG_BISON_FLEX
                                            printf("string value = %s\n", yylval.string);
#endif
                                            return NAME;
                                        }
{STRING}                                {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found string %s -- ", yytext);
#endif
                                            /* Strip quotes from string */
                                            yytext[strlen(yytext)-1] = '\0';
                                            yylval.c_string = yytext + 1;
#ifdef DEBUG_BISON_FLEX
                                            printf("string value = %s\n", yylval.string);
#endif
                                            return STRING;
                                        }
{COMMENT}                               {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found comment '%s', ignored\n", yytext);
#endif
                                            /* Ignore comment */
                                        }
"<-"                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '<-'\n");
#endif
                                            return ARROW_ASSIGN;
                                        }
":="                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found ':='\n");
#endif
                                            return EQUATION_ASSIGN;
                                        }
"~"                                     {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '~'\n");
#endif
                                            return TILDE_ASSIGN;
                                        }
"<<-"                                   {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '<<-'\n");
#endif
                                            return CONTROL_ASSIGN;
                                        }
"<-&"                                   {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '<-&'\n");
#endif
                                            return REFERENCE_ASSIGN;
                                        }
"+="                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '+='\n");
#endif
                                            return ADDITION_ASSIGN;
                                        }
"-="                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '-='\n");
#endif
                                            return SUBTRACTION_ASSIGN;
                                        }
"*="                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '*='\n");
#endif
                                            return MULTIPLICATION_ASSIGN;
                                        }
"/="                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '/='\n");
#endif
                                            return DIVISION_ASSIGN;
                                        }
"++"                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '++'\n");
#endif
                                            return INCREMENT;
                                        }
"--"                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '--'\n");
#endif
                                            return DECREMENT;
                                        }
"="                                     {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '='\n");
#endif
                                            return EQUAL;
                                        }
"&"                                     {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '&'\n");
#endif
                                            return AND;
                                        }
"|"                                     {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '|'\n");
#endif
                                            return OR;
                                        }
"&&"                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '&&'\n");
#endif
                                            return AND;
                                        }
"||"                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '||'\n");
#endif
                                            return OR;
                                        }
">"                                     {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '>'\n");
#endif
                                            return GT;
                                        }
">="                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '>='\n");
#endif
                                            return GE;
                                        }
"<"                                     {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '<'\n");
#endif
                                            return LT;
                                        }
"<="                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '<='\n");
#endif
                                            return LE;
                                        }
"=="                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '=='\n");
#endif
                                            return EQ;
                                        }
"!="                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found '!='\n");
#endif
                                            return NE;
                                        }
[ \t]+                                  {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found white space\n");
#endif
                                            /* Ignore white space */
                                        }
"\n"                                    {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found newline\n");
#endif
                                            foundNewline = true;
                                            return yytext[0];
                                        }
";"                                     {
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found ';'\n");
#endif
                                            return yytext[0];
                                        }
.                                       {
                                            /* any character not matching existing rules */
                                            /* pass it onto the parser and let the parser handle it */
#ifdef DEBUG_BISON_FLEX
                                            printf("Flex found char 0x%X ('%c')\n", yytext[0], yytext[0]);
#endif
                                            return yytext[0];
                                        }
%%
