#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 4   
#define RAM_SIZE 128 

// page table entry ka structure
typedef struct {
    int page_number;  // Page number of the process
    int frame_number; // Frame number in RAM to which the page is mapped
} PageTableEntry;

// process details
typedef struct {
    int process_id;          
    int arrival_time;        // Time when the process arrives
    int process_size;        // Actual size of the process
    int p_size;              // Size rounded up to the nearest page size multiple
    int num_pages;           
    int execution_time;      // Time required to complete the process
    PageTableEntry *page_table;  // Array for page table entries
} Process;


typedef struct {
    int frames[RAM_SIZE]; // Array to represent frames in RAM
} RAM;

// Queue node structure for FIFO scheduling
typedef struct QueueNode {
    Process *process;
    struct QueueNode *next;
} QueueNode;

// Queue structure for handling processes in FIFO order
typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;
