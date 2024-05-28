CC=      gcc
CFLAGS=  -Wall -g -std=gnu99
LD=      gcc
LDFLAGS= -L.
TARGETS= elgamel

all: $(TARGETS)

elgamel.o: elgamel.c 
	         $(CC) $(CFLAGS) -c -o $@ $^

elgamel:   elgamel.o
	         $(LD) $(LDFLAGS) -o $@ $^
