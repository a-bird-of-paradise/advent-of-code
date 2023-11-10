INCLUDE=-I/usr/local/opt/flex/include/ -I/usr/local/opt/llvm/include
LDFLAGS=-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++
DBGFLAGS=-fsanitize=address -g -fsanitize=leak 
FASTFLAGS=-Ofast -flto=full
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
	rm -f scanner.cpp aoc parser.hpp parser.cpp location.hh
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)