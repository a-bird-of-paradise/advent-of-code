FLEX=/usr/local/opt/flex/bin/flex
BISON=/usr/local/opt/bison/bin/bison

INCLUDE=-I/usr/local/opt/flex/include/ -I/usr/local/opt/llvm/include
LDFLAGS=-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++
DBGFLAGS=-fsanitize=address -g -fsanitize=leak 
FASTFLAGS=-Ofast -flto=full
CXXFLAGS=$(FASTFLAGS) -std=c++2b
CFLAGS=$(FASTFLAGS) -std=c99 -march=native
LIBS=

CC=/usr/local/opt/llvm/bin/clang
CXX=/usr/local/opt/llvm/bin/clang++

SOURCES=lex.yy.c valves.c main.c
OBJS=$(SOURCES:.c=.o)

.PHONY: answer

answer: 16 input.txt testinput.txt
	./16 < testinput.txt
	./16 < input.txt

16: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

%.o: %.c %.h Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

lex.yy.c: 16.l Makefile 
	$(FLEX) $<

16.tab.c 16.tab.h: 16.y Makefile
	$(BISON) -Wcounterexamples -d $<

.PHONY: clean

clean:
	rm -f lex.yy.c 16 16.tab.h 16.tab.c
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)
