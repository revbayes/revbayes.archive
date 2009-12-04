srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../ -I../primitive -I../../functions -I../../utils
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbString.o MethodDescr.o

RbString.o: RbString.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbString.cpp

MethodDescr.o: MethodDescr.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MethodDescr.cpp

clean: 
	rm -f *.o

