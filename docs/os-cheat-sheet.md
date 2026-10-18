# Operating Systems Quick Reference & Diagnostic Cheat Sheet

A companion reference for systems programming, debugging, and kernel inspection.

---

## 1. Process Virtual Address Space Layout (Linux x86-64)

```
 high address 0x7FFFFFFFFFFF +---------------------------------------+
                             | Kernel Space (Invisible to User Mode) |
              0x7FFFFFFFF000 +---------------------------------------+
                             | Stack (grows downwards)               |
                             |   [local vars, call frames]          |
                             |                   |                   |
                             |                   v                   |
                             |                                       |
                             |                   ^                   |
                             |                   |                   |
                             | Heap (grows upwards via brk / sbrk)   |
                             |   [malloc / free dynamic memory]      |
                             +---------------------------------------+
                             | Uninitialized Data (.bss)             |
                             |   [zero-initialized globals & statics]|
                             +---------------------------------------+
                             | Initialized Data (.data)              |
                             |   [initialized globals & statics]     |
                             +---------------------------------------+
                             | Read-Only Data (.rodata)              |
                             |   [string literals, constants]        |
                             +---------------------------------------+
                             | Text / Code (.text)                   |
                             |   [compiled machine instructions]     |
  low address 0x000000000000 +---------------------------------------+
```

---

## 2. x86-64 System V AMD64 ABI Calling Convention

### Integer Argument Passing Order
| Argument # | Register | Description |
|:---|:---|:---|
| 1st Argument | `%rdi` | First integer / pointer argument |
| 2nd Argument | `%rsi` | Second integer / pointer argument |
| 3rd Argument | `%rdx` | Third integer / pointer argument |
| 4th Argument | `%rcx` | Fourth integer / pointer argument |
| 5th Argument | `%r8`  | Fifth integer / pointer argument |
| 6th Argument | `%r9`  | Sixth integer / pointer argument |
| 7th+ Arguments | Stack | Pushed in reverse order onto user stack |

### Return Values & Control Registers
- **Return Value**: `%rax` (64-bit) or `%eax` (32-bit).
- **Stack Pointer**: `%rsp` (always points to the current top of the stack).
- **Base / Frame Pointer**: `%rbp` (points to the base of the current stack frame).
- **Instruction Pointer**: `%rip` (points to the next instruction to execute).

---

## 3. Core Operating System System Calls

| Syscall | Category | Purpose |
|:---|:---|:---|
| `fork()` | Process | Clones caller process, creating child with Copy-On-Write memory. |
| `execve()` | Process | Replaces current process image with a new executable program. |
| `waitpid()` | Process | Waits for child process state change (reaps zombies). |
| `exit()` / `_exit()` | Process | Terminates process and releases resources to kernel. |
| `brk()` / `sbrk()` | Memory | Expands or contracts the process heap break pointer. |
| `mmap()` | Memory | Maps files or anonymous memory pages into virtual address space. |
| `munmap()` | Memory | Unmaps virtual memory pages. |
| `socket()` | Sockets | Creates an endpoint for communication (returns file descriptor). |
| `bind()` | Sockets | Associates socket with an IP address and port number. |
| `listen()` | Sockets | Marks socket as passive; creates SYN Queue and Accept Queue. |
| `accept()` | Sockets | Dequeues completed connection from Accept Queue (blocks if empty). |
| `read()` / `write()` | I/O | Transfers data between file/socket descriptors and memory buffers. |
| `fsync()` | Storage | Flushes dirty OS page cache buffers for a fd to persistent disk. |
| `clone()` | Threads | Linux-specific syscall backing `pthreads`; creates threads or processes. |

---

## 4. Essential Kernel Inspection & Debugging Commands

### Binary & Assembly Inspection
```bash
# View all ELF sections and their sizes/addresses
readelf -S <binary>

# Disassemble binary code (.text section) with Intel syntax
objdump -d -M intel <binary>

# Display size breakdown of text, data, and bss segments
size <binary>

# Compile C source directly to annotated assembly
gcc -S -O0 -fno-asynchronous-unwind-tables source.c -o source.s
```

### Process & Memory Diagnostics
```bash
# Display detailed memory mapping of a running process (Virtual vs RSS)
pmap -x <pid>

# Inspect memory maps and permissions directly from procfs
cat /proc/<pid>/maps

# Inspect detailed page statistics (Shared, Private, Clean, Dirty)
cat /proc/<pid>/smaps

# Monitor virtual memory, page faults, and context switches every second
vmstat 1
```

### System Call & Execution Tracing
```bash
# Trace all system calls made by a process
strace ./<binary>

# Trace system calls with timestamps and syscall duration
strace -T -tt ./<binary>

# Trace only specific syscalls (e.g. memory allocations or network)
strace -e trace=brk,mmap,munmap ./<binary>
strace -e trace=network ./<binary>
```

### Sockets & Networking Inspection
```bash
# List all TCP listening sockets with accept queue backlog statistics
# Send-Q = Max backlog setting, Recv-Q = Current number of pending connections
ss -lntp

# Display active socket connections and timers
ss -tanpo

# Monitor network packet flow on localhost port 8080
tcpdump -i lo port 8080 -nn -X
```
