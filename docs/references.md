# Course References & Extended Operating Systems Curated Resources

This guide catalogues the official references cited in Hussein Nasser's *Fundamentals of Operating Systems* along with authoritative textbooks, papers, tools, and visualizers.

---

## 1. Official Course References (Citations from Hussein Nasser)

| Topic | Title / Article | Key Insight / Significance | Link |
|:---|:---|:---|:---|
| **Process & COW** | *A War Story Involving a Python, a Fork, a Cow, and a Bug* | Instagram's discovery of why Python's reference counting triggered Copy-On-Write page duplication, destroying memory sharing in forked workers. | [Substack](https://codeconfessions.substack.com/p/a-war-story-involving-a-python-a) |
| **Concurrency** | *User and Kernel Level Threads* | Explains 1:1, N:1, and M:N thread mapping models and user-space vs kernel-space context switching overhead. | [IIT CS450](http://www.cs.iit.edu/~cs561/cs450/ChilkuriDineshThreads/dinesh%27s%20files/User%20and%20Kernel%20Level%20Threads.html) |
| **Virtual Memory** | *Sharing Page Tables with `mshare()`* | Proposed Linux kernel enhancement to allow processes to share page table entries directly to conserve memory on massive database instances. | [LWN.net](https://lwn.net/Articles/895217/) |
| **CPU / MMU** | *TLB Flush Fundamentals* | Explains how context switching between processes invalidates the Translation Lookaside Buffer and the cost of page table walks. | [UMass Amherst](https://lass.cs.umass.edu/~shenoy/courses/fall12/lectures/notes/Lec06_notes.pdf) |
| **Hardware** | *DRAM Addressing & Sense Amplifiers* | Hardware-level memory architecture: banks, ranks, row buffers, and capacitor sense amplifiers. | [AMD Docs](https://docs.amd.com/r/en-US/pg313-network-on-chip/DRAM-Addressing) & [Wikipedia](https://en.wikipedia.org/wiki/Sense_amplifier) |
| **Storage** | *Linux on 4KB Sector Disks* | The transition from 512-byte legacy sectors to Advanced Format 4KB sectors, physical alignment, and read-modify-write penalties. | [IBM Archives](https://web.archive.org/web/20190503044834/https://developer.ibm.com/tutorials/l-linux-on-4kb-sector-disks/) |
| **Architecture** | *RISC vs. CISC: 20 Key Comparisons* | Instruction decoding complexity, fixed vs variable instruction length, and modern hybrid micro-op architectures. | [Spiceworks](https://www.spiceworks.com/tech/tech-general/articles/risc-vs-cisc/) |
| **Database Architecture** | *Let's make PostgreSQL multi-threaded* & *Objections* | Famous PostgreSQL mailing list debate: why process-per-connection provides critical crash isolation, memory limits, and security compared to multi-threading. | [PostgreSQL pgsql-hackers (2023)](https://www.postgresql.org/message-id/20230607213721.al3etgcgtija3ytz%40awork3.anarazel.de) |
| **Storage & I/O** | *Firefox 3 uses fsync excessively* | Bug report analysis demonstrating how excessive `fsync()` calls choked the Linux Page Cache and caused whole-system UI freezes on SQLite commits. | [Mozilla Bugzilla](https://bugzilla.mozilla.org/show_bug.cgi?id=421482) |
| **CPU Architecture** | *ARM Address Space ID (ASID)* | How modern MMUs tag TLB entries with process IDs to avoid full TLB flushes on context switches. | [ARM Developer](https://developer.arm.com/documentation/den0013/d/The-Memory-Management-Unit/Multi-tasking-and-OS-usage-of-translation-tables/Address-Space-ID) |
| **Kernel Optimization** | *Linux Kernel Faster memchr* | AVX2/AVX-512 vectorization in kernel memory manipulation routines for high throughput. | [Phoronix](https://www.phoronix.com/news/Linux-Kernel-Faster-memchr) |
| **I/O Architecture** | *Direct Memory Access (DMA)* | How peripherals and network controllers transfer data directly to RAM without continuous CPU intervention. | [Linux Kernel Guide](https://web.archive.org/web/20070521144721/https://www.science.unitn.it/~fiorella/guidelinux/tlk/node87.html) |

---

## 2. Essential Textbooks & Authoritative Books

1. **Operating Systems: Three Easy Pieces (OSTEP)** — *Remzi H. Arpaci-Dusseau & Andrea C. Arpaci-Dusseau*
   - Free online textbook structured around **Virtualization**, **Concurrency**, and **Persistence**.
   - URL: [pages.cs.wisc.edu/~remzi/OSTEP/](https://pages.cs.wisc.edu/~remzi/OSTEP/)

2. **Computer Systems: A Programmer's Perspective (CS:APP)** — *Randal E. Bryant & David R. O'Hallaron*
   - The gold standard for understanding how hardware, OS, and compilers meet (machine-level representation, cache memories, virtual memory, system-level I/O).
   - URL: [csapp.cs.cmu.edu](https://csapp.cs.cmu.edu/)

3. **The Linux Programming Interface (TLPI)** — *Michael Kerrisk*
   - The definitive encyclopedia of Linux system calls, process management, sockets, signals, and IPC.
   - URL: [man7.org/tlpi/](https://man7.org/tlpi/)

4. **Systems Performance: Enterprise and the Cloud (2nd Edition)** — *Brendan Gregg*
   - Deep dive into measuring and tuning CPU, memory, storage, and networking using modern Linux tools (eBPF, perf, ftrace).
   - URL: [brendangregg.com/systems-performance-2nd-edition-book.html](https://www.brendangregg.com/systems-performance-2nd-edition-book.html)

5. **Operating System Concepts ("The Dinosaur Book")** — *Abraham Silberschatz, Peter B. Galvin, Greg Gagne*
   - Classic university text covering scheduling algorithms, synchronization primitives, deadlocks, and virtual memory.

---

## 3. Top University Operating Systems Courses (Open Courseware)

- **MIT 6.S081 / 6.828: Operating System Engineering**
  - Hands-on kernel development using the xv6 teaching OS on RISC-V.
  - URL: [pdos.csail.mit.edu/6.828/](https://pdos.csail.mit.edu/6.828/)
- **UC Berkeley CS 162: Operating Systems and Systems Programming**
  - Covers concurrency, virtual memory, file systems, distributed systems, and Pintos OS.
  - URL: [cs162.org](https://cs162.org/)
- **Stanford CS 140 / CS 111: Operating Systems Principles**
  - Thread synchronization, page replacement algorithms, and crash-resilient file systems.
  - URL: [web.stanford.edu/class/cs140/](https://web.stanford.edu/class/cs140/)
- **Harvard CS 61: Systems Programming and Machine Organization**
  - Memory models, assembly language, and kernel abstractions.
  - URL: [read.seas.harvard.edu/cs61/](https://read.seas.harvard.edu/cs61/)

---

## 4. Key Seminal Papers & RFCs

- **The UNIX Time-Sharing System** — *Dennis M. Ritchie and Ken L. Thompson (1974)*
  - The foundational paper outlining the Unix design philosophy, file system, processes, and I/O.
- **The C10K Problem** — *Dan Kegel (1999)*
  - Landmark exploration of handling tens of thousands of concurrent clients and the transition from thread-per-client to non-blocking I/O multiplexing (`epoll`/`kqueue`).
- **RFC 9293 (Transmission Control Protocol - TCP Specification)**
  - Replaces RFC 793; formalizes connection states (SYN, ESTABLISHED, FIN_WAIT), TCP sliding window, and socket buffer dynamics.
- **Virtual Memory Architecture in Linux** — *Mel Gorman (Understanding the Linux Virtual Memory Manager)*
  - Detailed examination of zone allocators, slab allocators, page frame management, and swap.

---

## 5. Interactive Simulators & Diagnostic Tools

- [Compiler Explorer (godbolt.org)](https://godbolt.org/): Interactive disassembly of C/C++ to x86_64 and ARM assembly with compiler optimizations.
- [Linux man-pages (man7.org)](https://man7.org/linux/man-pages/): Official Linux programmer manual for all syscalls and standard library functions.
- [Brendan Gregg's Linux Performance Tools Map](https://www.brendangregg.com/linuxperf.html): Visual diagram mapping kernel subsystems to diagnostic utilities.
- [ExplainShell](https://explainshell.com/): Visual breakdown of complex command-line arguments.
