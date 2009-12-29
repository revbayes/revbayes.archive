srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../ -I../primitive -I../../functions -I../../utils -I../internal -I../../parser -I../../dag
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbComplex.o Container.o RbString.o StringVector.o IntVector.o Vector.o Simplex.o MemberObject.o

RbComplex.o: RbComplex.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbComplex.cpp

Container.o: Container.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Container.cpp

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

MemberObject.o: MemberObject.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MemberObject.cpp

clean: 
	rm -f *.o

