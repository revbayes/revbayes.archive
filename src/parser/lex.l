%{
/*
 * \file
 * Lexicographical analysis for the REvBayes parser.
 *
 * \brief Lexicographical analysis using flex
 *
 * \author Fredrik Ronquist
 * \date Last modified: $Date$
 *
 * $Id$
 */

/* The following statements go into the resulting cpp code */
#define YY_INPUT rrinput
#include <cstring>

#ifdef WIN32
#define strdup(a) _strdup(a)
#define yywrap() 1
#else
extern "C" { 
   char *strdup(const char *s);
}
#endif

/* include objects needed to understand grammar.tab.h */
#include "SyntaxElement.h"

#include "grammar.tab.h"

#ifdef DEBUG_LEX
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
ID       [a-zA-Z][a-zA-Z0-9_\-\.]*
STRING   \x22[^"]*\x22
OPTSIGN  [-+]?        
COMMENT  \x23.*

%%
function                                {
                                            PRINTF("Found 'function'\n");
                                            return FUNCTION;
                                        }
for                                     {
                                            PRINTF("Found 'for'\n");
                                            return FOR;
                                        }
in                                      {
                                            PRINTF("Found 'in'\n");
                                            return IN;
                                        }
if                                      {
                                            PRINTF("Found 'if'\n");
                                            return IF;
                                        }
else                                    {
                                            PRINTF("Found 'else'\n");
                                            return ELSE;
                                        }
while                                   {
                                            PRINTF("Found 'while'\n");
                                            return WHILE;
                                        }
next                                    {
                                            PRINTF("Found 'next'\n");
                                            return NEXT;
                                        }
break                                   {
                                            PRINTF("Found 'break'\n");
                                            return BREAK;
                                        }
null                                    {
                                            PRINTF("Found 'null'\n");
                                            return NULL;
                                        }
false                                   {
                                            PRINTF("Found 'false'\n");
                                            return FALSE;
                                        }
true                                    {
                                            PRINTF("Found 'true'\n");
                                            return TRUE;
                                        }
NULL                                    {
                                            PRINTF("Found 'NULL'\n");
                                            return NULL;
                                        }
FALSE                                   {
                                            PRINTF("Found 'FALSE'\n");
                                            return FALSE;
                                        }
TRUE                                    {
                                            PRINTF("Found 'TRUE'\n");
                                            return TRUE;
                                        }
{OPTSIGN}{DIGIT}+"."{DIGIT}*({EXP})?    {
                                            PRINTF("Found real %s\n", yytext);
                                            yylval.realValue = strtod(yytext,NULL); 
                                            PRINTF("double value = %f\n",yylval.realValue);
                                            return REAL;
                                        }
{OPTSIGN}{DIGIT}*"."{DIGIT}+({EXP})?    {
                                            PRINTF("Found real %s\n", yytext);
                                            yylval.realValue = strtod(yytext,NULL); 
                                            PRINTF("double value = %f\n",yylval.realValue);
                                            return REAL;
                                        }
{OPTSIGN}{DIGIT}+{EXP}                  { 
                                            /* Treat ints with exponents as floating point values */
                                            PRINTF("Found int with exponent %s\n", yytext);
                                            yylval.realValue = strtod(yytext,NULL); 
                                            PRINTF("double value = %f\n",yylval.realValue);
                                            return REAL;
                                        }
{OPTSIGN}{DIGIT}+                       {
                                            PRINTF("Found int %s\n", yytext);
                                            yylval.intValue = strtol(yytext,NULL,10); 
                                            PRINTF("int value = %d\n",yylval.intValue);
                                            return INT;
                                        }
{ID}                                    {
                                            PRINTF("Found id (name) %s\n", yytext);
                                            yylval.string = strdup(yytext);
                                            PRINTF("string value = %s\n", yylval.string);
                                            return NAME;
                                        }
{STRING}                                {
                                            PRINTF("Found string %s\n", yytext); 
                                            yylval.string = strdup(yytext);
                                            PRINTF("string value = %s\n", yylval.string);
                                            return STRING;
                                        }
{COMMENT}                               {
                                            PRINTF("Found comment %s\n", yytext);
                                            return COMMENT;
                                        }
"<-"                                    {
                                            PRINTF("Found '<-'\n");
                                            return LEFT_ASSIGN;
                                        }
":="                                    {
                                            PRINTF("Found ':='\n");
                                            return EQ_ASSIGN;
                                        }
"~"                                     {
                                            PRINTF("Found '~'\n");
                                            return TILDE;
                                        }
"="                                     {
                                            PRINTF("Found '='\n");
                                            return EQUAL;
                                        }
"&"                                     {
                                            PRINTF("Found '&'\n");
                                            return AND;
                                        }
"|"                                     {
                                            PRINTF("Found '|'\n");
                                            return OR;
                                        }
"&&"                                    {
                                            PRINTF("Found '&&'\n");
                                            return AND;
                                        }
"||"                                    {
                                            PRINTF("Found '||'\n");
                                            return OR;
                                        }
">"                                     {
                                            PRINTF("Found '>'\n");
                                            return GT;
                                        }
">="                                    {
                                            PRINTF("Found '>='\n");
                                            return GE;
                                        }
"<"                                     {
                                            PRINTF("Found '<'\n");
                                            return LT;
                                        }
"<="                                    {
                                            PRINTF("Found '<='\n");
                                            return LE;
                                        }
"=="                                    {
                                            PRINTF("Found '=='\n");
                                            return EQ;
                                        }
"!="                                    {
                                            PRINTF("Found '!='\n");
                                            return EQ;
                                        }
[ \t]+                                  {
                                            PRINTF ("Found white space\n");
                                            /* Ignore white space */
                                        }
"\r\n"                                  {
                                            PRINTF ("Found DOS newline\n");
                                            return '\n';
                                        }
"\r"                                    {
                                            PRINTF ("Found carriage return\n");
                                            return '\r';
                                        }
"\n"                                    {
                                            PRINTF ("Found newline\n");
                                            return yytext[0];
                                        }
.                                       {
                                            /* any character not matching existing rules */
                                            /* pass it onto the parser and let the parser handle it */
                                            PRINTF("Found char 0x%X ('%c')\n", yytext[0], yytext[0]);
                                            return yytext[0];
                                        }
%%
