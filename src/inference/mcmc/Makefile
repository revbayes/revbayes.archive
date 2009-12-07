srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../../datatypes -I../../functions -I../../distributions -I../../datatypes/complex -I../../moves -I../../main -I../../utils -I../../moves/objects -I../../dag -I../../datatypes/primitive -I../../datatypes/model
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbMcmc.o

RbMcmc.o: RbMcmc.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbMcmc.cpp

clean: 
	rm -f *.o

