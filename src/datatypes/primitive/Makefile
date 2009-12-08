srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../ -I../complex -I../../utils -I../../functions
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbPrimitive.o RbBool.o RbInt.o RbDouble.o

RbPrimitive.o: RbPrimitive.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbPrimitive.cpp

RbBool.o: RbBool.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbBool.cpp

RbInt.o: RbInt.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbInt.cpp

RbDouble.o: RbDouble.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbDouble.cpp

clean: 
	rm -f *.o

