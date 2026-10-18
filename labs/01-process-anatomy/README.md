# Lab 01: The Anatomy of a Process & Machine Code

> **Course Mapping:** Section 3 — *The Anatomy of a Process*  
> **Key Concepts:** Compilation pipeline, ELF binary structure, x86_64 assembly, instruction decoding, register movement.

---

## Overview

A program is a passive file on disk. A **process** is an active, executing instance of a program residing in virtual memory, assigned its own CPU registers, file descriptors, and address space.

In this lab, we inspect how high-level C code is translated by the compiler into machine instructions, how registers are manipulated, and how to disassemble and inspect the binary sections.

```
+--------------------------------------------------------------------------+
| Source (.c) ---> Preprocessed (.i) ---> Assembly (.s) ---> Object (.o)   |
|                                                              |           |
|                                                     Linker (ld)          |
|                                                              v           |
|                                                       Executable (ELF)   |
+--------------------------------------------------------------------------+
```

---

## Files in this Lab

- [`simple.c`](file:///labs/01-process-anatomy/simple.c): Minimal program adding two integers and printing the sum.
- [`test.c`](file:///labs/01-process-anatomy/test.c): Variation exploring local variable allocation on the stack.
- [`simpliefied.s`](file:///labs/01-process-anatomy/simpliefied.s): Clean, annotated assembly showing the function prologue, register moves, and epilogue.
- [`machine.s`](file:///labs/01-process-anatomy/machine.s): Machine-level assembly representation.
- [`full-assembly.s`](file:///labs/01-process-anatomy/full-assembly.s): Complete GCC assembly output including unwind information and metadata.
- [`Makefile`](file:///labs/01-process-anatomy/Makefile): Build automation script.

---

## Hands-On Walkthrough

### 1. Build Executables & Assembly
```bash
# Build the binaries
make all

# Generate fresh assembly files
make asm
```

### 2. Dissect the Assembly (`simple.c` vs `simpliefied.s`)
Notice how local variables are loaded into the stack relative to the Base Pointer (`%rbp`):
```assembly
pushq   %rbp              # Save previous base pointer
movq    %rsp, %rbp        # Set current stack pointer as new base pointer
subq    $16, %rsp         # Allocate 16 bytes on stack for local variables

movl    $1, -4(%rbp)      # a = 1  (stored at rbp - 4)
movl    $3, -8(%rbp)      # b = 3  (stored at rbp - 8)
movl    -4(%rbp), %edx    # edx = a
movl    -8(%rbp), %eax    # eax = b
addl    %edx, %eax        # eax = a + b
movl    %eax, -12(%rbp)   # c = eax
```

### 3. Inspect Binary ELF Sections with GNU Tools
```bash
# View segment sizes (text, data, bss)
size simple

# View all ELF sections and permissions
readelf -S simple

# Disassemble with objdump
objdump -d -M intel simple
```

---

## Key Takeaways
1. The **Text Segment** (`.text`) contains readonly machine instructions.
2. Local variables reside on the **Stack Frame** offset from `%rbp`.
3. The compiler balances register usage vs stack memory based on optimization flags (`-O0` vs `-O2`).
