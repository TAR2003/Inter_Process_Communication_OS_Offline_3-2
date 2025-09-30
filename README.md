# Inter Process Communication - Operating Systems Assignment

## Project Overview

This repository contains a comprehensive collection of C++ programs demonstrating various inter-process communication (IPC) concepts and threading mechanisms in operating systems. The project includes simulations of real-world scenarios such as museum visitor management, student report printing systems, and computational parallelism using POSIX threads, semaphores, and mutex locks.

## Repository Structure

```
Inter_Process_Communication_OS_Offline_3-2/
├── 2005090.cpp                           # Main museum visitor simulation
├── 2005090.exe                           # Compiled executable for main simulation
├── 2005090/
│   └── 2005090.cpp                       # Alternative version of main simulation
├── 2005090 (37)/
│   └── 2005090/
│       ├── 2005090.cpp                   # Version 37 of museum simulation
│       └── 2005090.exe                   # Compiled executable
│       └── 2005090/
│           └── 2005090.cpp               # Nested version
├── 2005090 (37).zip                      # Archived version 37
├── 2005090 (38)/
│   └── 2005090/
│       ├── 2005090                       # Executable (Linux format)
│       └── 2005090.cpp                   # Version 38 of museum simulation
├── 2005090 (38).zip                      # Archived version 38
├── 2005090.zip                           # Main version archive
├── a.exe                                 # General purpose executable
├── poisson_random_number_generator.cpp   # Poisson distribution random number generator
├── poisson_random_number_generator.exe   # Compiled Poisson generator
├── simple_sum_calculation.cpp            # Multi-threaded sum calculation
├── student_report_printing.cpp           # Student report printing simulation
├── temp.cpp                              # Student hall access control simulation
└── README.md                             # This documentation file
```

## Core Components

### 1. Museum Visitor Simulation (`2005090.cpp`)

The primary component of this repository is a sophisticated museum visitor management system that simulates visitor flow through different areas of a museum with capacity constraints and priority ticketing.

#### Features:
- **Multi-threaded visitor simulation** using POSIX threads
- **Priority ticketing system** with premium and standard tickets
- **Capacity-controlled areas** using semaphores:
  - Gallery 1: Limited to 5 visitors
  - Glass Corridor: Limited to 3 visitors
  - Gallery 2: Unlimited capacity
- **Sequential movement control** through museum steps using mutex locks
- **Photo booth access** with priority queuing for premium ticket holders
- **Thread-safe output** with synchronized printing
- **Poisson-distributed random delays** for realistic simulation timing

#### Parameters:
- `N`: Number of standard ticket holders
- `M`: Number of premium ticket holders
- `w`: Time spent at entrance area A
- `x`: Time spent in Gallery 1
- `y`: Time spent in Gallery 2
- `z`: Time spent in photo booth

#### Usage:
```bash
g++ -pthread 2005090.cpp -o museum_sim
./museum_sim <N> <M> <w> <x> <y> <z>
```

#### Example:
```bash
./museum_sim 10 6 2 6 6 12
```

### 2. Student Report Printing Simulation (`student_report_printing.cpp`)

This program simulates a group of students completing and printing their reports, demonstrating file I/O operations, thread synchronization, and realistic timing simulation.

#### Features:
- **Dynamic student management** with unique IDs and randomized writing times
- **Thread-safe file output** to prevent log interleaving
- **Staggered thread initialization** with random start times
- **File-based input/output** for configuration and results
- **State tracking** for student activities (writing vs. waiting for printing)

#### Usage:
```bash
g++ -pthread student_report_printing.cpp -o student_sim
./student_sim <input_file> <output_file>
```

#### Input File Format:
```
10  # Number of students
```

#### Output Format:
```
Student 1 started writing for 15 ms at 0 ms
Student 1 finished writing at 15 ms
Student 1 has arrived at the print station at 25 ms
...
```

### 3. Poisson Random Number Generator (`poisson_random_number_generator.cpp`)

A utility program demonstrating the generation of Poisson-distributed random numbers, which are used throughout the project for realistic timing simulation.

#### Features:
- **Poisson distribution implementation** using C++ standard library
- **Configurable lambda parameter** (λ = 10000.234)
- **Non-deterministic random number generation** using hardware entropy

#### Usage:
```bash
g++ poisson_random_number_generator.cpp -o poisson_gen
./poisson_gen
```

### 4. Multi-threaded Sum Calculation (`simple_sum_calculation.cpp`)

This program demonstrates parallel computation by calculating the sum of numbers from 1 to N using multiple threads, showcasing thread synchronization and workload distribution.

#### Features:
- **Parallel computation** with configurable thread count
- **Range-based work distribution** among threads
- **Thread synchronization** using pthread_join
- **Performance demonstration** of multi-threading benefits

#### Configuration:
- `N = 2,441,139`: Upper limit for sum calculation
- `M = 10`: Number of worker threads

#### Usage:
```bash
g++ -pthread simple_sum_calculation.cpp -o sum_calc
./sum_calc
```

### 5. Hall Access Control Simulation (`temp.cpp`)

A simplified simulation demonstrating basic semaphore usage for controlling access to a limited-capacity hall.

#### Features:
- **Semaphore-controlled access** to Hall A (capacity: 3 students)
- **Mutex-controlled door passage** ensuring sequential entry
- **Random arrival timing** for realistic simulation
- **Thread-safe output** for activity logging

#### Usage:
```bash
g++ -pthread temp.cpp -o hall_sim
./hall_sim
```

## Technical Implementation Details

### Synchronization Mechanisms

#### Semaphores
- `gallery1_capacity`: Controls Gallery 1 access (limit: 5)
- `glass_corridor`: Controls Glass Corridor access (limit: 3)
- `hall_a_capacity`: Controls Hall A access (limit: 3)

#### Mutex Locks
- `print_lock`: Thread-safe output operations
- `step0_lock`, `step1_lock`, `step2_lock`: Sequential movement control
- `premium_ticket_count_lock`: Premium ticket counter synchronization
- `standard_ticket_count_lock`: Standard ticket counter synchronization
- `photo_booth_mutex`: Photo booth access control
- `premium_ticket_holder_in_waiting_list`: Priority queue management
- `door_lock`: Hall door access control
- `output_lock`: File output synchronization

#### Data Structures

##### Visitor Class
```cpp
class visitor {
public:
    int visitor_id;      // Unique identifier (1001+ for standard, 2001+ for premium)
    bool premium_ticket; // Ticket type flag
};
```

##### Student Class
```cpp
class Student {
public:
    int id;              // Unique student identifier
    int writing_time;    // Random writing duration
    student_state state; // Current activity state
};
```

##### ThreadData Class
```cpp
class ThreadData {
public:
    long start;          // Range start for computation
    long end;            // Range end for computation
    long long sum;       // Computed partial sum
};
```

### Algorithm Implementations

#### Priority Queue Management
The museum simulation implements a sophisticated priority queue system for photo booth access:

1. **Premium Ticket Holders**: Get immediate access when available
2. **Standard Ticket Holders**: Must wait for all premium holders to finish
3. **Batch Processing**: Standard holders are processed in groups

#### Random Number Generation
All simulations use Poisson distribution (λ = 10000.234) for realistic timing:
- Arrival times
- Activity durations
- Thread initialization delays

#### Thread Synchronization Patterns
- **Producer-Consumer**: Student report generation and printing
- **Reader-Writer**: Shared counter management
- **Barrier Synchronization**: Sequential movement through museum areas

## Compilation Requirements

### Dependencies
- **POSIX Threads**: For multi-threading support
- **C++11 or later**: For chrono, random, and standard library features
- **GNU Compiler Collection (GCC)**: Recommended for compilation

### Compilation Commands

#### Individual Programs
```bash
# Museum simulation
g++ -pthread -std=c++11 2005090.cpp -o museum_sim

# Student report printing
g++ -pthread -std=c++11 student_report_printing.cpp -o student_sim

# Poisson random generator
g++ -std=c++11 poisson_random_number_generator.cpp -o poisson_gen

# Sum calculation
g++ -pthread -std=c++11 simple_sum_calculation.cpp -o sum_calc

# Hall simulation
g++ -pthread -std=c++11 temp.cpp -o hall_sim
```

#### Batch Compilation
```bash
# Compile all threaded programs
for file in 2005090.cpp student_report_printing.cpp simple_sum_calculation.cpp temp.cpp; do
    g++ -pthread -std=c++11 "$file" -o "${file%.cpp}"
done

# Compile non-threaded programs
g++ -std=c++11 poisson_random_number_generator.cpp -o poisson_gen
```

## Usage Examples

### Museum Simulation

#### Basic Usage
```bash
./museum_sim 15 8 3 7 5 10
```

#### Expected Output Format
```
Visitor 1001 has arrived at A at timestamp 1245
Visitor 1001 is at step 1 at timestamp 1545
Visitor 2001 has arrived at A at timestamp 1623
Visitor 1001 is at step 2 at timestamp 1747
...
Visitor 2001 is inside the photo booth at timestamp 5432
Visitor 1001 is at photo booth at timestamp 5876
...
```

### Student Report Printing

#### Input File (`input.txt`)
```
25
```

#### Execution
```bash
./student_sim input.txt output.txt
```

#### Output File Analysis
```
Student 1 started writing for 12 ms at 0 ms
Student 3 started writing for 8 ms at 1 ms
Student 1 finished writing at 12 ms
Student 3 finished writing at 9 ms
Student 1 has arrived at the print station at 20 ms
Student 3 has arrived at the print station at 18 ms
...
```

## Performance Considerations

### Memory Usage
- **Dynamic Memory Allocation**: Visitor and student objects are dynamically allocated
- **Thread Stack Space**: Each thread requires stack space (typically 8MB on Linux)
- **Synchronization Overhead**: Mutex and semaphore operations have minimal overhead

### Timing Accuracy
- **Microsecond Precision**: Using `usleep()` for precise timing control
- **Millisecond Timestamps**: High-resolution clock for event logging
- **Jitter Compensation**: Poisson distribution provides realistic variance

### Scalability
- **Thread Limits**: System-dependent thread creation limits
- **Memory Constraints**: Large visitor/student counts may require memory optimization
- **Synchronization Bottlenecks**: Critical sections should be minimized

## Configuration Parameters

### Museum Simulation Constants
```cpp
#define STEP_DELAY 2                    // Base delay for movement steps
#define SLEEP_MULTIPLIER 100000         // Microsecond multiplier
#define GALLERY1_CAPACITY 5             // Maximum Gallery 1 occupancy
#define GLASS_CORRIDOR_CAPACITY 3       // Maximum Glass Corridor occupancy
#define GLASS_CORRIDOR_SLEEP_TIME 10    // Transit time through corridor
#define RANDOM_NUMBER_MULTIPLIER 100    // Random delay scaling factor
```

### Student Simulation Constants
```cpp
#define MAX_WRITING_TIME 20             // Maximum writing duration
#define WALKING_TO_PRINTER 10           // Maximum walking time
#define SLEEP_MULTIPLIER 1000           // Millisecond multiplier
```

### Sum Calculation Constants
```cpp
long N = 2441139;                       // Upper limit for calculation
long M = 10;                            // Number of worker threads
```

## Error Handling

### Command Line Validation
All programs include argument count validation:
```cpp
if (argc != expected_count) {
    cout << "Usage: " << usage_string << endl;
    exit(0);
}
```

### File Operation Safety
Student simulation includes file buffer management:
```cpp
// Save original buffers
std::streambuf *cinBuffer = std::cin.rdbuf();
std::streambuf *coutBuffer = std::cout.rdbuf();

// Restore on exit
std::cin.rdbuf(cinBuffer);
std::cout.rdbuf(coutBuffer);
```

### Resource Cleanup
All programs properly destroy synchronization objects:
```cpp
// Destroy semaphores
sem_destroy(&gallery1_capacity);
sem_destroy(&glass_corridor);

// Destroy mutexes
pthread_mutex_destroy(&print_lock);
pthread_mutex_destroy(&step0_lock);
// ... additional cleanup
```

## Development Guidelines

### Code Style
- **Consistent Naming**: snake_case for variables, PascalCase for classes
- **Comprehensive Comments**: Detailed function and class documentation
- **Modular Design**: Separate functions for distinct operations
- **Error Prevention**: Input validation and resource management

### Threading Best Practices
- **Deadlock Avoidance**: Consistent lock ordering
- **Resource Management**: Proper cleanup in all execution paths
- **Race Condition Prevention**: Atomic operations for shared data
- **Performance Optimization**: Minimal critical section duration

### Testing Recommendations
- **Stress Testing**: Run with varying parameter combinations
- **Concurrency Testing**: Multiple execution runs to detect race conditions
- **Resource Monitoring**: Memory and thread usage validation
- **Output Verification**: Log analysis for correctness validation

## Version History

The repository contains multiple versions of the main museum simulation:

- **2005090.cpp**: Primary implementation with command-line arguments
- **2005090/ directory**: Alternative version with modified input handling
- **2005090 (37)/ directory**: Version 37 with enhanced argument validation
- **2005090 (38)/ directory**: Version 38 with refined error handling

Each version demonstrates iterative development and improvement in:
- Input parameter handling
- Error message clarity
- Code organization
- Comment documentation

## Academic Context

This project was developed as part of an Operating Systems course assignment focusing on:

- **Inter-Process Communication**: Thread-based communication patterns
- **Synchronization Mechanisms**: Practical application of semaphores and mutexes
- **Resource Management**: Controlled access to limited resources
- **Real-World Modeling**: Simulation of actual operational scenarios
- **Performance Analysis**: Multi-threading efficiency demonstration

**Author**: Nafis Tahmid (Student ID: 1905002)  
**Date**: November 10, 2024  
**Course**: Operating Systems - Level 3, Term 2

## Future Enhancements

### Potential Improvements
- **GUI Interface**: Visual representation of museum layout and visitor flow
- **Statistical Analysis**: Performance metrics and bottleneck identification
- **Configuration Files**: External parameter specification
- **Network Simulation**: Distributed system implementation
- **Real-Time Visualization**: Live monitoring dashboard

### Advanced Features
- **Priority Inheritance**: Advanced deadlock prevention
- **Load Balancing**: Dynamic thread allocation
- **Fault Tolerance**: Error recovery mechanisms
- **Scalability Testing**: Large-scale simulation support

## Conclusion

This repository provides a comprehensive demonstration of inter-process communication concepts through practical, real-world simulations. The implementations showcase proper use of POSIX threading, synchronization mechanisms, and resource management while maintaining code clarity and educational value. The modular design allows for easy modification and extension, making it suitable for both learning and research purposes.