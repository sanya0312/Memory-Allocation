#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 8  
#define RAM_SIZE 1024 
#define NFRAMES RAM_SIZE/PAGE_SIZE
// page table entry ka structure
typedef struct {
    int page_number;  // Page number of the process
    int frame_number; // Frame number in RAM to which the page is mapped
} PageTableEntry;

// process details
typedef struct {
    int process_id;          
    int arrival_time;        // Arrival time
    int process_size;        // Actual process size
    int p_size;              // calculated size
    int num_pages;           
    int execution_time;      // Completion time
    PageTableEntry *page_table;  // page table entry array
} Process;


typedef struct {
    int frames[NFRAMES]; // frames in RAM array
} RAM;

// Queue for FIFO
typedef struct QueueNode {
    Process *process;
    struct QueueNode *next;
} QueueNode;

// Queue for processes
typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;
