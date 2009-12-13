srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../ -I../primitive -I../../functions -I../../utils -I../internal -I../../parser -I../../dag
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: MethodDescr.o MethodTable.o RbComplex.o RbString.o StringVector.o IntVector.o Vector.o Simplex.o

MethodDescr.o: MethodDescr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MethodDescr.cpp

MethodTable.o: MethodTable.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MethodTable.cpp

RbComplex.o: RbComplex.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbComplex.cpp

RbString.o: RbString.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbString.cpp

StringVector.o: StringVector.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c StringVector.cpp

IntVector.o: IntVector.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c IntVector.cpp

Vector.o: Vector.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Vector.cpp

Simplex.o: Simplex.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Simplex.cpp

clean: 
	rm -f *.o

