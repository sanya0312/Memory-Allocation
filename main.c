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

    char input[10];  // To store user input for exit command

    while (1) {  // Infinite loop to take input until the user decides to exit
        int arrival_time, process_size, execution_time;

        printf("\nEnter details for a new process or type 'exit' to stop:\n");

        printf("Arrival Time (or type 'exit' to stop): ");
        scanf("%s", input);
        if (strcmp(input, "exit") == 0) break;  // Exit the loop if 'exit' is typed
        arrival_time = atoi(input);  // Convert input to integer if not 'exit'

        printf("Process Size (in KB): ");
        scanf("%d", &process_size);

        printf("Execution Time (in seconds): ");
        scanf("%d", &execution_time);

        // Create a new process and add it to the queue
        Process *process = createProcess(arrival_time, process_size, execution_time);
        enqueueProcess(&queue, process);
    }

    // Simulate processes if there are any in the queue
    if (queue.front != NULL) {
        printf("\nStarting simulation...\n");
        simulateProcesses(&queue, &ram);
        printf("Simulation completed.\n");
    } else {
        printf("No processes to simulate.\n");
    }

    return 0;
}
