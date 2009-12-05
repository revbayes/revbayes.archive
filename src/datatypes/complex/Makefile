srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../ -I../primitive -I../../functions -I../../utils
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbComplex.o RbString.o StringVector.o IntVector.o

RbComplex.o: RbComplex.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbComplex.cpp

RbString.o: RbString.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbString.cpp

StringVector.o: StringVector.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c StringVector.cpp

IntVector.o: IntVector.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c IntVector.cpp

clean: 
	rm -f *.o

