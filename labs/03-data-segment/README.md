# Lab 03: Data Segment & Global Variables

> **Course Mapping:** Section 3 — *The Anatomy of a Process*  
> **Key Concepts:** Initialized Data Segment (`.data`), Uninitialized Data (`.bss`), Read-Only Data (`.rodata`), static memory lifetime.

---

## Overview

Unlike local variables allocated on the stack (which are ephemeral and freed when a function returns), **global** and **static** variables exist for the entire lifetime of the process.

The operating system organizes them into distinct segments:
- **`.data`**: Holds initialized global and static variables (non-zero). These values are physically stored inside the executable binary file on disk.
- **`.bss`**: ("Block Started by Symbol") Holds uninitialized or zero-initialized globals. They take **0 bytes** of space in the file on disk; the OS simply zeroes out the corresponding memory pages upon loading.
- **`.rodata`**: Read-only constants and string literals.

---

## Files in this Lab

- [`global.c`](file:///labs/03-data-segment/global.c): Declares global variables `A = 10` and `B = 20`.
- [`global.s`](file:///labs/03-data-segment/global.s): Raw GCC assembly showing `.globl` directives and `.data` section placements.
- [`simpleglobal.s`](file:///labs/03-data-segment/simpleglobal.s): Annotated assembly focusing on RIP-relative addressing.
- [`Makefile`](file:///labs/03-data-segment/Makefile): Build configuration.

---

## Hands-On Walkthrough

### 1. Build and Generate Assembly
```bash
make all
make asm
```

### 2. Examine Symbol Directives in Assembly
```assembly
    .globl  A
    .data
    .align 4
    .type   A, @object
    .size   A, 4
A:
    .long   10

    .globl  B
    .align 4
    .type   B, @object
    .size   B, 4
B:
    .long   20
```

Notice that inside `main()`, variables `A` and `B` are accessed via **RIP-relative addressing**:
```assembly
movl    A(%rip), %edx     # Load global A using offset from current Instruction Pointer
movl    B(%rip), %eax     # Load global B
addl    %edx, %eax
```

### 3. Verify with `readelf` and `nm`
```bash
# List symbols and which section they reside in ('D' = .data, 'B' = .bss, 'T' = .text)
nm global | grep -E " A| B"

# Check segment headers
readelf -S global | grep -A 2 "\.data"
```

---

## Key Takeaways
1. Global variables are stored at fixed virtual memory offsets resolved during link time or loaded via PIC (Position Independent Code).
2. Because globals are shared across all threads in a process, concurrent mutations require OS synchronization primitives (mutexes, spinlocks) to avoid data races.
