srcdir      = .
RM          = rm
CXX         = g++
INCLUDES    = -I. -I../main -I../dag -I../datatypes -I../datatypes/complex -I../functions -I../utils -I../datatypes/primitive
CXXFLAGS    = -O3 -ffast-math -Wall
BISON       = bison
FLEX        = flex
BISONFLAGS  = -y -v

all: grammar.tab.c lex.yy.cpp Frame.o SyntaxConstant.o SyntaxVariable.o SyntaxBinaryExpr.o SyntaxUnaryExpr.o SyntaxFunctionCall.o SyntaxLabeledExpr.o SyntaxFormal.o

grammar.tab.c grammar.tab.h: grammar.y
	$(BISON) -o grammar.tab.c --defines $(BISONFLAGS) grammar.y

lex.yy.cpp: lex.l
	$(FLEX) -olex.yy.cpp lex.l

Frame.o: Frame.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Frame.cpp

SyntaxConstant.o: SyntaxConstant.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxConstant.cpp

SyntaxVariable.o: SyntaxVariable.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxVariable.cpp

SyntaxBinaryExpr.o: SyntaxBinaryExpr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxBinaryExpr.cpp

SyntaxUnaryExpr.o: SyntaxUnaryExpr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxUnaryExpr.cpp

SyntaxFunctionCall.o: SyntaxFunctionCall.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxFunctionCall.cpp

SyntaxLabeledExpr.o: SyntaxLabeledExpr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxLabeledExpr.cpp

SyntaxAssignExpr.o: SyntaxAssignExpr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxAssignExpr.cpp

SyntaxFunctionDef.o: SyntaxFunctionDef.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxFunctionDef.cpp

SyntaxFormal.o: SyntaxFormal.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxFormal.cpp

SyntaxClassDef.o: SyntaxClassDef.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxClassDef.cpp

SyntaxStatement.o: SyntaxStatement.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxStatement.cpp

SyntaxForCondition.o: SyntaxForCondition.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxForCondition.cpp

clean:
	rm -f *.o lex.yy.cpp grammar.tab.c grammar.tab.h grammar.output


