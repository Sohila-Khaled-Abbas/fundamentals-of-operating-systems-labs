CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2

TARGETS = alloc1 alloc2

all: $(TARGETS)

alloc1: alloc1.c
	$(CC) $(CFLAGS) -o alloc1 alloc1.c

alloc2: alloc2.c
	$(CC) $(CFLAGS) -o alloc2 alloc2.c

run: all
	@echo "=== Running alloc1 (1,000,000 individual malloc calls) ==="
	./alloc1
	@echo "=== Running alloc2 (1 contiguous malloc call) ==="
	./alloc2

clean:
	rm -f $(TARGETS) *.exe *.out

.PHONY: all run clean
