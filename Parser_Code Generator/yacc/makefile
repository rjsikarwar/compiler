a.out: lex.yy.c y.tab.c symtable.h
	@echo "Compiling parser .... "
	@gcc lex.yy.c y.tab.c

lex.yy.c: prog.l
	@echo "Creating lexical analyzer .... "
	@lex prog.l

y.tab.c: prog.y
	@echo "Creating parser .... "
	@yacc -d prog.y

clean:
	@echo "Cleaning .... "
	@rm -rf lex.yy.c lex.yy.o y.tab.c y.tab.h y.tab.o a.out
