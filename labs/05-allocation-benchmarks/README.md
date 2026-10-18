# Lab 05: Memory Allocation Overhead, Fragmentation & Batching

> **Course Mapping:** Section 4 — *Memory Management*  
> **Key Concepts:** Memory allocator metadata, cache locality, memory fragmentation, cost of individual vs. contiguous allocations.

---

## Overview

In high-performance systems and backend engines (such as network routers, proxy servers, and database engines), managing memory allocations efficiently is crucial.

This lab directly demonstrates the difference between:
- **`alloc1.c`**: Performing **1,000,000 separate `malloc()` calls**, each for one 44-byte `Packet` struct.
- **`alloc2.c`**: Performing **1 single contiguous `malloc()` call** for an array of 1,000,000 `Packet` structs.

```
Approach 1 (alloc1.c): 1,000,000 separate allocations
  [Header|Data] ... [Header|Data] ... [Header|Data] ... scattered all over heap!
  Overhead: 1,000,000 * 16 bytes metadata = ~16 MB wasted purely on chunk headers!

Approach 2 (alloc2.c): 1 contiguous allocation
  [Header| Packet 0 | Packet 1 | Packet 2 | ... | Packet 999999 ]
  Overhead: Exactly 1 chunk header! Fast, sequential memory access.
```

---

## Files in this Lab

- [`alloc1.c`](file:///labs/05-allocation-benchmarks/alloc1.c): Allocates an array of 1M pointers, each pointing to a separate `malloc(sizeof(Packet))`.
- [`alloc2.c`](file:///labs/05-allocation-benchmarks/alloc2.c): Allocates the entire array in a single `malloc(1000000 * sizeof(Packet))`.
- [`alloc2.s`](file:///labs/05-allocation-benchmarks/alloc2.s): Assembly listing for alloc2.
- [`Makefile`](file:///labs/05-allocation-benchmarks/Makefile): Automated benchmark runner.

---

## Hands-On Walkthrough

### 1. Compile the Benchmarks
```bash
make all
```

### 2. Measure Execution Time & Memory Usage
On Linux / WSL, run with `/usr/bin/time -v`:

```bash
# Benchmark alloc1 (1M mallocs)
/usr/bin/time -v ./alloc1

# Benchmark alloc2 (1 malloc)
/usr/bin/time -v ./alloc2
```

### 3. Expected Observations

| Metric | `alloc1` (Individual) | `alloc2` (Contiguous) | Difference |
|:---|:---|:---|:---|
| **CPU Time** | ~150 - 300 ms | ~2 - 8 ms | **~30x - 50x faster** |
| **Max RSS (Memory)** | ~70 - 80 MB | ~44 MB | **~40% less memory** |
| **Heap Chunks** | 1,000,000 | 1 | Zero fragmentation |
| **Page Faults** | High | Low | Much higher TLB hit rate |

---

## Key Takeaways
1. **Allocator Metadata Overhead**: Every individual `malloc` adds 8–16 bytes of bookkeeping headers. Over millions of objects, this consumes dozens of megabytes of RAM.
2. **Cache Locality**: Contiguous memory in `alloc2` allows the CPU hardware prefetcher to load subsequent packets into L1/L2 caches ahead of time.
3. **Engineering Practice**: Use object pools, arenas, or flat contiguous buffers for high-frequency data structures instead of fine-grained `malloc`/`free` calls.
