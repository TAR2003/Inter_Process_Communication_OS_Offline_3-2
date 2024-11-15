#include <iostream>
#include <chrono>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

// Number of students
#define NUM_STUDENTS 10

// Semaphore to control the number of students in Hall A
sem_t hall_a_capacity;

// Mutex to control the door
pthread_mutex_t door_lock;

// Mutex for synchronized printing
pthread_mutex_t print_lock;

// Start time for the simulation
auto start_time = std::chrono::high_resolution_clock::now();

/**
 * Get the elapsed time in milliseconds since the start of the simulation.
 * @return The elapsed time in milliseconds.
 */
long long get_time()
{
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    return duration.count();
}

// Thread-safe print function
void safe_print(const std::string &message)
{
    pthread_mutex_lock(&print_lock);
    std::cout << message << std::endl;
    pthread_mutex_unlock(&print_lock);
}

// Function for each student thread
void *student_activities(void *arg)
{
    int student_id = *(int *)arg;
    free(arg); // Free the dynamically allocated ID

    // Random delay before attempting to enter
    usleep(rand() % 1000000);

    // Wait for the door
    safe_print("[" + std::to_string(get_time()) + "ms] Student " + std::to_string(student_id) + " is waiting at the door.");
    pthread_mutex_lock(&door_lock);
    safe_print("[" + std::to_string(get_time()) + "ms] Student " + std::to_string(student_id) + " is passing through the door.");

    // Simulate time to pass through the door
    usleep(500000);
    pthread_mutex_unlock(&door_lock);
    safe_print("[" + std::to_string(get_time()) + "ms] Student " + std::to_string(student_id) + " has passed through the door.");

    // Wait to enter Hall A
    sem_wait(&hall_a_capacity);
    safe_print("[" + std::to_string(get_time()) + "ms] Student " + std::to_string(student_id) + " has entered Hall A.");

    // Simulate time spent in Hall A
    sleep(2);

    // Leave Hall A
    safe_print("[" + std::to_string(get_time()) + "ms] Student " + std::to_string(student_id) + " is leaving Hall A.");
    sem_post(&hall_a_capacity);

    return NULL;
}

int main()
{
    pthread_t students[NUM_STUDENTS];

    // Initialize semaphore with a max of 3 in Hall A
    sem_init(&hall_a_capacity, 0, 3);

    // Initialize the door mutex and the print mutex
    pthread_mutex_init(&door_lock, NULL);
    pthread_mutex_init(&print_lock, NULL);

    // Create student threads
    for (int i = 0; i < NUM_STUDENTS; i++)
    {
        int *student_id = new int(i + 1); // Dynamically allocate memory for student ID
        pthread_create(&students[i], NULL, student_activities, student_id);
    }

    // Wait for all student threads to finish
    for (int i = 0; i < NUM_STUDENTS; i++)
    {
        pthread_join(students[i], NULL);
    }

    // Destroy semaphore and mutexes
    sem_destroy(&hall_a_capacity);
    pthread_mutex_destroy(&door_lock);
    pthread_mutex_destroy(&print_lock);

    return 0;
}
