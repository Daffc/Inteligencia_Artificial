# DOUGLAS AFFONSO CLEMENTINO
# GRR20175877

EXECS  = floodit
 
# arquivos-objeto
objects = floodit.o

all:  $(EXECS)

floodit: floodit.o 
	gcc floodit.o  -o $@

floodit.o : floodit.c
	gcc -c floodit.c

clean:
	-rm $(objects)
	-rm *~

purge: clean
	-rm $(EXECS)
