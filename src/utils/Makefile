srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../datatypes -I../datatypes/complex
CXXFLAGS    = -O3 -ffast-math -Wall


all: RbException.o

RbException.o: RbException.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbException.cpp

clean: 
	rm -f *.o

