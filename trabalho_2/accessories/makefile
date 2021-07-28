CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline
EXEC = controlador exemplo1 exemplo2

all: controlador exemplo1 exemplo2

controlador: controlador.c

exemplo1: campo.h campo.c exemplo1.c

exemplo2: campo.h campo.c exemplo2.c

clean:
	rm -f controlador exemplo1 exemplo2 *.o

clean:
	-rm -f *.o 
	-rm -rf obj
	-rm -f *~

purge: clean
	-rm -f $(EXEC)