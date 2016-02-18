P03.out : proj3.o lex.o setLimits.o syntactic.o interpreter.o symbol.o
	g++ -g -o P03.out proj3.o lex.o setLimits.o syntactic.o interpreter.o symbol.o

proj3.o : Project3.cpp
	g++ -g -c -o proj3.o Project3.cpp

lex.o : lex.h lex.cpp
	g++ -g -c lex.cpp

setLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c  -o setLimits.o SetLimits.cpp

syntactic.o : syntactic.h syntactic.cpp
	g++ -g -c  -o syntactic.o syntactic.cpp

interpreter.o : Interpreter.cpp Interpreter.h OpCodes.h
	g++ -g -c -o interpreter.o Interpreter.cpp

symbol.o : Interpreter.h Symbol.h Symbol.cpp OpCodes.h
	g++ -g -c -o symbol.o Symbol.cpp


clean : 
	rm *.o *.out *.lst *.dbg *.sym
	rm -rf ./kuehnleP3

submit :	
	rm -rf ./kuehnleP3
	mkdir kuehnleP3
	cp lex.* SetLimits.* syntactic.* Project3.cpp makefile kuehnleP3
	tar -czvf kuehnleP3.tgz kuehnleP3
	cp kuehnleP3.tgz ~tiawatts/cs460drop