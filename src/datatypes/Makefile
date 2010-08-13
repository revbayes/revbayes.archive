srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../dag
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: RbObject.o

RbObject.o: RbObject.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbObject.cpp

clean: 
	rm -f *.o

