# Lab 09: Storage Management & The OS Page Cache

> **Course Mapping:** Section 7 — *Storage Management*  
> **Key Concepts:** Storage hierarchy, OS Page Cache, buffered I/O, dirty pages, writeback daemons, `fsync()`, `fdatasync()`, durability vs latency.

---

## Overview

Storage devices (HDDs and SSDs) are thousands of times slower than system RAM. To prevent applications from grinding to a halt during file I/O, modern operating system kernels implement the **Page Cache**:

1. When an application calls `write()`, the kernel copies the data into pages in RAM, marks them as **Dirty Pages**, and returns success immediately (sub-microsecond latency).
2. The dirty pages are flushed to physical storage asynchronously in the background by kernel threads (e.g. `kswapd`, `pdflush`).
3. If an application requires **ACID Durability** (guaranteeing data will survive a power loss or crash, like a database Write-Ahead Log), it must explicitly invoke `fsync()`.
4. Calling `fsync()` forces the CPU to wait until the storage controller has physically flushed its volatile on-disk cache to persistent flash cells or magnetic platters.

```
Application write() ----> [ OS Page Cache in RAM ] ----> Returns to App immediately!
                                |
                   (Asynchronous background flush)
                                |
                                v
               [ Physical Disk / NVMe SSD Storage ]
```

---

## Files in this Lab

- [`page_cache_sync.c`](file:///labs/09-storage-page-cache/page_cache_sync.c): Benchmarks 1,000 writes using buffered Page Cache vs 1,000 writes with explicit `fsync()` after each write.
- [`Makefile`](file:///labs/09-storage-page-cache/Makefile): Automated build configuration.

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
   Lab 09: Storage Management & OS Page Cache Demo      
========================================================
Writing 1000 records of 128 bytes each...

[1] Buffered Writes (Kernel Page Cache):
    Elapsed Time: 0.85 ms
    Latency/Write: 0.0008 ms
    Status: Data resides in RAM (dirty pages), OS flushes lazily.

[2] Synchronous Writes (fsync after each write):
    Elapsed Time: 342.10 ms
    Latency/Write: 0.3421 ms
    Status: Every write is guaranteed durable on physical storage.

========================================================
Analysis & Insights:
    fsync is ~400.0x slower due to disk write cycles.
========================================================
```

### 3. Monitoring Dirty Pages on Linux
While running disk-heavy applications, monitor Linux kernel dirty pages in real time:
```bash
watch -n 0.5 "cat /proc/meminfo | grep -E 'Dirty|Writeback'"
```

---

## Real-World Case Study: The Firefox 3 `fsync` Stall

In official course reference [Firefox 3 uses fsync excessively](https://bugzilla.mozilla.org/show_bug.cgi?id=421482), Firefox switched to SQLite for URL history and bookmarks. On every URL visited, SQLite issued an `fsync()`. 

On Linux with Ext3/Ext4 filesystems in `data=ordered` mode, an `fsync()` on *any* file forced the kernel journal to flush *all* pending filesystem writes across the entire operating system! This caused total desktop UI freezes whenever Firefox recorded a page visit.

Databases solve this bottleneck using **Group Commit**: bundling `fsync()` calls across hundreds of concurrent transactions into a single disk sync.
