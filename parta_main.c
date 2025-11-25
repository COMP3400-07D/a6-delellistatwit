#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

/*
Command-line interface for FCFS and Round-Robin scheduling.
 *
 * Usage:
 *   ./parta_main fcfs 5 8 2
 *   ./parta_main rr 2 5 8 2
 *
 * Prints:
 *   - Scheduler type (FCFS or RR(q))
 *   - Accepted processes and their bursts
 *   - Average wait time (2 decimals)
 *
 * Errors:
 *   If missing args → print error and exit(1)
 */
int main(int argc, char* argv[]) {

    if (argc < 3) 
    {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    int is_fcfs = strcmp(argv[1], "fcfs") == 0;
    int is_rr   = strcmp(argv[1], "rr") == 0;

    if (!is_fcfs && !is_rr) 
    {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    int quantum = 0;
    int burst_start_index;

    if (is_fcfs) 
    {
        if (argc < 3) 
        {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        printf("Using FCFS\n\n");
        burst_start_index = 2;
    }

    else 
    {
        if (argc < 4) 
        {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        quantum = atoi(argv[2]);
        printf("Using RR(%d)\n\n", quantum);
        burst_start_index = 3;
    }

    int num_bursts = argc - burst_start_index;

    if (num_bursts <= 0) 
    {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    int* bursts = malloc(sizeof(int) * num_bursts);

    for (int i = 0; i < num_bursts; i++)
    {
        bursts[i] = atoi(argv[burst_start_index + i]);
        printf("Accepted P%d: Burst %d\n", i, bursts[i]);
    }

    struct pcb* procs = init_procs(bursts, num_bursts);

    int total_time;

    if (is_fcfs)
        total_time = fcfs_run(procs, num_bursts);
    else
        total_time = rr_run(procs, num_bursts, quantum);

    double avg_wait = 0.0;

    for (int i = 0; i < num_bursts; i++)
        avg_wait += procs[i].wait;

    avg_wait /= num_bursts;

    printf("Average wait time: %.2f\n", avg_wait);

    free(bursts);
    free(procs);

    return 0;
}