srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I. -I../parser -I../dag -I../datatypes -I../utils
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: ObjectSlot.o ArgumentRule.o Argument.o FunctionDef.o RbFunction.o UserFunction.o BuiltinFunction.o

ObjectSlot.o: ObjectSlot.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ObjectSlot.cpp

ArgumentRule.o: ArgumentRule.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ArgumentRule.cpp

Argument.o: Argument.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Argument.cpp

FunctionDef.o: FunctionDef.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c FunctionDef.cpp

RbFunction.o: RbFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbFunction.cpp

UserFunction.o: UserFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c UserFunction.cpp

BuiltinFunction.o: BuiltinFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c BuiltinFunction.cpp

clean: 
	rm -f *.o

