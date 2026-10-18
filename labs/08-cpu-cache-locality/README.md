# Lab 08: Inside the CPU — Cache Hierarchy & Spatial Locality

> **Course Mapping:** Section 5 — *Inside The CPU*  
> **Key Concepts:** CPU cache levels (L1, L2, L3), cache lines (64 bytes), spatial vs temporal locality, row-major vs column-major array traversal.

---

## Overview

The speed of modern CPU cores (clocked at 3 to 5 GHz) is orders of magnitude faster than main memory (DRAM). Fetching data from RAM can take 50 to 100 nanoseconds (hundreds of clock cycles), creating the classic **Memory Wall**.

To bridge this latency gap, CPUs use multi-level hardware caches:
- **L1 Cache**: ~1 to 2 ns latency (per core, 32KB–64KB)
- **L2 Cache**: ~3 to 5 ns latency (per core, 512KB–1MB)
- **L3 Cache**: ~10 to 20 ns latency (shared across cores, 16MB–64MB)
- **Main Memory (RAM)**: ~60 to 100 ns latency

Whenever the CPU accesses memory, it fetches an entire **Cache Line** (standard 64 bytes), not just the single requested byte. 

If your code accesses contiguous elements within that 64-byte window (**Spatial Locality**), subsequent reads hit L1 cache with zero delay! If your code skips across memory in large strides, every single access incurs a cache miss.

---

## Files in this Lab

- [`cache_locality.c`](file:///labs/08-cpu-cache-locality/cache_locality.c): Benchmarks row-major traversal (stride-1, cache-friendly) vs column-major traversal (stride-4096, cache-unfriendly) over a 4096 x 4096 integer matrix (64 MB).
- [`Makefile`](file:///labs/08-cpu-cache-locality/Makefile): Build configuration.

---

## Hands-On Walkthrough

### 1. Build and Run the Benchmark
```bash
make all
make run
```

### 2. Sample Benchmark Output
```text
========================================================
  Lab 08: CPU Cache Hierarchy & Spatial Locality Demo   
========================================================
Matrix size: 4096 x 4096 (64 MB of contiguous memory)

[1] Row-Major Traversal (Cache Friendly):
    Time:       14.20 ms
    Elements:   16777216

[2] Column-Major Traversal (Cache Unfriendly):
    Time:       128.50 ms
    Elements:   16777216

========================================================
Performance Summary:
    Speedup factor with Cache Locality: 9.05x faster!
========================================================
```

### 3. Profiling Hardware Cache Misses with Linux `perf`
On Linux, run:
```bash
perf stat -e L1-dcache-loads,L1-dcache-load-misses,cycles,instructions ./cache_locality
```
Notice the dramatic difference in **L1-dcache-load-misses**!

---

## Key Takeaways
1. Memory access patterns dictate real-world performance more than algorithmic complexity in many backend applications.
2. Contiguous arrays and sequential memory scanning outperform linked lists and pointer-heavy trees due to CPU cache line prefetching.
3. In C, multidimensional arrays are laid out in **row-major order**; inner loops should iterate over column indices `matrix[row][col]`.
