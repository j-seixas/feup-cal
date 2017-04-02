IDIR= ./
CC=g++
CFLAGS=-I$(IDIR) -Wall

ODIR= ./obj
LDIR= 

LIBS=

_DEPS=graph.h utilities.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

proj: $(OBJ)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o proj
