srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../ -I../complex -I../../utils -I../../functions
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbBool.o RbInt.o RbDouble.o

RbBool.o: RbBool.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbBool.cpp

RbInt.o: RbInt.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbInt.cpp

RbDouble.o: RbDouble.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbDouble.cpp

clean: 
	rm -f *.o

