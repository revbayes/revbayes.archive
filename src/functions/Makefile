srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../dag -I../datatypes -I../utils
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: ObjectSlot.o ArgumentRule.o FunctionDef.o

ObjectSlot.o: ObjectSlot.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ObjectSlot.cpp

ArgumentRule.o: ArgumentRule.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ArgumentRule.cpp

FunctionDef.o: FunctionDef.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c FunctionDef.cpp

clean: 
	rm -f *.o

