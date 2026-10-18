CC ?= gcc
CFLAGS ?= -Wall -Wextra -O0 -g

TARGETS = simple test

all: $(TARGETS)

simple: simple.c
	$(CC) $(CFLAGS) -o simple simple.c

test: test.c
	$(CC) $(CFLAGS) -o test test.c

asm: simple.c test.c
	$(CC) -S -O0 simple.c -o simple_gen.s
	$(CC) -S -O0 test.c -o test_gen.s

clean:
	rm -f $(TARGETS) *.exe *.out simple_gen.s test_gen.s

.PHONY: all asm clean
