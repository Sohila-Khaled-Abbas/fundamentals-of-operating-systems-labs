CC ?= gcc
CFLAGS ?= -Wall -Wextra -O0 -g

TARGETS = global

all: $(TARGETS)

global: global.c
	$(CC) $(CFLAGS) -o global global.c

asm: global.c
	$(CC) -S -O0 -fno-asynchronous-unwind-tables global.c -o global_gen.s

clean:
	rm -f $(TARGETS) *.exe *.out global_gen.s

.PHONY: all asm clean
