srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../main -I../dag -I../datatypes -I../datatypes/complex -I../functions -I../utils -I../datatypes/primitive
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@
BISON       = bison
FLEX        = flex
SRCS        = grammar.y lex.l SyntaxElement.cpp SyntaxHelpExpr.cpp SyntaxConstant.cpp SyntaxLabeledExpr.cpp
OBJECTS     = grammar.tab.o lex.yy.o SyntaxElement.o SyntaxHelpExpr.o SyntaxConstant.o SyntaxLabeledExpr.o
BISONFLAGS  = -y -v

CFLAGS = $(CXXFLAGS)

all: grammar.tab.c lex.yy.cpp Environment.o SyntaxConstant.o SyntaxVariable.o

grammar.tab.c grammar.tab.h: grammar.y
	$(BISON) -o grammar.tab.c --defines $(BISONFLAGS) grammar.y

lex.yy.cpp: lex.l
	$(FLEX) -olex.yy.cpp lex.l

Environment.o: Environment.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Environment.cpp

SyntaxHelpExpr.o: SyntaxHelpExpr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxHelpExpr.cpp

SyntaxConstant.o: SyntaxConstant.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxConstant.cpp

SyntaxVariable.o: SyntaxVariable.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxVariable.cpp

SyntaxLabeledExpr.o: SyntaxLabeledExpr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxLabeledExpr.cpp

clean: 
	rm -f *.o lex.yy.cpp grammar.tab.c grammar.tab.h grammar.output


