srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I. -I../datatypes -I../functions -I../distributions
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

all: DAGNode.o ConstantNode.o DeterministicNode.o StochasticNode.o

DAGNode.o: DAGNode.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c DAGNode.cpp

ConstantNode.o: ConstantNode.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ConstantNode.cpp

DeterministicNode.o: DeterministicNode.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c DeterministicNode.cpp

StochasticNode.o: StochasticNode.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c StochasticNode.cpp

clean: 
	rm -f *.o

