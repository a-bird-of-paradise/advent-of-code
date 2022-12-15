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

SOURCES=lex.yy.c main.c
OBJS=$(SOURCES:.c=.o)

.PHONY: answer

answer: 14 input.txt testinput.txt
	./14 < testinput.txt
	./14 < input.txt

14: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

%.o: %.c %.h Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

lex.yy.c: 14.l Makefile 
	$(FLEX) $<

14.tab.c 14.tab.h: 14.y Makefile
	$(BISON) -Wcounterexamples -d $<

.PHONY: clean

clean:
	rm -f lex.yy.c 14 14.tab.h 14.tab.c
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)