CC=      gcc
CFLAGS=  -Wall -g -std=gnu99
LD=      gcc
LDFLAGS= -L.
TARGETS= elgamel elgamel-bob elgamel-alice

all: $(TARGETS)

elgamel.o: elgamel.c 
	         $(CC) $(CFLAGS) -c -o $@ $^

elgamel-bob.o: elgamel-bob.c
	         $(CC) $(CFLAGS) -c -o $@ $^

elgamel-alice.o: elgamel-alice.c
	         $(CC) $(CFLAGS) -c -o $@ $^

elgamel:   elgamel.o
	         $(LD) $(LDFLAGS) -o $@ $^

elgamel-bob:   elgamel-bob.o
	         $(LD) $(LDFLAGS) -o $@ $^

elgamel-alice:   elgamel-alice.o
	         $(LD) $(LDFLAGS) -o $@ $^
