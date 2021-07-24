# DOUGLAS AFFONSO CLEMENTINO
# GRR20175877
CC = gcc
OBJDIR = obj
EXEC = floodit
MKDIR_P = mkdir -p
 
# arquivos-objeto
objects = obj/floodit.o obj/GameFunctions.o

.PHONY: clean purge

all: ${OBJDIR} $(EXEC)

$(EXEC): $(objects)
	$(CC) $(objects) -o $@

$(OBJDIR)/%.o: %.c
	$(CC) -c $< -o $@

${OBJDIR}:
	-mkdir $@

clean:
	-rm -f *.o 
	-rm -rf obj
	-rm -f *~

purge: clean
	-rm -f $(EXEC)
