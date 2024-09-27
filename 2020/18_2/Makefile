INCLUDE=-IC:/ProgramData/win_flex_bison-2.5.25
LDFLAGS=
DBGFLAGS=-fsanitize=address -g -fsanitize=leak 
FASTFLAGS=-O3
CXXFLAGS=$(INCLUDE) $(FASTFLAGS) -std=c++2b 
LIBS=

SOURCES=Scanner.cpp Parser.cpp astnode.cpp main.cpp Context.cpp
OBJS:=$(addprefix build/, $(SOURCES:.cpp=.o))
DEPS:=$(OBJS:.o=.d)

default: cppcalc input.txt testinput.txt
	./cppcalc < testinput.txt
	./cppcalc < input.txt

cppcalc: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(LDFLAGS) $^ -o $@	

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
	
build/%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

Parser.hpp Parser.cpp: cppcalc.y
	$(YACC) -Wcounterexamples -d $^

Scanner.cpp:	cppcalc.l Parser.hpp Parser.cpp
	$(LEX) $<

%.txt:	%.raw.txt
	cat $< | sed 's/$$/;/g' > $@

.PHONY: clean

clean:
	rm -f location.hh cppcalc Scanner.cpp Parser.hpp Parser.cpp
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -f testinput.txt input.txt


-include $(DEPS)