srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../ -I../complex
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: RbBool.o

RbBool.o: RbBool.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbBool.cpp

clean: 
	rm -f *.o

