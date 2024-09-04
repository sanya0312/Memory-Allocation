#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "requirements.h"

int main() {
    Queue queue;
    queue.front = NULL;
    queue.rear = NULL;

    RAM ram;
    initializeRAM(&ram);

    char input[10]; 

    while (1) {
        int arrival_time, process_size, execution_time;

        printf("\nEnter details for a new process or type 'exit' to stop:\n");

        printf("Arrival Time (or type 'exit' to stop): ");
        scanf("%s", input);
        if (strcmp(input, "exit") == 0) break; 
        arrival_time = atoi(input);  

        printf("Process Size (in KB): ");
        scanf("%d", &process_size);

        printf("Execution Time (in seconds): ");
        scanf("%d", &execution_time);

        // Create process
        Process *process = createProcess(arrival_time, process_size, execution_time);
        // Add process to queue
        enqueueProcess(&queue, process);
    }

    // Simulate processes
    if (queue.front != NULL) {
        printf("\nStarting simulation...\n");
        simulateProcesses(&queue, &ram);
        printf("Simulation completed.\n");
    } else {
        printf("No processes to simulate.\n");
    }

    return 0;
}
