FLEX=/usr/local/opt/flex/bin/flex
BISON=/usr/local/opt/bison/bin/bison

INCLUDE=-I/usr/local/opt/flex/include/ -I/usr/local/opt/llvm/include
LDFLAGS=-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++
DBGFLAGS=-fsanitize=address -g -fsanitize=leak -fsanitize=undefined -fsanitize=integer
FASTFLAGS=-Ofast -flto=full
CXXFLAGS=$(FASTFLAGS) -std=c++2b
CFLAGS=$(FASTFLAGS) -std=c99 -march=native
LIBS=

CC=/usr/local/opt/llvm/bin/clang
CXX=/usr/local/opt/llvm/bin/clang++

SOURCES=21.tab.c lex.yy.c astfuncs.c main.c
OBJS=$(SOURCES:.c=.o)

.PHONY: answer

answer: 21 input.txt testinput.txt
	./21 < testinput.txt
	./21 < input.txt

21: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

%.o: %.c %.h Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

lex.yy.c: 21.l Makefile 
	$(FLEX) $<

21.tab.c 21.tab.h: 21.y Makefile
	$(BISON) -Wcounterexamples -d $<

.PHONY: clean

clean:
	rm -f lex.yy.c 21 21.tab.h 21.tab.c
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)
