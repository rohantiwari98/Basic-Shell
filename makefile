CC=gcc
CFLAGS=-I
DEPS=commandline.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS).

commandline: commandline.o commandlinefunctions.o stringmanip.o
	gcc -o commandline commandline.o commandlinefunctions.o stringmanip.o -I.
