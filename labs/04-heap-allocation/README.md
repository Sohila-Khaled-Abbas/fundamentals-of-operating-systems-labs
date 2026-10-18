# Lab 04: Dynamic Memory & Heap Allocation

> **Course Mapping:** Section 4 — *Memory Management*  
> **Key Concepts:** Dynamic memory allocation, `malloc()`, `free()`, pointer arithmetic, `brk()` / `sbrk()`, `mmap()`, memory leaks.

---

## Overview

When memory requirements cannot be determined at compile time (or exceed stack limits), applications request memory from the **Heap**.

The heap is a contiguous region of virtual memory managed jointly by:
1. **The Operating System Kernel**: Provides memory to the process in chunks of 4KB pages via the `brk()` (adjusting the program break pointer) or `mmap()` (creating anonymous memory mappings) system calls.
2. **The User-Space Memory Allocator** (e.g. `ptmalloc` in glibc, `jemalloc`, `tcmalloc`): Subdivides large kernel pages into smaller requested blocks when your C code calls `malloc(size)`.

```
Lower Memory (Static)                High Memory
  [ .text | .data | .bss ]  --->  [ HEAP ===>  ...gap...  <=== STACK ]
                                  ^
                                  |-- program break (brk pointer)
```

---

## Files in this Lab

- [`simpleheap.c`](file:///labs/04-heap-allocation/simpleheap.c): Allocates 4 bytes with `malloc()`, writes a value, mutates it via pointer dereference, and frees it.
- [`simpleheap.s`](file:///labs/04-heap-allocation/simpleheap.s): Assembly listing showing the call to `malloc@PLT` and `free@PLT`.
- [`simplifiedcode.s`](file:///labs/04-heap-allocation/simplifiedcode.s): Simplified assembly view.
- [`Makefile`](file:///labs/04-heap-allocation/Makefile): Build recipe.

---

## Hands-On Walkthrough

### 1. Build and Run
```bash
make all
./simpleheap
```

### 2. Trace System Calls with `strace`
Observe what happens when `malloc()` runs:
```bash
strace ./simpleheap
```

You will see:
```text
brk(NULL)                               = 0x55d7f8d68000
brk(0x55d7f8d89000)                     = 0x55d7f8d89000
```
Notice that `malloc(4)` did **not** ask the kernel for 4 bytes! The kernel only works with pages (4KB). The C library requested a large block (e.g. 132KB via `brk`) and carved out the 4 bytes for your pointer, saving the rest for future allocations.

### 3. Check for Memory Leaks with Valgrind
```bash
valgrind --leak-check=full ./simpleheap
```
Output:
```
All heap blocks were freed -- no leaks are possible
```

If you comment out `free(ptr);` in `simpleheap.c` and rebuild, Valgrind immediately flags the exact line of code where memory was leaked!

---

## Key Takeaways
1. `malloc()` returns a virtual address pointer. Physical RAM is not actually assigned until the page is touched for the first time (Lazy Allocation via Minor Page Fault).
2. Every `malloc()` stores hidden header metadata (chunk size, flags) immediately preceding the returned pointer address.
3. Forgetting `free()` causes virtual memory to grow until the kernel **OOM (Out Of Memory) Killer** steps in and terminates the process.
