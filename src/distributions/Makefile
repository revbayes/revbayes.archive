srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../parser -I../dag -I../datatypes -I../datatypes/complex -I../utils -I../main -I../datatypes/primitive -I../functions
CXXFLAGS    = -O3 -ffast-math -Wall
LDFLAGS     = -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@

CFLAGS = $(CXXFLAGS)

all: Distribution.o DistributionReal.o

Distribution.o: Distribution.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Distribution.cpp

DistributionReal.o: DistributionReal.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c DistributionReal.cpp

clean: 
	rm -f *.o

