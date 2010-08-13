%{
/**
 * @file
 * Lexicographical analysis for the RevBayes language.
 *
 * @brief Lexicographical analysis using flex
 *
 * @author Fredrik Ronquist
 * @date Last modified: $Date$
 *
 * $Id$
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

/* Include objects needed to understand grammar.tab.h */
#include "Parser.h"     // To catch any parser debug flags
#include "RbString.h"
#include "SyntaxElement.h"
#include "SyntaxFormal.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxLabeledExpr.h"

#include "grammar.tab.h"

#ifdef DEBUG_PARSER
#define PRINTF printf
#else
#define PRINTF(...)
#endif

#define YY_NEVER_INTERACTIVE 1
extern void rrinput(char *,int &,int);
#define YY_NO_UNISTD_H
%}

DIGIT    [0-9] 
EXP      [Ee][+-]?[0-9]+
ID       [a-zA-Z][a-zA-Z0-9_\-]*
STRING   \x22[^"]*\x22
OPTSIGN  [-+]?        
COMMENT  \x23.*

%%
function                                {
                                            PRINTF("Flex found 'function'\n");
                                            return FUNCTION;
                                        }
class                                   {
                                            PRINTF("Flex found 'class'\n");
                                            return CLASS;
                                        }
for                                     {
                                            PRINTF("Flex found 'for'\n");
                                            return FOR;
                                        }
in                                      {
                                            PRINTF("Flex found 'in'\n");
                                            return IN;
                                        }
if                                      {
                                            PRINTF("Flex found 'if'\n");
                                            return IF;
                                        }
else                                    {
                                            PRINTF("Flex found 'else'\n");
                                            return ELSE;
                                        }
while                                   {
                                            PRINTF("Flex found 'while'\n");
                                            return WHILE;
                                        }
next                                    {
                                            PRINTF("Flex found 'next'\n");
                                            return NEXT;
                                        }
break                                   {
                                            PRINTF("Flex found 'break'\n");
                                            return BREAK;
                                        }
return                                  {
                                            PRINTF("Flex found 'return'\n");
                                            return RETURN;
                                        }
null                                    {
                                            PRINTF("Flex found 'null'\n");
                                            return RBNULL;
                                        }
false                                   {
                                            PRINTF("Flex found 'false'\n");
                                            return FALSE;
                                        }
true                                    {
                                            PRINTF("Flex found 'true'\n");
                                            return TRUE;
                                        }
NULL                                    {
                                            PRINTF("Flex found 'NULL'\n");
                                            return RBNULL;
                                        }
FALSE                                   {
                                            PRINTF("Flex found 'FALSE'\n");
                                            return FALSE;
                                        }
TRUE                                    {
                                            PRINTF("Flex found 'TRUE'\n");
                                            return TRUE;
                                        }
{OPTSIGN}{DIGIT}+"."{DIGIT}*({EXP})?    {
                                            PRINTF("Flex found real %s -- ", yytext);
                                            yylval.realValue = strtod(yytext,NULL); 
                                            PRINTF("double value = %f\n",yylval.realValue);
                                            return REAL;
                                        }
{OPTSIGN}{DIGIT}*"."{DIGIT}+({EXP})?    {
                                            PRINTF("Flex found real %s -- ", yytext);
                                            yylval.realValue = strtod(yytext,NULL); 
                                            PRINTF("double value = %f\n",yylval.realValue);
                                            return REAL;
                                        }
{OPTSIGN}{DIGIT}+{EXP}                  { 
                                            /* Treat ints with exponents as floating point values */
                                            PRINTF("Flex found int with exponent %s -- ", yytext);
                                            yylval.realValue = strtod(yytext,NULL); 
                                            PRINTF("double value = %f\n",yylval.realValue);
                                            return REAL;
                                        }
{OPTSIGN}{DIGIT}+                       {
                                            PRINTF("Flex found int %s -- ", yytext);
                                            yylval.intValue = strtol(yytext,NULL,10); 
                                            PRINTF("int value = %d\n",yylval.intValue);
                                            return INT;
                                        }
{ID}                                    {
                                            PRINTF("Flex found id (name) %s -- ", yytext);
                                            yylval.string = yytext;
                                            PRINTF("string value = %s\n", yylval.string);
                                            return NAME;
                                        }
{STRING}                                {
                                            PRINTF("Flex found string %s -- ", yytext); 
                                            /* Strip quotes from string */
                                            yytext[strlen(yytext)-1] = '\0';
                                            yylval.string = yytext + 1;
                                            PRINTF("string value = %s\n", yylval.string);
                                            return STRING;
                                        }
{COMMENT}                               {
                                            PRINTF("Flex found comment %s\n", yytext);
                                            return COMMENT;
                                        }
"<-"                                    {
                                            PRINTF("Flex found '<-'\n");
                                            return ARROW_ASSIGN;
                                        }
":="                                    {
                                            PRINTF("Flex found ':='\n");
                                            return EQUATION_ASSIGN;
                                        }
"~"                                     {
                                            PRINTF("Flex found '~'\n");
                                            return TILDE_ASSIGN;
                                        }
"="                                     {
                                            PRINTF("Flex found '='\n");
                                            return EQUAL;
                                        }
"&"                                     {
                                            PRINTF("Flex found '&'\n");
                                            return AND;
                                        }
"|"                                     {
                                            PRINTF("Flex found '|'\n");
                                            return OR;
                                        }
"&&"                                    {
                                            PRINTF("Flex found '&&'\n");
                                            return AND;
                                        }
"||"                                    {
                                            PRINTF("Flex found '||'\n");
                                            return OR;
                                        }
">"                                     {
                                            PRINTF("Flex found '>'\n");
                                            return GT;
                                        }
">="                                    {
                                            PRINTF("Flex found '>='\n");
                                            return GE;
                                        }
"<"                                     {
                                            PRINTF("Flex found '<'\n");
                                            return LT;
                                        }
"<="                                    {
                                            PRINTF("Flex found '<='\n");
                                            return LE;
                                        }
"=="                                    {
                                            PRINTF("Flex found '=='\n");
                                            return EQ;
                                        }
"!="                                    {
                                            PRINTF("Flex found '!='\n");
                                            return EQ;
                                        }
[ \t]+                                  {
                                            PRINTF ("Flex found white space\n");
                                            /* Ignore white space */
                                        }
"\n"                                    {
                                            PRINTF ("Flex found newline\n");
                                            foundNewline = true;
                                            return yytext[0];
                                        }
.                                       {
                                            /* any character not matching existing rules */
                                            /* pass it onto the parser and let the parser handle it */
                                            PRINTF("Flex found char 0x%X ('%c')\n", yytext[0], yytext[0]);
                                            return yytext[0];
                                        }
%%
