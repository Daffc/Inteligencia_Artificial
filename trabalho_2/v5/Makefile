CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline
EXEC =  jogador 

all: jogador

jogador: pilha.h pilha.c campo.h campo.c jogador.h jogador.c

clean:
	-rm -f *.o 
	-rm -f *~

purge: clean
	-rm -f $(EXEC)