CC=g++
CFLAGS=-O4 -Wall -Wextra -Wno-unused-parameter -ggdb
TESTFLAGS=-lgtest -lgtest_main -pthread

SRCDIR=./src
OBJDIR=./objs
TESTDIR=./test

_TESTDEPS=test.cpp
TESTDEPS=$(patsubst %, $(TESTDIR)/%, $(_TESTDEPS))

_DEPS=mpnumber.h
DEPS=$(patsubst %, $(SRCDIR)/%, $(_DEPS))

_OBJ=mpnumber.o
OBJ=$(patsubst %, $(OBJDIR)/%, $(_OBJ))


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)
	ar cr libmplib.a $(OBJ)

test: $(TESTDEPS) $(OBJ)
	$(CC) -o $(TESTDIR)/test.elf $(TESTDIR)/test.cpp $(CFLAGS) $(OBJ) $(TESTFLAGS)
	./test/test.elf

clean:
	rm -rf $(OBJDIR)/* $(TESTDIR)/*.elf libmplib.a