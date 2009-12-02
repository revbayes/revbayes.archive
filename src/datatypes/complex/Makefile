srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../ -I../primitive -I../../functions -I../../utils
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: RbString.o

RbString.o: RbString.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbString.cpp

clean: 
	rm -f *.o

