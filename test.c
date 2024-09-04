#include <stdio.h>
#include <stdlib.h>
#include "requirements.h"

void Test1(){
    Queue queue;
    queue.front = NULL;
    queue.rear = NULL;

    RAM ram;
    initializeRAM(&ram);

    // Test 1: Create processes
    Process *p1 = createProcess(0, 300, 3);
    Process *p2 = createProcess(0, 200, 3);
    Process *p3 = createProcess(2, 100, 2);
    Process *p4 = createProcess(2, 50, 4);
    Process *p5 = createProcess(4, 400, 3);

    enqueueProcess(&queue, p1);
    enqueueProcess(&queue, p2);
    enqueueProcess(&queue, p3);
    enqueueProcess(&queue, p4);
    enqueueProcess(&queue, p5);

    // Simulate processes
    printf("Starting simulation...\n");
    simulateProcesses(&queue, &ram);

    printf("Simulation completed.\n");
}

void Test2(){
    Queue queue;
    queue.front = NULL;
    queue.rear = NULL;

    RAM ram;
    initializeRAM(&ram);

    Process *p1 = createProcess(0, 500, 3);
    Process *p2 = createProcess(2, 200, 2);
    Process *p3 = createProcess(2, 150, 4);
    Process *p4 = createProcess(4, 705, 2);
    Process *p5 = createProcess(10, 500, 3);
    Process *p6 = createProcess(12, 100, 2);
    Process *p7 = createProcess(16, 1024, 1);

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
}

int main() {
    Test1();
    Test2();
    return 0;
}
