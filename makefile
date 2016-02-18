P3.out: Project3.o Lex.o Syn.o Interpreter.o Symbol.o SetLimits.o
	g++ -o P3.out Project3.o Lex.o Syn.o Interpreter.o Symbol.o SetLimits.o

Project3.o: Project3.cpp Lex.h Syn.h
	g++ -c Project3.cpp -g

Lex.o: Lex.cpp Lex.h
	g++ -c Lex.cpp -g

Syn.o: Syn.h Syn.cpp Lex.h Interpreter.h
	g++ -c Syn.cpp -g

Interpreter.o: Interpreter.h Interpreter.cpp Lex.h Symbol.h
	g++ -c Interpreter.cpp -g

Symbol.o: Symbol.cpp Symbol.h OpCodes.h Interpreter.h
	g++ -c Symbol.cpp -g

SetLimits.o: SetLimits.cpp SetLimits.h
	g++ -c SetLimits.cpp

clean:	
	rm *.o P3.out *.lst* *.dbg* *.sym*

run: ./P3.out test1.in 
	clear
	./P3.out test1.in

submit:
	rm -rf fewellP3
	rm *.o P3.out *.lst* *.dbg* *.sym*
	mkdir fewellP3
	cp Project3.cpp Lex.* Syn.* Interpreter.* Symbol.* OpCodes.h SetLimits.* makefile fewellP3
	tar -czvf fewellP3.tgz fewellP3
	cp fewellP3.tgz ~tiawatts/cs460drop

