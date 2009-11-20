srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../ -I../datatypes -I../utils
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: ObjectSlot.o ArgRule.o

ObjectSlot.o: ObjectSlot.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ObjectSlot.cpp

ArgRule.o: ArgRule.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ArgRule.cpp

clean: 
	rm -f *.o

