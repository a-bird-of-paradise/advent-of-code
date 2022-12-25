FLEX=/usr/local/opt/flex/bin/flex
BISON=/usr/local/opt/bison/bin/bison

INCLUDE=-I/usr/local/opt/flex/include/ -I/usr/local/opt/llvm/include
LDFLAGS=-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++
DBGFLAGS=-fsanitize=address -g -fsanitize=leak -fsanitize=undefined -fsanitize=integer
FASTFLAGS=-Ofast -flto=full
CXXFLAGS=$(DBGFLAGS) -std=c++2b
CFLAGS=$(DBGFLAGS) -std=c99 -march=native
LIBS=

CC=/usr/local/opt/llvm/bin/clang
CXX=/usr/local/opt/llvm/bin/clang++

SOURCES=lex.yy.c
OBJS=$(SOURCES:.c=.o)

.PHONY: answer

answer: 25 input.txt testinput.txt
	./25 < testinput.txt
	./25 < input.txt

25: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

%.o: %.c %.h Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

lex.yy.c lex.yy.h: 25.l Makefile 
	$(FLEX) $<

25.tab.c 25.tab.h: 25.y Makefile
	$(BISON) -Wcounterexamples -d $<

.PHONY: clean

clean:
	rm -f lex.yy.c 25 25.tab.h 25.tab.c
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)
