#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "requirements.h"

int rampointer = 16;
int last_process_id = 0;

void initializeRAM(RAM *ram) {
    for (int i = 0; i < 16; i++) {
        ram->frames[i].process_id = 0;  // Assigned to OS
        ram->frames[i].page_number = -1;
    }
    for (int i = 16; i < NFRAMES; i++) {
        ram->frames[i].process_id = -1;  // Free frame
        ram->frames[i].page_number = -1;
    }
}

// Calculate p_size
int calculatePages(int process_size) {
    return ((process_size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

void printpagetable(Process *process) {
    for (int i = 0; i < process->num_pages; i++) {
        printf("%d %d\n", i, process->page_table[i]);
    }
}

Process* createProcess(int arrival_time, int process_size, int execution_time) {
    Process *new_process = (Process *)malloc(sizeof(Process));
    new_process->process_id = ++last_process_id;  // Assign a unique process ID automatically
    new_process->arrival_time = arrival_time;
    new_process->process_size = process_size;
    new_process->p_size = (process_size + PAGE_SIZE - 1) / PAGE_SIZE; // Round up page size
    new_process->num_pages = new_process->p_size;
    new_process->execution_time = execution_time;
    new_process->page_table = (int *)malloc(new_process->num_pages * sizeof(int));
    return new_process;
}

void enqueueProcess(Queue *queue, Process *process) {
    QueueNode *new_node = (QueueNode *)malloc(sizeof(QueueNode));
    new_node->process = process;
    new_node->next = NULL;
    if(queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    printf("my process with process id %d is added to the queue: ", process->process_id);
    printf("queue ka front node = %d", queue->front->process->process_id);
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

void printQueue(Queue *queue){
    QueueNode *start = queue->front;
    while(start != NULL){
        printf("queue ka front node = %d", queue->front->process->process_id);
        start = start->next;
    }
}

void displayMemoryStats(RAM *ram, int totalFrames, Queue *queue) {
    printf("\n--- Memory Allocation Stats ---\n");
    printf("Frame Number | Memory Status | Page Number | Process ID\n");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < totalFrames; i++) {
        if(ram->frames[i].process_id == -1) {
            printf("%12d | FREE          |        N/A |        N/A\n", i);
        } else {
            printf("%12d | Allocated     | %10d | %10d\n", i, ram->frames[i].page_number, ram->frames[i].process_id);
        }
    }
    printf("-------------------------------------------------------\n");
}

int allocateMemory(RAM *ram, Process *process) {
    int frames_needed = process->num_pages;
    int start_frame = -1;

    // Check contiguous
    for(int i = 0; i <= NFRAMES - frames_needed; i++) {
        int j;
        for(j = 0; j < frames_needed; j++) {
            if(ram->frames[i + j].process_id != -1) break; // Not contiguous
        }
        if(j == frames_needed) { // Contiguous
            start_frame = i;
            for(j = 0; j < frames_needed; j++) {
                ram->frames[i + j].process_id = process->process_id;
                ram->frames[i + j].page_number = j;
                process->page_table[j] = i + j;
            }
            printpagetable(process);
            return 1; 
        }
    }

    // Non-contiguous
    int allocated_frames = 0;
    for(int i = 0; i < NFRAMES && allocated_frames < frames_needed; i++) {
        if(ram->frames[i].process_id == -1) { 
            ram->frames[i].process_id = process->process_id;
            ram->frames[i].page_number = allocated_frames;
            process->page_table[allocated_frames] = i;
            allocated_frames++;
        }
    }

    return (allocated_frames == frames_needed) ? 1 : 0; 
}

void freeMemory(RAM *ram, Process *process) {
    // Iterate through all frames and free the frames belonging to the given process
    for (int i = 0; i < NFRAMES; i++) {
        if (ram->frames[i].process_id == process->process_id) {
            ram->frames[i].process_id = -1;  // Mark as free
            ram->frames[i].page_number = -1; // Reset page number
        }
    }
    printf("Memory has been freed for Process ID: %d\n", process->process_id);
}

void simulateProcesses(Queue *queue, RAM *ram) {
    int current_time = 0;
    
    while (queue->front != NULL) {
        Process *process = dequeueProcess(queue);

        if (process->arrival_time > current_time) {
            current_time = process->arrival_time;
        }
        if (!allocateMemory(ram, process)) {
            printf("Memory allocation failed for process %d\n", process->process_id);
            continue;
        }
        displayMemoryStats(ram, NFRAMES, queue);
        printf("Executing process number %d from time %d to %d\n", process->process_id, current_time, current_time + process->execution_time);
        current_time++;
        freeMemory(ram, process);
        free(process->page_table);
        free(process);
    }
}

int main() {
    Queue queue; 
    queue.front = NULL;
    queue.rear = NULL;

    RAM ram; 
    initializeRAM(&ram);  

    int choice;
    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Create Process\n");
        printf("2. Allocate Memory\n");
        printf("3. Free Memory\n");
        printf("4. Display Memory Stats\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int arrival_time, process_size, execution_time;
            printf("Enter arrival time: ");
            scanf("%d", &arrival_time);
            printf("Enter process size: ");
            scanf("%d", &process_size);
            printf("Enter execution time: ");
            scanf("%d", &execution_time);
            
            // automatic assignment of process-id
            Process *new_process = createProcess(arrival_time, process_size, execution_time);
            enqueueProcess(&queue, new_process);  // Add process to queue
            printf("Process created with ID: %d\n", new_process->process_id);

        } else if (choice == 2) {
            // queue empty or note
            if (queue.front == NULL) {
                printf("No process in the queue to allocate memory.\n");
            } else {
                Process *process = queue.rear->process;
                if (allocateMemory(&ram, process)) {
                    printf("Memory allocated to Process ID: %d\n", process->process_id);
                } else {
                    printf("Failed to allocate memory to Process ID: %d\n", process->process_id);
                }
            }
        } else if (choice == 3) {
            int process_id;
            printf("Enter process ID to free memory: ");
            scanf("%d", &process_id);

            // find process with given process_id
            QueueNode *current = queue.front;
            QueueNode *prev = NULL;  

            // process found flag
            int found = 0; 

            while (current != NULL) {
                // printf("hi");
                if (current->process->process_id == process_id) {
                    // Process found
                    Process *process_to_free = current->process;
                    // printf("hello\n");
                    // Remove process from queue
                    if (prev == NULL) {
                        queue.front = current->next;
                    } else {
                        prev->next = current->next;
                    }
                    if (current == queue.rear) {
                        queue.rear = prev;
                    }

                    // Free memory in RAM aur fir process
                    freeMemory(&ram, process_to_free);
                    free(process_to_free->page_table);  // Free pagetable memory
                    free(process_to_free);  // Free process structure memory
                    free(current);  // Free queue node memory

                    printf("Process with ID %d has been removed from the queue and memory freed.\n", process_id);
                    found = 1;  // process found
                    break;
                }
                prev = current;
                current = current->next;
            }

            if (!found) {
                printf("Process with ID %d not found in the queue.\n", process_id);
            }
        } else if (choice == 4) {
            displayMemoryStats(&ram, NFRAMES, &queue);
        } else if (choice == 5) {
            printf("Exiting the program.\n");
            break;
        } else if(choice == 6){
            printQueue(&queue);
        }
        else {
            printf("Invalid choice! Please enter a valid option.\n");
        }
    }

    return 0;
}

// int main() {
//     RAM ram;
//     initializeRAM(&ram);
//     Queue queue = {NULL, NULL};
//     while (1) {
//         int process_id, arrival_time, execution_time, process_size;
//         char input[10];
//         printf("Enter the details (id, arrival_time, size, completion_time) or type 'exit': ");
//         scanf("%s", input);
//         if(strcmp(input, "exit") == 0) break;
//         process_id = atoi(input);
//         scanf("%d %d %d", &arrival_time, &process_size, &execution_time);
//         Process *new_process = createProcess(process_id, arrival_time, process_size, execution_time);
//         enqueueProcess(&queue, new_process);
//     }
//     simulateProcesses(&queue, &ram);
//     return 0;
// }
