# Lab 07: Sockets, OS Buffers, Queues & Web Server Internals

> **Course Mapping:** Section 8 — *Socket Management*  
> **Key Concepts:** Sockets as file descriptors, `listen()` backlog, SYN Queue vs. Accept Queue, OS Send Buffer (`SO_SNDBUF`) vs Receive Buffer (`SO_RCVBUF`), TCP Flow Control, Zero Window Probe, Slowloris & Slow Clients.

---

## Overview

In modern operating systems, networking is abstracted through the **BSD Socket API**. A socket is represented by an integer **File Descriptor (fd)** in the process file descriptor table.

Behind every TCP listening socket, the kernel manages two critical queues:
1. **SYN Queue (Incomplete Connection Queue)**: Holds connections currently executing the TCP 3-way handshake (SYN received, SYN-ACK sent, awaiting ACK).
2. **Accept Queue (Completed Connection Queue)**: Holds fully established connections ready to be picked up by the application via `accept()`. The size of this queue is bounded by the `backlog` argument in `listen(server_fd, backlog)`.

```
Client SYN ----> [ SYN Queue ] (OS handles handshake)
                      |
                      v 3-way handshake completes
                 [ Accept Queue ] (Size controlled by listen(fd, backlog))
                      |
                      v Application calls accept()
                 Returns client_fd to Application Process
```

---

## Files in this Lab

| Source File | Simulated Network / Server Condition |
|:---|:---|
| [`cwebserver.c`](file:///labs/07-socket-webserver/cwebserver.c) | **Baseline HTTP Server**: Standard single-threaded web server (`listen(fd, 10)`). |
| [`cwebserver-slow-connection.c`](file:///labs/07-socket-webserver/cwebserver-slow-connection.c) | **Backlog Saturation**: Restricted backlog (`listen(fd, 1)`), demonstrating accept queue overflow. |
| [`cwebserver-slow-download.c`](file:///labs/07-socket-webserver/cwebserver-slow-download.c) | **Slow Download**: Server simulates blocked send buffer / slow client reading (`sleep(6)`). |
| [`cwebserver-slow-processing.c`](file:///labs/07-socket-webserver/cwebserver-slow-processing.c) | **Blocked Worker**: Simulates long database/computation delay before reading request. |
| [`cwebserver-zero-tcp-window.c`](file:///labs/07-socket-webserver/cwebserver-zero-tcp-window.c) | **Zero TCP Window**: Loops reading infinitely without writing, or client filling buffers without reading. |
| [`test_clients.py`](file:///labs/07-socket-webserver/test_clients.py) | Python test client to reproduce all 4 network scenarios cross-platform. |
| [`test_clients.sh`](file:///labs/07-socket-webserver/test_clients.sh) | Bash/cURL script for Linux/WSL socket inspection. |
| [`Makefile`](file:///labs/07-socket-webserver/Makefile) | Compiles all server variations. |

---

## Hands-On Walkthrough

### 1. Build All Web Servers
```bash
make all
```

### 2. Experiment A: Baseline Server & Socket Queue Inspection
Start the standard web server:
```bash
./cwebserver
```
In another terminal, inspect the listening socket using `ss`:
```bash
ss -lntp "sport = :8080"
```
Notice:
- `Send-Q`: Shows the maximum Accept Queue limit (`10`).
- `Recv-Q`: Shows the current count of connections waiting in the Accept Queue.

Send a request using cURL:
```bash
curl -i http://localhost:8080/
```

### 3. Experiment B: Accept Queue Saturation (`cwebserver-slow-connection.c`)
Start the server with a backlog of 1:
```bash
./cwebserver-slow-connection
```
Run the burst connection test with Python:
```bash
python3 test_clients.py 3
```
Observe how subsequent connections are dropped or forced into SYN retransmission once the kernel accept queue overflows!

### 4. Experiment C: Zero TCP Window Probe (`cwebserver-zero-tcp-window.c`)
Start `cwebserver-zero-tcp-window` and run:
```bash
python3 test_clients.py 4
```
Observe with `tcpdump` or Wireshark:
```bash
tcpdump -i lo port 8080 -nn
```
When the receiver's OS buffer is filled to capacity, it advertises `win 0` (TCP Zero Window). The sender pauses transmission and sends periodic 1-byte **Zero Window Probes** until the application drains the buffer.

---

## Key Takeaways
1. Single-threaded blocking servers cannot scale because any client holding a connection open stalls the entire event loop.
2. Production servers (Nginx, Envoy, Node.js, Redis) use non-blocking I/O multiplexing (`epoll` on Linux, `kqueue` on BSD/macOS, `io_uring`) so one thread can service thousands of concurrent sockets.
