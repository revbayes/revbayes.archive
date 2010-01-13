srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../../datatypes -I../../functions -I../../distributions -I../../datatypes/complex -I../ -I../../main -I../../utils -I../../dag -I../../datatypes/primitive -I../../datatypes/model
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbMove.o MoveScale.o MoveSlidingWindow.o

RbMove.o: RbMove.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbMove.cpp

MoveScale.o: MoveScale.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MoveScale.cpp

MoveSlidingWindow.o: MoveSlidingWindow.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MoveSlidingWindow.cpp

clean: 
	rm -f *.o

