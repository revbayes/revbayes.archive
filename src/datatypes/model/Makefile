srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../ -I../primitive -I../../functions -I../../utils -I../internal -I../../parser -I../../dag -I../../main -I../complex
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: Model.o

Model.o: Model.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Model.cpp

clean: 
	rm -f *.o

