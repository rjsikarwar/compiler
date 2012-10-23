a.out: lex.yy.c SymTable.h
	@echo "Compiling lexer .... "
	@gcc lex.yy.c

lex.yy.c: lex.l
	@echo "Creating lexical analyzer .... "
	@lex lex.l

clean:
	@echo "Cleaning .... "
	@rm -rf lex.yy.c

purge: clean
	@echo "Purging .... "
	@rm -rf a.out
