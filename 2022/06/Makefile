FLEX=/usr/local/opt/flex/bin/flex

INCLUDE=-I/usr/local/opt/flex/include/ -I/usr/local/opt/llvm/include
LDFLAGS=-L/usr/local/opt/llvm/lib -L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++
DBGFLAGS=-fsanitize=address -g -fsanitize=leak 
FASTFLAGS=-Ofast -flto=full
CXXFLAGS=$(DBGFLAGS) -std=c++2b
CFLAGS=$(DBGFLAGS) -std=c99
LIBS=

CC=/usr/local/opt/llvm/bin/clang
CXX=/usr/local/opt/llvm/bin/clang++

SOURCES=main.c
OBJS=$(SOURCES:.c=.o)

.PHONY: answer

answer: 06 input.txt testinput.txt
	./06 < testinput.txt
	./06 < input.txt

06: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

lex.yy.c: 06.l
	$(FLEX) $<

.PHONY: clean

clean:
	rm -f lex.yy.c 06
	rm -rf *.dSYM *.gcda *.gcno merge.out default.profraw
	rm -f $(OBJS)