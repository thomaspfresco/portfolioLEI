EXE = meta2
FILE1 = gocompiler.l
FILE2 = gocompiler.y



exe: ${FILE1} ${FILE2}
	lex ${FILE1}
	yacc -y -d -t -v ${FILE2}
	cc -o ${EXE} y.tab.c lex.yy.c functions.c
	
clear:
	rm -rf *${EXE}
