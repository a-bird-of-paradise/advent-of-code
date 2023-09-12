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

answer: 07 input.txt testinput.txt
	./07 < testinput.txt
	./07 < input.txt

07: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

scanner.cpp: 07.l scanner.internal.hpp parser.hpp parser.cpp
	$(FLEX) $<

scanner.hpp: scanner.internal.hpp

main.cpp: scanner.hpp

parser.hpp parser.cpp: 07.y
	$(BISON) -d -Wcounterexamples $^

.PHONY: clean

clean:
	rm -f 07.lex.cpp 07 parser.hpp parser.cpp
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)