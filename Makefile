CXX=g++
MKDEPEND=gcc -MM
TARGETS=jnibind

CXXFLAGS= -lfl -g
LIBS=

all: grammer 

install: 
	cp $(TARGETS) /usr/local/bin

grammer: 
	flex jnibind.l
	bison -d jnibind.y
	$(CXX) lex.yy.c jnibind.tab.c -g -o $(TARGETS) $(CXXFLAGS)

clean:
	rm -f $(TARGETS)
	rm -f cscope.* tags .depend core* testbind *.tab.c *.tab.h lex.yy.c Bind*.cpp Bind*.h Bind*.java
