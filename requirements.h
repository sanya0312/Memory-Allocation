#ifndef REQUIREMENTS_H
#define REQUIREMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 8  
#define RAM_SIZE 1024 
#define NFRAMES RAM_SIZE/PAGE_SIZE

extern int last_process_id;

typedef struct {
    int process_id;  // ID of the process using this frame (-1 : free, 0 : Operating System)
    int page_number; 
} Frame;

// process details
typedef struct {
    int process_id;          
    int arrival_time;        // Arrival time
    int process_size;        // Actual process size
    int p_size;              // Calculated size
    int num_pages;           
    int execution_time;
    int completion_time;      // Completion time
    int* page_table;         // Page table entry array
} Process;

typedef struct {
    Frame frames[NFRAMES]; // Array of frames in RAM
} RAM;

// Nodes of queue
typedef struct QueueNode {
    Process *process;
    struct QueueNode *next;
} QueueNode;

// Queue for processes
typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

void initializeRAM(RAM *ram);

// process management
Process* createProcess(int arrival_time, int process_size, int execution_time);
void enqueueProcess(Queue *queue, Process *process);
Process *dequeueProcess(Queue *queue);

// memory management
int allocateMemory(RAM *ram, Process *process);
void freeMemory(RAM *ram, Process *process);

// simulation
void displayMemoryStats(RAM *ram);
void simulateProcesses(Queue *queue, RAM *ram);
void printQueue(Queue *queue);

#endif