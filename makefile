objects = main.o interpreter.o functions.o hextodecimal.o

pyvm : $(objects)
	cc -o pyvm $(objects) -lm
main.o : programfiles/main.c headers/htod.h headers/functions.h
	cc -c programfiles/main.c
interpreter.o : programfiles/interpreter.c headers/include.h headers/functions.h
	cc -c programfiles/interpreter.c
functions.o : functions/functions.c headers/include.h headers/functions.h headers/htod.h
	    cc -c functions/functions.c
hextodecimal.o : functions/hextodecimal.c 
		cc -c functions/hextodecimal.c 
clean :
	rm pyvm $(objects)
