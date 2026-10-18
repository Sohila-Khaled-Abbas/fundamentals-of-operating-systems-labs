CC ?= gcc
CFLAGS ?= -Wall -Wextra -O0 -g

TARGETS = simpleheap

all: $(TARGETS)

simpleheap: simpleheap.c
	$(CC) $(CFLAGS) -o simpleheap simpleheap.c

asm: simpleheap.c
	$(CC) -S -O0 -fno-asynchronous-unwind-tables simpleheap.c -o simpleheap_gen.s

clean:
	rm -f $(TARGETS) *.exe *.out simpleheap_gen.s

.PHONY: all asm clean
