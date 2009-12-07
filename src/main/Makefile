srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../datatypes -I../functions -I../distributions -I../datatypes/complex -I../moves -I../dag -I../utils -I../moves/objects -I../datatypes/primitive
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbMonitor.o

RbMonitor.o: RbMonitor.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbMonitor.cpp

clean: 
	rm -f *.o

