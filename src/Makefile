IDIR =../include
CC=gcc


ifeq ($(CC), gcc)
 $(info Compiling with GCC)
 CFLAGS= -std=c11 -O3 -I$(IDIR) #-Wall -Wextra -g
else ifeq ($(CC), clang)
 $(info Compiling with CLANG)
 CFLAGS= -std=c11 -O3 -I$(IDIR) #-Weverything
else
 $(info Compiler not recognized)
endif

ODIR=obj
TDIR=tests

DEPS = $(IDIR)

_OBJ = reader.o analyzer.o printer.o watchdog.o logger.o main.o buffer.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TDEFOBJ = reader.o analyzer.o printer.o watchdog.o logger.o buffer.o
TDEFOBJ = $(patsubst %,$(ODIR)/%,$(_TDEFOBJ))
TODIR = $(patsubst %,$(TDIR)/%,$(ODIR))
_TOBJ = test_reader.o test_analyzer.o test_printer.o test_watchdog.o test_logger.o test_buffer.o test_main.o
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