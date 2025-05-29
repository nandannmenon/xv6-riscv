# MLFQ Scheduler for xv6

This project implements a **Multi-Level Feedback Queue (MLFQ)** scheduler for the [xv6](https://pdos.csail.mit.edu/6.828/2021/xv6.html) operating system. The aim is to enhance xv6’s default Round Robin scheduler by dynamically prioritizing I/O-bound processes and preventing starvation of CPU-bound processes through an aging mechanism.

## Features

- ✅ Multiple priority queues  
- ✅ Aging mechanism to prevent starvation  
- ✅ Dynamic priority adjustment based on time slices and wait time  
- ✅ Better responsiveness for I/O-bound processes  
- ✅ Queue monitoring for debugging  

## Motivation

The default Round Robin scheduler in xv6 lacks differentiation between interactive and CPU-bound tasks. Our implementation addresses this by:
- Prioritizing short and I/O-bound jobs
- Penalizing long CPU-bound processes
- Gradually aging waiting processes to avoid starvation

## Design & Implementation

### Key Modifications

- `struct proc`: Added `priority`, `time_slices_used`, and `wait_time`
- `scheduler()`: Modified to select processes based on queue priority
- `update_priorities()`: Implements aging by promoting long-waiting processes

### Parameters

```c
#define NUM_QUEUES 3
#define AGING_THRESHOLD 15
#define TIME_SLICE {5, 10, 15}  // Time slices for each queue level


Scheduling logic
//On process selection
p = select_process();
run_process(p, TIME_SLICE[p->priority]);

// On time slice exhaustion
if (p->time_slices_used >= TIME_SLICE[p->priority]) {
    p->time_slices_used = 0;
    if (p->priority < NUM_QUEUES - 1)
        p->priority++;
}

Aging Mechanism
Processes waiting too long are promoted to a higher priority queue:

if (p->wait_time >= AGING_THRESHOLD) {
    p->priority--;
    p->wait_time = 0;
}

Test Programs
cpu_bound.c: Simulates a CPU-intensive workload
io_bound.c: Simulates an I/O-bound workload

Running tests
$ make qemu
$ cpu_bound &
$ io_bound &

Observation
Monitor queue behaviors and process promotion/demotion via debug logs.

Results
| Metric            | Round Robin | MLFQ     |
| ----------------- | ----------- | -------- |
| Time Slice        | 15 ticks    | 5 ticks  |
| Responsiveness    | Poor        | Improved |
| Starvation Issues | Present     | Resolved |

References
xv6 Source Code Documentation
S. Baruah et al., Real-time Scheduling in Embedded Systems
J. Li & M. Kim, Efficient Scheduling for Multi-core Processors
Arpaci-Dusseau et al., Operating Systems: Three Easy Pieces
Smith College, Scheduling in Unix-like Systems
