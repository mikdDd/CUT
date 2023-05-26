IDIR =../include
CC=gcc
CFLAGS= -std=c11 -O3 -I$(IDIR) -Wall -Wextra -g
CFLAGSTEST = -std=c11 -O3 -I../$(IDIR) -Wall -Wextra -g
ODIR=obj
TDIR=tests

DEPS = $(IDIR)

_OBJ = reader.o analyzer.o printer.o watchdog.o logger.o main.o data_buffer.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TDEFOBJ = reader.o analyzer.o printer.o watchdog.o logger.o data_buffer.o
TDEFOBJ = $(patsubst %,$(ODIR)/%,$(_TDEFOBJ))
TODIR = $(patsubst %,$(TDIR)/%,$(ODIR))
_TOBJ = test_reader.o test_main.o
TOBJ = $(patsubst %,$(TDIR)/$(ODIR)/%,$(_TOBJ))
TIDIR = $(patsubst %,%$(IDIR),../)

$(TODIR)/%.o: $(TDIR)/%.c $(IDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cut: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: test
test: $(TOBJ)  $(TDEFOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	./test

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f $(TODIR)/*.o *~ core $(INCDIR)/*~ 