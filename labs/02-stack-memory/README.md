# Lab 02: Stack Memory, Call Frames & Calling Conventions

> **Course Mapping:** Section 3 — *The Anatomy of a Process*  
> **Key Concepts:** Function call frames, stack pointer (`%rsp`), base pointer (`%rbp`), instruction pointer (`%rip`), return addresses, prologue & epilogue.

---

## Overview

The **stack** is a LIFO (Last-In, First-Out) data structure managed directly by the CPU architecture. It grows **downward** (from high memory addresses toward lower memory addresses).

Each function invocation creates a **Stack Frame** that contains:
1. Function arguments (beyond those in registers)
2. Return address (pushed automatically by `call`)
3. Caller's saved Base Pointer (`push %rbp`)
4. Local variables

```
High Memory
  |
  +-------------------------------+
  | main() Local Variables        |
  +-------------------------------+
  | Return Address (to main)      | <-- pushed by `call func1`
  +-------------------------------+
  | Saved %rbp of main            | <-- pushed by `pushq %rbp`
  +-------------------------------+ <-- %rbp of func1 points here
  | func1() Local Variables (z=1) |
  +-------------------------------+ <-- %rsp (top of stack) points here
  |                               |
  v (grows downward)
Low Memory
```

---

## Files in this Lab

- [`stack.c`](file:///labs/02-stack-memory/stack.c): Code with `main()` calling `func1()`.
- [`stack_output_assembly.s`](file:///labs/02-stack-memory/stack_output_assembly.s): Raw assembly listing with annotations.
- [`simplfied_assembly.s`](file:///labs/02-stack-memory/simplfied_assembly.s): Cleaned up assembly focusing on call sequences.
- [`loaded.s`](file:///labs/02-stack-memory/loaded.s): Assembly showing symbol definitions and loading.
- [`nocomments.s`](file:///labs/02-stack-memory/nocomments.s): Stripped assembly listing.
- [`Makefile`](file:///labs/02-stack-memory/Makefile): Compilation automation.

---

## Hands-On Walkthrough

### 1. Compile and Generate Assembly
```bash
make all
make asm
```

### 2. Trace the Call Sequence
Inspect `func1` in `stack.c`:
```c
void func1 () {
    int z = 1;
    z = z + 1;
    return;
}
```

In assembly:
```assembly
func1:
    pushq   %rbp            # 1. Save caller's base pointer
    movq    %rsp, %rbp      # 2. Set new frame base
    movl    $1, -4(%rbp)    # 3. int z = 1
    addl    $1, -4(%rbp)    # 4. z = z + 1
    nop
    popq    %rbp            # 5. Restore caller's base pointer
    ret                     # 6. Pop return address off stack into %rip
```

When `ret` executes, the CPU pops the saved `%rip` off the stack and jumps right back to line 12 of `main()`.

### 3. Debug with GDB
Step through instructions and inspect registers:
```bash
gdb ./stack
(gdb) break main
(gdb) run
(gdb) info registers rsp rbp rip
(gdb) stepi
(gdb) x/16xw $rsp
```

---

## Key Takeaways
1. Stack allocation is virtually free in terms of CPU cost—it is just adjusting the `%rsp` register pointer.
2. Function return addresses are stored on the stack; overwriting them via buffer overflows leads to security exploits (stack smashing).
3. The stack has a fixed limit (typically 8MB on Linux, check via `ulimit -s`); exceeding it causes a **Stack Overflow (SIGSEGV)**.
