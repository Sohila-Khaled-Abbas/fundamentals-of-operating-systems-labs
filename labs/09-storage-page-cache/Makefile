CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2

TARGETS = page_cache_sync

all: $(TARGETS)

page_cache_sync: page_cache_sync.c
	$(CC) $(CFLAGS) -o page_cache_sync page_cache_sync.c

run: page_cache_sync
	./page_cache_sync

clean:
	rm -f $(TARGETS) *.exe *.out *.tmp

.PHONY: all run clean
