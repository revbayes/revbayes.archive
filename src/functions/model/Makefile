srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../ -I../../parser -I../../dag -I../../datatypes -I../../datatypes/complex -I../../utils   -I../../datatypes/model
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: Func_model.o

Func_model.o: Func_model.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Func_model.cpp

clean: 
	rm -f *.o

