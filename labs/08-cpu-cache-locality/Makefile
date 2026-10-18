CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2

TARGETS = cache_locality

all: $(TARGETS)

cache_locality: cache_locality.c
	$(CC) $(CFLAGS) -o cache_locality cache_locality.c

run: cache_locality
	./cache_locality

clean:
	rm -f $(TARGETS) *.exe *.out

.PHONY: all run clean
