srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../ -I../ -I../../datatypes -I../../functions -I../../distributions -I../../datatypes/complex -I../../main -I../../utils -I../../dag -I../../datatypes/primitive -I../../datatypes/model -I../../parser
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: Move_mscale.o Move_mslide.o

Move_mscale.o: Move_mscale.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Move_mscale.cpp

Move_mslide.o: Move_mslide.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Move_mslide.cpp

clean: 
	rm -f *.o

