FLEX=/usr/local/opt/flex/bin/flex

INCLUDE=-I/usr/local/opt/flex/include/ -I/usr/local/opt/llvm/include
LDFLAGS=-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++
DBGFLAGS=-fsanitize=address -g -fsanitize=leak 
FASTFLAGS=-Ofast -flto=full
CXXFLAGS=$(FASTFLAGS) -std=c++2b
CFLAGS=$(FASTFLAGS) -std=c99
LIBS=

CC=/usr/local/opt/llvm/bin/clang
CXX=/usr/local/opt/llvm/bin/clang++

SOURCES=03.lex.cpp main.cpp
OBJS=$(SOURCES:.cpp=.o)

.PHONY: answer

answer: 03 input.txt testinput.txt
	./03 < testinput.txt
	./03 < input.txt

03: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

03.lex.cpp: 03.l scanner.internal.hpp
	$(FLEX) $<

scanner.hpp: scanner.internal.hpp

main.cpp: scanner.hpp

.PHONY: clean

clean:
	rm -f 03.lex.cpp 03
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)