CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include
# what dos this do ^^


# This avoids error: ‘fileno’ was not declared in this scope
CPPFLAGS += -std=c++0x
# Avoid warnings about yyunput not used
CPPFLAGS += -Wno-unused-function

all : bin/c_compiler

# Bison
src/c_parser.tab.cpp src/c_parser.tab.hpp : src/c_parser.y
	bison -v -d src/c_parser.y -o src/c_parser.tab.cpp

# Flex
src/c_lexer.yy.cpp : src/c_lexer.flex src/c_parser.tab.hpp
	flex -o src/c_lexer.yy.cpp  src/c_lexer.flex


bin/c_compiler : src/c_compiler.o src/c_parser.tab.o src/c_lexer.yy.o src/c_parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/c_compiler $^

clean :
	rm src/*.o
	rm src/*.tab.cpp
	rm src/*.yy.cpp
	rm -r -f tmp/
	rm -r -f bin/
