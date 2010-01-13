srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./
CXXFLAGS    = -O3 -ffast-math -Wall

all: UserInterface.o

UserInterface.o: UserInterface.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c UserInterface.cpp

clean: 
	rm -f *.o

