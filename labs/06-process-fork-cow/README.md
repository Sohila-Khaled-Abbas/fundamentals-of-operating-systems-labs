# Lab 06: Process Management, `fork()` & Copy-On-Write (COW)

> **Course Mapping:** Section 6 — *Process Management* & Section 4 — *Memory Management*  
> **Key Concepts:** `fork()`, PID, virtual memory page tables, Copy-On-Write (COW), page faults, physical frame sharing.

---

## Overview

When a process calls `fork()`, the operating system creates a nearly identical duplicate child process. 

In early Unix operating systems, `fork()` physically copied every single byte of RAM from parent to child, which was slow and memory intensive. Modern operating systems solve this with **Copy-On-Write (COW)**:

1. When `fork()` is invoked, the child receives a duplicate **Page Table**, pointing to the exact same physical memory frames as the parent.
2. The MMU marks all shared pages as **Read-Only**.
3. Neither parent nor child consumes extra RAM initially!
4. The instant either process tries to **write** to a page, the CPU hardware generates a **Page Fault (Trap)**.
5. The kernel intercepts the fault, allocates a new physical 4KB frame, copies only that specific page, updates the page table to Read/Write, and resumes execution seamlessly.

```
Initial fork(): Both Page Tables point to the same Physical Frame
Parent Virtual Page 0x1000 ---> [ Physical Frame #42 ] <--- Child Virtual Page 0x1000
                                  (Marked Read-Only)

Child attempts to WRITE: Kernel traps page fault, duplicates frame!
Parent Virtual Page 0x1000 ---> [ Physical Frame #42 ] (Parent Data)
Child Virtual Page 0x1000  ---> [ Physical Frame #89 ] (Child Data Copy)
```

---

## Files in this Lab

- [`nofork.c`](file:///labs/06-process-fork-cow/nofork.c): Single process allocating 10 MB buffer. Baseline memory usage.
- [`p.c`](file:///labs/06-process-fork-cow/p.c): Allocates 10 MB buffer, forks into parent & child, but **neither writes** to the buffer. Memory is 100% shared via COW!
- [`cow.c`](file:///labs/06-process-fork-cow/cow.c): Allocates 10 MB buffer, forks, and the child **writes to new pages**, triggering page faults and physical memory duplication.
- [`Makefile`](file:///labs/06-process-fork-cow/Makefile): Build configuration.

---

## Hands-On Walkthrough

> **Note:** These experiments use POSIX APIs (`fork()`, `unistd.h`) and must be executed in Linux, WSL2, or the provided Docker container.

### 1. Build the Binaries
```bash
make all
```

### 2. Experiment A: Fork without Writes (`p.c`)
Run `./p &` in the background and inspect memory:
```bash
./p &
PID=$!
ps -o pid,vsz,rss,comm -p $PID
```
Both parent and child show 10 MB virtual memory, but **Resident Set Size (RSS)** shared memory indicates that the physical RAM is shared.

### 3. Experiment B: Inspect Page Sharing with `pmap` and `/proc/<pid>/smaps`
```bash
# Check shared vs private clean/dirty pages
cat /proc/<PID>/smaps | grep -A 10 "Shared_Dirty"
```

### 4. Experiment C: Trigger Copy-on-Write with Writes (`cow.c`)
Run `./cow` and observe the child PID modifying bytes across 1000-byte strides:
```bash
./cow
```
As `buffer[(i++)*1000]++` touches new 4KB virtual pages, the Linux kernel duplicates individual 4KB frames on-the-fly!

---

## The Famous Instagram "Python, Fork & COW" War Story

In course reference [A War Story About a Python, a Fork, a Cow, and a Bug](https://codeconfessions.substack.com/p/a-war-story-involving-a-python-a), Instagram engineers ran Django web servers using a prefork model (Gunicorn/uWSGI). 

They noticed that even though workers were only *reading* data, the servers rapidly ran out of memory. 

**Why?** In CPython, reading an object increments its internal `ob_refcnt` reference counter. Incrementing a counter is a **write**! This caused the CPU to trigger COW page faults across all shared memory pages, forcing the OS to duplicate virtually every page in memory and destroying the memory savings of `fork()`.
