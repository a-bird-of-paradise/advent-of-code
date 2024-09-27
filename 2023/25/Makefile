INCLUDE=-IC:/ProgramData/win_flex_bison-2.5.25
LDFLAGS=
DBGFLAGS=-fsanitize=address -g -fsanitize=leak 
FASTFLAGS=-O3
CXXFLAGS=$(INCLUDE) $(FASTFLAGS) -std=c++2b 
LIBS=

SOURCES=scanner.cpp parser.cpp funcs.cpp main.cpp
OBJS=$(SOURCES:.cpp=.o)

.PHONY: answer

answer: aoc testinput.txt input.txt
	./aoc < testinput.txt
	./aoc < input.txt

aoc: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

scanner.cpp: scanner.l scanner.internal.hpp parser.hpp parser.cpp
	$(LEX) $<

parser.hpp parser.cpp: parser.y
	$(YACC) -v $^

.PHONY: clean

clean:
	rm -f scanner.cpp parser.cpp parser.hpp location.hh parser.output aoc
	rm -f $(OBJS)