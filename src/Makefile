IDIR =../include
CC=gcc
CFLAGS= -std=c11 -O3 -I$(IDIR)

ODIR=obj


DEPS = $(IDIR)

_OBJ = reader.o analyzer.o printer.o watchdog.o logger.o main.o data_buffer.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cut: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 