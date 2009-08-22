srcdir      = .
RM          = rm
CXX         = g++
CC          = g++
INCLUDES    = -Incl -I../libxml2/include
CXXFLAGS    = -O3 -ffast-math -Wall $(INCLUDES)
LDFLAGS     =  -lreadline -lm -lxml2 -lpthread
LIBS        = -lreadline -lm 
INSTALL     = @INSTALL@
BISON       = bison
FLEX        = flex
SRCS        = grammar.y lex.l
OBJECTS     = grammar.tab.o lex.yy.o
BISONFLAGS  = -y -v

CFLAGS = $(CXXFLAGS)

all: grammar.tab.c lex.yy.cpp

grammar.tab.c grammar.tab.h: grammar.y
	$(BISON) -o grammar.tab.c --defines $(BISONFLAGS) grammar.y

lex.yy.cpp: lex.l
	$(FLEX) -olex.yy.cpp lex.l

clean: 
	rm -f *.o lex.yy.cpp grammar.tab.c grammar.tab.h


