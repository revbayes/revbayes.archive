srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../ -I../../datatypes/primitive -I../../dag -I../../utils -I../../datatypes/complex -I../../parser -I../../datatypes -I../../functions -I../../moves/simple/ -I../../moves
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     = -lreadline -lm -lxml2 -lpthread

CFLAGS = $(CXXFLAGS)

all: Dist_norm.o Dist_exp.o Dist_unif.o

Dist_norm.o: Dist_norm.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Dist_norm.cpp

Dist_exp.o: Dist_exp.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Dist_exp.cpp

Dist_unif.o: Dist_unif.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Dist_unif.cpp

clean: 
	rm -f *.o

