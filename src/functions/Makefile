srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../parser -I../dag -I../datatypes -I../datatypes/complex -I../utils -I../main -I../distributions -I../datatypes/primitive
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: Argument.o ArgumentRule.o MinmaxRule.o Ellipsis.o RbFunction.o UserFunction.o MemberFunction.o ConstructorFunction.o DistributionFunction.o

Argument.o: Argument.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Argument.cpp

ArgumentRule.o: ArgumentRule.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ArgumentRule.cpp

MinmaxRule.o: MinmaxRule.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MinmaxRule.cpp

Ellipsis.o: Ellipsis.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Ellipsis.cpp

RbFunction.o: RbFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbFunction.cpp

UserFunction.o: UserFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c UserFunction.cpp

MemberFunction.o: MemberFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MemberFunction.cpp

ConstructorFunction.o: ConstructorFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ConstructorFunction.cpp

DistributionFunction.o: DistributionFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c DistributionFunction.cpp

clean: 
	rm -f *.o

