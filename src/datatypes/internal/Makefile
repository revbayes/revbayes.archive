srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../ -I../complex -I../../functions -I../../utils -I../../parser
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: MethodDescr.o MethodTable.o 

MethodDescr.o: MethodDescr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MethodDescr.cpp

MethodTable.o: MethodTable.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MethodTable.cpp

clean: 
	rm -f *.o

