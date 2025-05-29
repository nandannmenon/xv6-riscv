# 🧠 MLFQ Scheduler for xv6

This project implements a **Multi-Level Feedback Queue (MLFQ)** scheduler for the [xv6](https://pdos.csail.mit.edu/6.828/2021/xv6.html) operating system. The goal is to enhance xv6’s default Round Robin scheduler by dynamically prioritizing I/O-bound processes and preventing starvation of CPU-bound processes through an **aging mechanism**.

---

## ✅ Features

- Multiple priority queues  
- Dynamic priority adjustment  
- Aging mechanism to prevent starvation  
- Better responsiveness for I/O-bound processes  
- Queue monitoring and debugging support  

---

## 🎯 Motivation

The default Round Robin scheduler in xv6 treats all processes equally, which can lead to:

- Poor responsiveness for interactive tasks  
- Starvation of long-running background tasks  

### Our Solution: MLFQ Scheduler

- Prioritizes short and I/O-bound jobs  
- Penalizes CPU-bound processes with lower priority  
- Implements **aging** to promote long-waiting processes  
- Achieves a balance between fairness and performance  

---

## 🛠️ Design & Implementation

### 🔧 Key Modifications

- **`struct proc`** updated to include:
  - `int priority` – queue level (0 = highest)
  - `int time_slices_used` – how many ticks the process has used
  - `int wait_time` – tracks how long a process has waited

- **`scheduler()`** modified to:
  - Select processes based on priority
  - Adjust time slices per queue level
  - Requeue or promote/demote processes accordingly

- **`update_priorities()`** added to implement aging

---

### ⚙️ Parameters

```c
#define NUM_QUEUES 3
#define AGING_THRESHOLD 15
#define TIME_SLICE {5, 10, 15}  // Queue 0: 5 ticks, Queue 1: 10, Queue 2: 15
```

---

## 📜 Scheduling Logic

### 🧮 Process Selection

```c
p = select_process();
run_process(p, TIME_SLICE[p->priority]);
```

### ⏳ On Time Slice Exhaustion

```c
if (p->time_slices_used >= TIME_SLICE[p->priority]) {
    p->time_slices_used = 0;
    if (p->priority < NUM_QUEUES - 1)
        p->priority++;
}
```

---

## ⏫ Aging Mechanism

To prevent starvation, processes waiting too long are promoted:

```c
if (p->wait_time >= AGING_THRESHOLD) {
    p->priority--;
    p->wait_time = 0;
}
```

---

## 🧪 Test Programs

### `cpu_bound.c`

Simulates a CPU-intensive workload with heavy computation in a tight loop.

### `io_bound.c`

Simulates an I/O-bound workload using `sleep()` to mimic I/O wait.

---

## ▶️ Running the Scheduler

### Step-by-Step

```bash
$ make qemu             # Compile and start xv6
$ cpu_bound &           # Run CPU-bound process in background
$ io_bound &            # Run I/O-bound process in background
```

### Observation

- Monitor logs for queue status
- Observe aging and priority changes in real time
- Compare responsiveness and starvation handling vs Round Robin

---

## 📊 Results

| Metric            | Round Robin | MLFQ         |
|-------------------|-------------|--------------|
| Time Slice        | 15 ticks    | 5–15 ticks   |
| Responsiveness    | Poor        | Improved     |
| Starvation Issues | Present     | Resolved     |

---

## 📚 References

- **xv6 Source Code Documentation**  
- S. Baruah et al., *Real-Time Scheduling in Embedded Systems*  
- J. Li & M. Kim, *Efficient Scheduling for Multi-core Processors*  
- Arpaci-Dusseau et al., *Operating Systems: Three Easy Pieces*  
- Smith College, *Scheduling in Unix-like Systems*  

---

> 🏫 Developed as part of the Operating Systems course at **Shiv Nadar University**
