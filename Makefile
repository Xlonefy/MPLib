CC=g++
CFLAGS=-O2 -Wall -Wextra

SRCDIR=./src
OBJDIR=./objs

_DEPS=mpnumber.h
DEPS=$(patsubst %, $(SRCDIR)/%, $(_DEPS))

_OBJ=mpnumber.o
OBJ=$(patsubst %, $(OBJDIR)/%, $(_OBJ))


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)
	ar cr libmplib.a $(OBJ)

clean:
	rm -rf $(OBJDIR)/*