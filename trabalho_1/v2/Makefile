# DOUGLAS AFFONSO CLEMENTINO
# GRR20175877
CC = gcc
OBJDIR = obj
EXEC = floodit
FLAGS = -g
MKDIR_P = mkdir -p
 
# arquivos-objeto
objects = obj/floodit.o obj/GameFunctions.o obj/Graph.o

.PHONY: clean purge

all: ${OBJDIR} $(EXEC)

$(EXEC): $(objects)
	$(CC) $(objects) -o $@ $(FLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) -c $< -o $@ $(FLAGS)

${OBJDIR}:
	-mkdir $@

clean:
	-rm -r *.o
	-rm -r obj
	-rm *~

purge: clean
	-rm $(EXEC)
