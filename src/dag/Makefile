srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I./ -I../datatypes -I../functions -I../distributions -I../datatypes/complex -I../moves -I../main -I../utils -I../moves/objects
CXXFLAGS    = -O3 -ffast-math -Wall

CFLAGS = $(CXXFLAGS)

all: RbObjectWrapper.o DAGNode.o ConstantNode.o DeterministicNode.o StochasticNode.o DAGNodeContainer.o

RbObjectWrapper.o: RbObjectWrapper.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbObjectWrapper.cpp

DAGNode.o: DAGNode.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c DAGNode.cpp

ConstantNode.o: ConstantNode.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c ConstantNode.cpp

DeterministicNode.o: DeterministicNode.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c DeterministicNode.cpp

StochasticNode.o: StochasticNode.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c StochasticNode.cpp

DAGNodeContainer.o: DAGNodeContainer.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c DAGNodeContainer.cpp

clean: 
	rm -f *.o

