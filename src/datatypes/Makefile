srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../dag -I./complex -I../functions -I../utils
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: RbObject.o RbUndefined.o

RbObject.o: RbObject.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbObject.cpp

RbUndefined.o: RbUndefined.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbUndefined.cpp

clean: 
	rm -f *.o

