srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../ -I../primitive -I../../functions -I../../utils
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbString.o StringVector.o RbComplex.o

RbString.o: RbString.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbString.cpp

StringVector.o: StringVector.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c StringVector.cpp

RbComplex.o: RbComplex.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbComplex.cpp

clean: 
	rm -f *.o

