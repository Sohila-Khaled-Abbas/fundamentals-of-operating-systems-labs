CC ?= gcc
CFLAGS ?= -Wall -Wextra -O0 -g

TARGETS = nofork p cow

all: $(TARGETS)

nofork: nofork.c
	$(CC) $(CFLAGS) -o nofork nofork.c

p: p.c
	$(CC) $(CFLAGS) -o p p.c

cow: cow.c
	$(CC) $(CFLAGS) -o cow cow.c

clean:
	rm -f $(TARGETS) *.exe *.out

.PHONY: all clean
