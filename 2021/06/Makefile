FLEX=/usr/local/opt/flex/bin/flex
BISON=/usr/local/opt/bison/bin/bison

INCLUDE=-I/usr/local/opt/flex/include/ -I/usr/local/opt/llvm/include
LDFLAGS=-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++
DBGFLAGS=-fsanitize=address -g -fsanitize=leak 
FASTFLAGS=-Ofast -flto=full
CXXFLAGS=$(FASTFLAGS) -std=c++2b
CFLAGS=$(FASTFLAGS) -std=c99
LIBS=

CC=/usr/local/opt/llvm/bin/clang
CXX=/usr/local/opt/llvm/bin/clang++

SOURCES=scanner.cpp main.cpp parser.cpp
OBJS=$(SOURCES:.cpp=.o)

.PHONY: answer

answer: 06 input.txt testinput.txt
	./06 < testinput.txt
	./06 < input.txt

06: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

scanner.cpp: 06.l scanner.internal.hpp parser.hpp parser.cpp
	$(FLEX) $<

scanner.hpp: scanner.internal.hpp

main.cpp: scanner.hpp

parser.hpp parser.cpp: 06.y
	$(BISON) -d -Wcounterexamples $^

.PHONY: clean

clean:
	rm -f 06.lex.cpp 06 parser.hpp parser.cpp
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)