#include <stdio.h>
#include <stdlib.h>
#include "requirements.h"

int main() {
    Queue queue;
    queue.front = NULL;
    queue.rear = NULL;

    RAM ram;
    initializeRAM(&ram);

    // Test 1: Create and add processes
    Process *p1 = createProcess(0, 500, 3);
    Process *p2 = createProcess(2, 200, 2);
    Process *p3 = createProcess(2, 150, 4);
    Process *p4 = createProcess(4, 700, 2);
    Process *p5 = createProcess(10, 500, 3);
    Process *p6 = createProcess(12, 100, 2);
    Process *p7 = createProcess(16, 896, 1);

    enqueueProcess(&queue, p1);
    enqueueProcess(&queue, p2);
    enqueueProcess(&queue, p3);
    enqueueProcess(&queue, p4);
    enqueueProcess(&queue, p5);
    enqueueProcess(&queue, p6);
    enqueueProcess(&queue, p7);

    // Simulate processes
    printf("Starting simulation...\n");
    simulateProcesses(&queue, &ram);

    printf("Simulation completed.\n");
    return 0;
}
