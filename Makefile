CXX=g++
MKDEPEND=gcc -MM
TARGETS=jnibind
TESTMSGPACK_SOURCES=\

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

dep: depend
depend:
	/bin/rm -f .depend
	@for fn in $(TESTMSGPACK_SOURCES); \
		do \
		echo "$(MKDEPEND) $(CXXFLAGS) $$fn >> .depend";\
		pathnm=`dirname $$fn`; \
		if [ -n $$pathnm -a '.' != $$pathnm ]; \
		then \
		echo -n $$pathnm/ >> .depend;\
		fi;\
		$(MKDEPEND) $(CXXFLAGS) $$fn >> .depend;\
		done;

-include .depend
