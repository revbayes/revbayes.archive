srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -Incl -I../libxml2/include -I../main
CXXFLAGS    = -O3 -ffast-math -Wall $(INCLUDES)
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@
BISON       = bison
FLEX        = flex
SRCS        = grammar.y lex.l SyntaxElement.cpp SyntaxHelpExpr.cpp SyntaxConstant.cpp SyntaxLabeledExpr.cpp
OBJECTS     = grammar.tab.o lex.yy.o SyntaxElement.o SyntaxHelpExpr.o SyntaxConstant.o SyntaxLabeledExpr.o
BISONFLAGS  = -y -v

CFLAGS = $(CXXFLAGS)

all: grammar.tab.c lex.yy.cpp

grammar.tab.c grammar.tab.h: grammar.y
	$(BISON) -o grammar.tab.c --defines $(BISONFLAGS) grammar.y

lex.yy.cpp: lex.l
	$(FLEX) -olex.yy.cpp lex.l

SyntaxElement.o: SyntaxElement.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxElement.cpp

SyntaxHelpExpr.o: SyntaxHelpExpr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxHelpExpr.cpp

SyntaxConstant.o: SyntaxConstant.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxConstant.cpp

SyntaxLabeledExpr.o: SyntaxLabeledExpr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxLabeledExpr.cpp

clean: 
	rm -f *.o lex.yy.cpp grammar.tab.c grammar.tab.h grammar.output


