srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -I../../main -I../../utils
CXXFLAGS    = -O3 -ffast-math -Wall $(INCLUDES)
LDFLAGS     = -lm
LIBS        = -lm 
INSTALL     = @INSTALL@
SRCS        = Simplex.cpp RateMatrix.cpp RbLogic.cpp RbInt.cpp RbReal.cpp RbString.cpp RbSimplex.cpp RbRateMatrix.cpp RbAbstractDataType.cpp
OBJECTS     = Simplex.o RateMatrix.o RbLogic.o RbInt.o RbReal.o RbString.o RbSimplex.o RbRateMatrix.o RbAbstractDataType.o
BISONFLAGS  = -y -v

CFLAGS = $(CXXFLAGS)

all: $(OBJECTS)

Simplex.o: Simplex.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c Simplex.cpp

RateMatrix.o: RateMatrix.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RateMatrix.cpp

RbAbstractDataType.o: RbAbstractDataType.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbAbstractDataType.cpp

RbLogic.o: RbLogic.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbLogic.cpp

RbInt.o: RbInt.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbInt.cpp

RbReal.o: RbReal.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbReal.cpp

RbString.o: RbString.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbString.cpp

RbSimplex.o: RbSimplex.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbSimplex.cpp

RbRateMatrix.o: RbRateMatrix.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RbRateMatrix.cpp

clean: 
	rm -f *.o

