CC=gcc
C++=g++
LD=g++
CFLAGS=-c -Wall -pedantic-errors -O3 -std=gnu99 -I../
LDFLAGS=
OBJECTS=ex2client.o ../exp1lib.o
EXECUTABLE=ex2client

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(C++) $(CFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f ${EXECUTABLE} ${EXECUTABLE}.exe ${OBJECTS} core
