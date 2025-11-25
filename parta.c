#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Allocates and initializes an array of PCBs on the heap.
 * Each PCB gets:
 *   - pid = index
 *   - burst_left = bursts[i]
 *   - wait = 0
 */
struct pcb* init_procs(int* bursts, int blen) {
    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    for (int i = 0; i < blen; i++) 
    {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }
    return procs;
}

/**
 * Debug helper: prints all PCB fields.
 * Not used in tests.
 */
void printall(struct pcb* procs, int plen) {
     for (int i = 0; i < plen; i++) 
     {
        printf("PID %d: burst_left=%d wait=%d\n",
               procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * Runs `current` process for `amount`.
 *   - current burst_left -= amount (but not below 0)
 *   - all OTHER processes with burst_left > 0 increase wait by amount
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    if (procs[current].burst_left < amount) 
    {
        amount = procs[current].burst_left;
    }

    procs[current].burst_left -= amount;

    for (int i = 0; i < plen; i++) 
    {
        if (i != current && procs[i].burst_left > 0) 
        {
            procs[i].wait += amount;
        }
    }
}

/**
 * Runs FCFS scheduling starting at pid 0.
 * Each process runs to completion before the next begins.
 * Returns total elapsed time.
 */
int fcfs_run(struct pcb* procs, int plen) {
    int time = 0;

    for (int i = 0; i < plen; i++) 
    {
        int burst = procs[i].burst_left;

        run_proc(procs, plen, i, burst);
        time += burst;
    }

    return time;
}

/**
 * Returns the next pid to run in Round Robin.
 *
 * Rules:
 *   - Cycle through processes (0 → 1 → 2 → ... → 0)
 *   - Skip any with burst_left == 0
 *   - If all are done, return -1
 */
int rr_next(int current, struct pcb* procs, int plen) {
    int all_done = 1;
    for (int i = 0; i < plen; i++) 
    {
        if (procs[i].burst_left > 0) all_done = 0;
    }
    if (all_done) return -1;

    if (current == -1) 
    {
        for (int i = 0; i < plen; i++) 
        {
            if (procs[i].burst_left > 0) return i;
        }
        return -1;
    }

    int next = (current + 1) % plen;

    while (procs[next].burst_left == 0)
    {
        next = (next + 1) % plen;
    }

    return next;
}

/**
 * Runs Round-Robin scheduling starting at pid 0.
 * Each process runs for min(quantum, burst_left).
 * Returns total time elapsed.
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    int time = 0;

    int current = 0;

    if (procs[0].burst_left == 0) 
    {
        current = rr_next(0, procs, plen);
        if (current == -1) return 0;
    }

    while (1) 
    {
        if (procs[current].burst_left == 0) 
        {
            current = rr_next(current, procs, plen);
            if (current == -1) break;
        }

        int amt = (procs[current].burst_left < quantum)
                    ? procs[current].burst_left
                    : quantum;

        run_proc(procs, plen, current, amt);
        time += amt;

        current = rr_next(current, procs, plen);
        if (current == -1) break;
    }

    return time;
}