srcdir      = .
RM          = rm
CXX         = g++
INCLUDES    = -I. -I../main -I../dag -I../datatypes -I../datatypes/complex -I../functions -I../utils -I../datatypes/primitive -I../distributions -I../ui
CXXFLAGS    = -O3 -ffast-math -Wall
BISON       = bison
FLEX        = flex
BISONFLAGS  = -y -v

all: grammar.tab.c lex.yy.cpp ObjectSlot.o Frame.o FunctionTable.o Workspace.o SyntaxElement.o SyntaxConstant.o SyntaxVariable.o SyntaxBinaryExpr.o SyntaxUnaryExpr.o SyntaxFunctionCall.o SyntaxLabeledExpr.o SyntaxAssignExpr.o SyntaxFormal.o SyntaxFunctionDef.o SyntaxClassDef.o SyntaxForCondition.o SyntaxStatement.o Parser.o grammar.tab.o lex.yy.o

grammar.tab.c grammar.tab.h: grammar.y
	$(BISON) -o grammar.tab.c --defines $(BISONFLAGS) grammar.y

lex.yy.cpp: lex.l
	$(FLEX) -olex.yy.cpp lex.l

grammar.tab.o: grammar.tab.c
	g++ $(INCLUDES) $(CXXFLAGS) -c grammar.tab.c
    
lex.yy.o: lex.yy.cpp grammar.tab.h
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c lex.yy.cpp
    
ObjectSlot.o: ObjectSlot.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ObjectSlot.cpp

Frame.o: Frame.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Frame.cpp

FunctionTable.o: FunctionTable.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c FunctionTable.cpp

Workspace.o: Workspace.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Workspace.cpp

SyntaxElement.o: SyntaxElement.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxElement.cpp

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

SyntaxFormal.o: SyntaxFormal.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxFormal.cpp

SyntaxFunctionDef.o: SyntaxFunctionDef.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxFunctionDef.cpp

SyntaxClassDef.o: SyntaxClassDef.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxClassDef.cpp

SyntaxForCondition.o: SyntaxForCondition.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxForCondition.cpp

SyntaxStatement.o: SyntaxStatement.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c SyntaxStatement.cpp

Parser.o: Parser.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Parser.cpp

clean:
	rm -f *.o lex.yy.cpp grammar.tab.c grammar.tab.h grammar.output


