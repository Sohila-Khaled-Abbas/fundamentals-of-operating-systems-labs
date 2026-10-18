CC ?= gcc
CFLAGS ?= -Wall -Wextra -O0 -g

TARGETS = cwebserver \
          cwebserver-slow-connection \
          cwebserver-slow-download \
          cwebserver-slow-processing \
          cwebserver-zero-tcp-window

all: $(TARGETS)

cwebserver: cwebserver.c
	$(CC) $(CFLAGS) -o cwebserver cwebserver.c

cwebserver-slow-connection: cwebserver-slow-connection.c
	$(CC) $(CFLAGS) -o cwebserver-slow-connection cwebserver-slow-connection.c

cwebserver-slow-download: cwebserver-slow-download.c
	$(CC) $(CFLAGS) -o cwebserver-slow-download cwebserver-slow-download.c

cwebserver-slow-processing: cwebserver-slow-processing.c
	$(CC) $(CFLAGS) -o cwebserver-slow-processing cwebserver-slow-processing.c

cwebserver-zero-tcp-window: cwebserver-zero-tcp-window.c
	$(CC) $(CFLAGS) -o cwebserver-zero-tcp-window cwebserver-zero-tcp-window.c

clean:
	rm -f $(TARGETS) *.exe *.out

.PHONY: all clean
