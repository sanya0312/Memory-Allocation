#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 8  
#define RAM_SIZE 1024 
#define NFRAMES RAM_SIZE/PAGE_SIZE

extern int last_process_id;

// Frame structure to store process ID and page number
typedef struct {
    int process_id;  // ID of the process using this frame
    int page_number; // Page number within the process
} Frame;

// process details
typedef struct {
    int process_id;          
    int arrival_time;        // Arrival time
    int process_size;        // Actual process size
    int p_size;              // Calculated size
    int num_pages;           
    int execution_time;      // Completion time
    int* page_table;         // Page table entry array
} Process;

typedef struct {
    Frame frames[NFRAMES]; // Array of frames in RAM
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
