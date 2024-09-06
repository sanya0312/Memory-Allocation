# Virtual Process Simulator

## Overview

The Virtual Memory Simulator is a simulation tool that demonstrates memory management techniques used in operating systems. It manages processes and allocates memory dynamically, simulating both contiguous and non-contiguous memory allocation methods.

## Components

1. **Memory Management**
   - **RAM Structure**: Represents the memory with an array of frames.
   - **Process Structure**: Represents processes with attributes like arrival time, size, execution time, and page table.
   - **Memory Allocation**: Handles both contiguous and non-contiguous memory allocations.

2. **Process Management**
   - **Process Creation**: Creates a new process with a unique ID and attributes.
   - **Queue Management**: Manages processes in a queue, including adding and removing processes.

3. **Simulation**
   - **Memory Stats Display**: Shows the current memory allocation status.
   - **Queue State Display**: Prints the state of the process queue.
   - **Simulation Execution**: Simulates the processes over time, allocating and freeing memory as processes arrive and complete.

## Standard Libraries

The project makes use of the following standard libraries:

- **<stdio.h>**: Provides the core input and output functions such as `printf` and `scanf`.
- **<stdlib.h>**: Includes functions for memory allocation (`malloc`, `free`), process control (`exit`), and other utility functions.
- **<string.h>**: Contains functions for manipulating strings and memory, for eg. `strcmp`.
- **<unistd.h>**: Used for functions like `sleep()` to add delays during the simulation.

## Files

- **`requirements.h`**: Header file containing declarations of data structures and functions.
- **`requirements.c`**: Contains the main logic and implementations of the functions declared in `requirements.h`, including process creation, memory allocation, and simulation.
- **`main.c`**: Serves as the entry point for the program and handles calling functions for process creation, memory management, and simulation.

## Compilation and Execution

### Compilation

To compile the program, use the following command:

```sh
gcc -o simulator main.c -L. myLibrary.a
```

## Execution

To run the simulation, execute the compiled binary:

```sh
./simulator
```

## Testing

A test file is included to validate the simulation's functionality. It contains two test functions:

- **Test1**: Simulates processes with various sizes and arrival times that shows the complete simulation.
- **Test2**: Tests with a range of process sizes and execution times, including edge cases which includes failure of memory allocation.

Compile and run the test file to ensure the accuracy of the simulation.

```sh
gcc -o simulatedTest test.c -L. myLibrary.a
./simulatedTest
```

## Usage

### Initialization

- The RAM is initialized with a specified number of frames, some of which are reserved for the operating system.

### Process Creation

- Enter the arrival time, process size, and execution time for each new process.
- The process is created and added to the queue.

### Memory Allocation

- The simulation attempts to allocate memory for processes as they arrive.
- Processes are allocated memory using both contiguous and non-contiguous methods.

### Simulation

- The simulation runs by processing each process in the queue.
- For each second, the simulation:
  - Frees memory for completed processes.
  - Attempts to allocate memory for new processes.
  - Displays memory stats and the queue state.
- The simulation continues until all processes are completed or a time limit is reached.

## Functions

- `initializeRAM(RAM *ram)`: Initializes the RAM with free and OS-occupied frames.
- `createProcess(int arrival_time, int process_size, int execution_time)`: Creates a new process and assigns it a unique ID.
- `enqueueProcess(Queue *queue, Process *process)`: Adds a process to the queue.
- `dequeueProcess(Queue *queue)`: Removes and returns a process from the queue.
- `allocateMemory(RAM *ram, Process *process)`: Allocates memory to a process using both contiguous and non-contiguous methods.
- `freeMemory(RAM *ram, Process *process)`: Frees memory allocated to a process.
- `displayMemoryStats(RAM *ram)`: Displays the current status of memory allocation.
- `simulateProcesses(Queue *all_queue, Queue *allocated_queue, RAM *ram)`: Runs the simulation of processes over time.
- `printQueue(Queue *queue)`: Prints the current state of the process queue.

## Example

1. **Run the Program**: Execute the program to start the input loop.
2. **Enter Process Details**: Provide arrival time, process size, and execution time for each process.
3. **Simulation**: After adding all processes, the simulation will start, showing memory allocation and queue status updates.

## Future Scope

The project can be further enhanced by incorporating the following features:
* **Different Types of Process Scheduling**: Implementing various scheduling algorithms such as Round Robin, Shortest Job First (SJF), Priority Scheduling, etc., to simulate a more realistic OS environment.
* **Page Swapping with Different Page Replacement Algorithms**: Introducing the concept of page swapping to handle memory management more dynamically. Future implementations can include additional page replacement algorithms like Least Recently Used (LRU), Optimal Page Replacement.

## Notes

- The simulation uses a fixed number of frames (1024 KB) and page size (8 KB).
- Contiguous allocation is attempted first; if it fails, non-contiguous allocation is used.
- Memory allocation failures are handled by cleaning up any allocated memory and continuing the simulation.

