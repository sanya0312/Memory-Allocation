#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "requirements.h"

int rampointer = 16;

void initializeRAM(RAM *ram) {
    for (int i = 0; i < 16; i++){
        ram->frames[i] = 0; // assigned to os
    }
    for (int i = 16; i < NFRAMES; i++) {
        ram->frames[i] = -1; // initializing all frames to free
    }
}

//calculate p_size
int calculatePages(int process_size) {
    return ((process_size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

void printpagetable(Process *process){
    for(int i = 0; i < process->num_pages; i++){
        printf("%d %d\n", i, process->page_table[i]);
    }
}

Process *createProcess(int process_id, int arrival_time, int process_size, int execution_time) {
    Process *new_process = (Process *)malloc(sizeof(Process));
    new_process->process_id = process_id;
    new_process->arrival_time = arrival_time;
    new_process->process_size = process_size;
    new_process->p_size = calculatePages(process_size);
    new_process->num_pages = new_process->p_size / PAGE_SIZE;
    new_process->execution_time = execution_time;
    printf("numpages %d\n", new_process->num_pages);

    new_process->page_table = (int *)malloc(new_process->num_pages * sizeof(int));
    // printpagetable(new_process);
    return new_process;
}

void enqueueProcess(Queue *queue, Process *process) {
    QueueNode *new_node = (QueueNode *)malloc(sizeof(QueueNode));
    new_node->process = process;
    new_node->next = NULL;
    if(queue->rear == NULL){
        queue->front = new_node;
        queue->rear = new_node;
    }else{
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


// void printqueue(Queue *qu){
//     while(qu->front != NULL){
//         QueueNode* temp = qu->front;
//         Process *process = temp->process;
//         int id = process->process_id;
//         printf("id = %d\n", id);
//         dequeueProcess(qu);
//     }
// }

void displayMemoryStats(int *frames, int totalFrames, Queue *queue) {
    printf("\n--- Memory Allocation Stats ---\n");
    printf("Frame Number | Memory Status | Page Number | Process ID\n");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < totalFrames; i++) {
        if(frames[i] == -1) {
            printf("%12d | Free          | N/A        | N/A\n", i);
        }else{
            QueueNode *current = queue->front;
            while (current != NULL) {
                for (int j = 0; j < current->process->num_pages; j++) {
                    if (current->process->page_table[j] == i) {
                        printf("%12d | %10d | %10d\n", i, current->process->process_id, j);
                        break;
                    }
                }
                current = current->next;
            }
        }
    }
    printf("-------------------------------------------------------\n");
}

int allocateMemory(RAM *ram, Process *process) {
    int frames_needed = process->num_pages;
    int start_frame = -1;

    // Check contiguous
    printf("frames needed %d \n", frames_needed);

    for(int i = 0; i <= NFRAMES - frames_needed; i++){
        int j;
        for(j = 0; j < frames_needed; j++) {
            if(ram->frames[i + j] != -1) break; // Not contiguous
        }
        if(j == frames_needed){ //contiguous
            start_frame = i;
            for(j = 0; j < frames_needed; j++){
                ram->frames[i + j] = process->process_id;
                // process->page_table[j].page_number = j;
                process->page_table[j] = i + j;
            }
            printpagetable(process);
            printf("starting %d ending %d",start_frame, rampointer);
            return 1; 
        }
    }
    //non-contiguous
    int allocated_frames = 0;
    for(int i = 0; i < NFRAMES && allocated_frames < frames_needed; i++){
        if(ram->frames[i] == -1){ 
            ram->frames[i] = process->process_id;
            // process->page_table[allocated_frames].page_number = allocated_frames;
            process->page_table[allocated_frames] = i;
            allocated_frames++;
        }
        printpagetable(process);
        printf("starting %d ending %d",start_frame, rampointer);
    }

    return (allocated_frames == frames_needed) ? 1 : 0; 
}

void freeMemory(RAM *ram, Process *process) {
    for (int i = 0; i < NFRAMES; i++) {
        if (ram->frames[i] == process->process_id) {
            ram->frames[i] = -1;
        }
    }
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
        displayMemoryStats(ram->frames, NFRAMES, queue);
        printf("Executing process number %d from time %d to %d\n", process->process_id,current_time,current_time+process->execution_time);
        current_time++;
        freeMemory(ram, process);
        free(process->page_table);
        free(process);
        // displayMemoryStats(ram->frames, NFRAMES, queue);
    }
}

int main() {
    RAM ram;
    initializeRAM(&ram);
    Queue queue = {NULL, NULL};
    while (1) {
        int process_id, arrival_time, execution_time, process_size;
        char input[10];
        printf("Enter the details (id, arrival_time, size, completion_time) or type 'exit': ");
        scanf("%s", input);
        if(strcmp(input, "exit") == 0) break;
        process_id = atoi(input);
        scanf("%d %d %d", &arrival_time, &process_size, &execution_time);
        Process *new_process = createProcess(process_id, arrival_time, process_size, execution_time);
        enqueueProcess(&queue, new_process);
    }
    // printqueue(&queue);
    simulateProcesses(&queue, &ram);
    return 0;
}
