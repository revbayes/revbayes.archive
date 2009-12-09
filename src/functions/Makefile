srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../parser -I../dag -I../datatypes -I../datatypes/complex -I../utils
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: Argument.o ArgumentRule.o RbFunction.o

ArgumentRule.o: ArgumentRule.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ArgumentRule.cpp

Argument.o: Argument.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Argument.cpp

RbFunction.o: RbFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbFunction.cpp

UserFunction.o: UserFunction.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c UserFunction.cpp

clean: 
	rm -f *.o

