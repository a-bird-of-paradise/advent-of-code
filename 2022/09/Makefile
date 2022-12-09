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

SOURCES=lex.yy.c
OBJS=$(SOURCES:.c=.o)

.PHONY: answer

answer: 09 input.txt testinput.txt testinput2.txt
	./09 < testinput.txt
	./09 < testinput2.txt
	./09 < input.txt

09: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

lex.yy.c: 09.l
	$(FLEX) $<

.PHONY: clean

clean:
	rm -f lex.yy.c 09
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)