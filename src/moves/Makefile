srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../datatypes -I../functions -I../distributions -I../datatypes/complex -I../main -I../utils -I../dag -I../datatypes/primitive -I../datatypes/model
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: MoveSchedule.o

MoveSchedule.o: MoveSchedule.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c MoveSchedule.cpp

clean: 
	rm -f *.o

