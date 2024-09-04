#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep()
#include "requirements.h"

int last_process_id = 0;
int current_time = 0;

void initializeRAM(RAM *ram) {
    for(int i = 0; i < 16; i++){
        ram->frames[i].process_id = 0; // Assigned to OS
        ram->frames[i].page_number = -1;
    }
    for (int i = 16; i < NFRAMES; i++) {
        ram->frames[i].process_id = -1; // Free frame
        ram->frames[i].page_number = -1;
    }
}

int calculatePages(int process_size) {
    return ((process_size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

Process* createProcess(int arrival_time, int process_size, int execution_time) {
    Process *new_process = (Process *)malloc(sizeof(Process));
    new_process->process_id = ++last_process_id;  // Assign a unique process ID automatically
    new_process->arrival_time = arrival_time;
    new_process->process_size = process_size;
    new_process->p_size = (process_size + PAGE_SIZE - 1) / PAGE_SIZE; // Round up page size
    new_process->num_pages = new_process->p_size;
    new_process->execution_time = execution_time;
    new_process->completion_time = arrival_time + execution_time;  // Time when the process completes
    new_process->page_table = (int *)malloc(new_process->num_pages * sizeof(int));
    return new_process;
}

void enqueueProcess(Queue *queue, Process *process) {
    QueueNode *new_node = (QueueNode *)malloc(sizeof(QueueNode));
    new_node->process = process;
    new_node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

Process *dequeueProcess(Queue *queue) {
    if (queue->front == NULL) return NULL;
    QueueNode *temp = queue->front;
    Process *process = temp->process;
    queue->front = queue->front->next;
    if (queue->front == NULL) queue->rear = NULL;
    free(temp);
    return process;
}

void displayMemoryStats(RAM *ram) {
    printf("\n--- Memory Allocation Stats at Time: %d ---\n", current_time);
    printf("Frame Number | Memory Status | Page Number | Process ID\n");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < NFRAMES; i++) {
        if (ram->frames[i].process_id == -1) {
            printf("%12d | FREE          |        N/A |        N/A\n", i);
        } else {
            printf("%12d | Allocated     | %10d | %10d\n", i, ram->frames[i].page_number, ram->frames[i].process_id);
        }
    }
    printf("-------------------------------------------------------\n");
}

int allocateMemory(RAM *ram, Process *process) {
    int frames_needed = process->num_pages;
    int allocated_frames = 0;

    // Check contiguous allocation
    for (int i = 0; i <= NFRAMES - frames_needed; i++) {
        int j;
        for (j = 0; j < frames_needed; j++) {
            if (ram->frames[i + j].process_id != -1) break; // Not contiguous
        }
        if (j == frames_needed) { // Contiguous
            for (j = 0; j < frames_needed; j++) {
                ram->frames[i + j].process_id = process->process_id;
                ram->frames[i + j].page_number = j;
                process->page_table[j] = i + j;
            }
            return 1; 
        }
    }

    // Non-contiguous allocation
    int frames_allocated = 0;
    for (int i = 0; i < NFRAMES && frames_allocated < frames_needed; i++) {
        if (ram->frames[i].process_id == -1) { 
            ram->frames[i].process_id = process->process_id;
            ram->frames[i].page_number = frames_allocated;
            process->page_table[frames_allocated] = i;
            frames_allocated++;
        }
    }

    if (frames_allocated < frames_needed) {
        // Allocation failed
        printf("Memory allocation failed for process %d. Not enough free frames available.\n", process->process_id);
        freeMemory(ram, process); // Clean up any allocated frames
        return 0; // Failure
    }
    return 1; // Success
}

void freeMemory(RAM *ram, Process *process) {
    for (int i = 0; i < NFRAMES; i++) {
        if (ram->frames[i].process_id == process->process_id) {
            ram->frames[i].process_id = -1;  // Mark as free
            ram->frames[i].page_number = -1; // Reset page number
        }
    }
    printf("Memory has been freed for Process ID: %d\n", process->process_id);
}

void printQueueState(Queue *queue) {
    QueueNode *current_node = queue->front;
    printf("Queue state:\n");
    while (current_node != NULL) {
        printf("Process ID: %d\n", current_node->process->process_id);
        current_node = current_node->next;
    }
}

void simulateProcesses(Queue *queue, RAM *ram) {
    while (queue->front != NULL && current_time <= 100) {  // Run simulation until queue is empty or time limit reached
        printf("\nCurrent Time: %d\n", current_time);

        // Allocate memory for processes arriving at current time
        QueueNode *current_node = queue->front;
        while (current_node != NULL) {
            Process *process = current_node->process;
            if (process->arrival_time == current_time) {
                printf("Attempting to allocate memory for Process ID: %d\n", process->process_id);
                if (allocateMemory(ram, process)) {
                    printf("Memory allocated to Process ID: %d\n", process->process_id);
                } else {
                    printf("Memory allocation failed for Process ID: %d\n", process->process_id);
                    // Dequeue the failed process and free it
                    // Process* failed_process = dequeueProcess(queue);
                    // if (failed_process != NULL) {
                    //     free(failed_process->page_table);
                    //     free(failed_process);
                    // }
                }
            }
            current_node = current_node->next;
        }

        // Free memory for processes completing at current time
        current_node = queue->front;
        QueueNode *prev_node = NULL;
        while (current_node != NULL) {
            Process *process = current_node->process;
            if (process->completion_time == current_time) {
                printf("Freeing memory for Process ID: %d\n", process->process_id);
                freeMemory(ram, process);
                free(process->page_table);
                Process* completed_process = dequeueProcess(queue);  // Remove the completed process from the queue
                if (completed_process != NULL) {
                    free(completed_process);
                }
                current_node = (prev_node != NULL) ? prev_node->next : queue->front;
            } else {
                prev_node = current_node;
                current_node = current_node->next;
            }
        }

        // Display memory stats at each second
        displayMemoryStats(ram);

        // Print queue state for debugging
        printQueueState(queue);

        sleep(1);  // Simulate 1 second delay
        current_time++;  // Increment simulated time
    }
}

