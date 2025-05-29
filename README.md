# MLFQ Scheduler for xv6

This project implements a **Multi-Level Feedback Queue (MLFQ)** scheduler for the [xv6](https://pdos.csail.mit.edu/6.828/2021/xv6.html) operating system. The goal is to enhance xv6’s default Round Robin scheduler by dynamically prioritizing I/O-bound processes and preventing starvation of CPU-bound processes through an aging mechanism.

---

## Features

- Multiple priority queues  
- Dynamic priority adjustment  
- Aging mechanism to prevent starvation  
- Improved responsiveness for I/O-bound processes  
- Queue monitoring and debugging support  

---

## Motivation

The default Round Robin scheduler in xv6 treats all processes equally, which can lead to:

- Poor responsiveness for interactive tasks  
- Starvation of long-running background processes  

This MLFQ scheduler addresses these limitations by:

- Prioritizing short and I/O-bound jobs  
- Penalizing CPU-bound processes with reduced priority  
- Promoting long-waiting processes to higher priorities using an aging mechanism  
- Providing a balanced and fair CPU scheduling approach  

---

## Design and Implementation

### Key Modifications

- `struct proc` updated to include:
  - `int priority` – current queue level (0 = highest)
  - `int time_slices_used` – number of ticks consumed in current level
  - `int wait_time` – tracks wait time for aging

- `scheduler()` modified to:
  - Select processes based on queue priority
  - Apply time slice restrictions per priority level
  - Requeue or demote processes appropriately

- `update_priorities()` added to apply the aging mechanism

---

### Parameters

```c
#define NUM_QUEUES 3
#define AGING_THRESHOLD 15
#define TIME_SLICE {5, 10, 15}  // Time slices for each queue level
```

---

## Scheduling Logic

### Process Selection

```c
p = select_process();
run_process(p, TIME_SLICE[p->priority]);
```

### Time Slice Exhaustion

```c
if (p->time_slices_used >= TIME_SLICE[p->priority]) {
    p->time_slices_used = 0;
    if (p->priority < NUM_QUEUES - 1)
        p->priority++;
}
```

---

## Aging Mechanism

To prevent starvation, processes that have waited longer than `AGING_THRESHOLD` are promoted:

```c
if (p->wait_time >= AGING_THRESHOLD) {
    p->priority--;
    p->wait_time = 0;
}
```

This ensures that low-priority processes eventually receive CPU time.

---

## Test Programs

### cpu_bound.c

Simulates a CPU-intensive workload with continuous computation.

### io_bound.c

Simulates an I/O-bound workload using sleep intervals.

---

## Running the Scheduler

### Compilation and Execution

```bash
$ make qemu             # Compile and start xv6
$ cpu_bound &           # Run CPU-bound process in background
$ io_bound &            # Run I/O-bound process in background
```

### Observation

Monitor logs and queue behavior to verify correct scheduling, demotion, promotion, and starvation prevention. Use debugging output to trace process movements across queues.

---

## Results

| Metric            | Round Robin | MLFQ         |
|-------------------|-------------|--------------|
| Time Slice        | 15 ticks    | 5–15 ticks   |
| Responsiveness    | Poor        | Improved     |
| Starvation Issues | Present     | Resolved     |

The MLFQ scheduler improves responsiveness for interactive processes and ensures fairness without significant overhead.

---

## References

- xv6 Source Code Documentation  
- S. Baruah et al., *Real-Time Scheduling in Embedded Systems*  
- J. Li and M. Kim, *Efficient Scheduling for Multi-core Processors*  
- Arpaci-Dusseau et al., *Operating Systems: Three Easy Pieces*  
- Smith College, *Scheduling in Unix-like Systems*  

---

**Note:** This project was developed as part of the Operating Systems course at **Shiv Nadar University**.
