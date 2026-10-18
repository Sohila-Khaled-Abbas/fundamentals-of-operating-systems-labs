CC ?= gcc
CFLAGS ?= -Wall -Wextra -O0 -g

TARGETS = stack

all: $(TARGETS)

stack: stack.c
	$(CC) $(CFLAGS) -o stack stack.c

asm: stack.c
	$(CC) -S -O0 -fno-asynchronous-unwind-tables -fno-stack-protector stack.c -o stack_gen.s

clean:
	rm -f $(TARGETS) *.exe *.out stack_gen.s

.PHONY: all asm clean
