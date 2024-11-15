#include <iostream>
#include <chrono>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <bits/stdc++.h>
using namespace std;

#define STEP_DELAY 2

#define SLEEP_MULTIPLIER 100

#define GALLERY1_CAPACITY 5
#define GLASS_CORRIDOR_CAPACITY 3
// Semaphore to control the number of visitors in Gallery1
sem_t gallery1_capacity;
// Semaphore to control the number of visitors in DE Glass Corridor
sem_t glass_corridor;
// Semaphore to control the number of visitors in Gallery2
sem_t gallery2_capacity;

// Mutex to control step 0
pthread_mutex_t step0_lock;
// Mutex to control step 1
pthread_mutex_t step1_lock;
// Mutex to control step 2
pthread_mutex_t step2_lock;

// Mutex for synchronized printing
pthread_mutex_t print_lock;

int w, x, y, z;

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

/// @brief Print function which is thread safe
/// @param message A string which contains the information you want to print
void safe_print(const string &message)
{
    pthread_mutex_lock(&print_lock);
    cout << message << endl;
    pthread_mutex_unlock(&print_lock);
}

/// a visitor class
class visitor
{
public:
    int student_id;
    bool premium_ticket;
};

// Function for each visitor thread
void *visitor_activities(void *arg)
{
    visitor v = *(visitor *)arg;
    int student_id = v.student_id;
    bool premium_ticket = v.premium_ticket;

    // Random delay before attempting to enter
    usleep(rand() % 100000);

    // Wait for the door
    safe_print("Student " + to_string(student_id) + " has arrived at A at " + to_string(get_time()));
    sleep(w);

    /// Now entering at step 1
    pthread_mutex_lock(&step0_lock);
    safe_print("Student " + to_string(student_id) + " is at step 1 at " + to_string(get_time()));
    sleep(STEP_DELAY);
    pthread_mutex_unlock(&step0_lock);

    /// Now entering at step 2
    pthread_mutex_lock(&step1_lock);
    safe_print("Student " + to_string(student_id) + " is at step 2 at " + to_string(get_time()));
    sleep(STEP_DELAY);
    pthread_mutex_unlock(&step1_lock);

    /// Now entering at step 3
    pthread_mutex_lock(&step2_lock);
    safe_print("Student " + to_string(student_id) + " is at step 3 at " + to_string(get_time()));
    sleep(STEP_DELAY);
    pthread_mutex_unlock(&step2_lock);

    sem_wait(&gallery1_capacity);
    safe_print("Student " + to_string(student_id) + " is at C(Gallery 1) at " + to_string(get_time()));
    sleep(x);
    sem_post(&gallery1_capacity);

    sem_wait(&glass_corridor);
    safe_print("Student " + to_string(student_id) + " is at Glass Corrior at " + to_string(get_time()));
    sleep(10);
    sem_post(&glass_corridor);

    safe_print("Student " + to_string(student_id) + " is exiting the glass corridor at " + to_string(get_time()));

    return NULL;
}

int main()
{
    int N, M;
    N = 10;
    M = 5;
    w = 3;
    x = 3;
    y = 10;

    int totalvisitors = N + M;
    pthread_t visitors[totalvisitors];

    vector<visitor *> allvisitors(totalvisitors);
    for (int i = 0; i < N; i++)
    {
        allvisitors[i] = new visitor();
        allvisitors[i]->premium_ticket = false;
        allvisitors[i]->student_id = 1001 + i;
    }

    for (int i = 0; i < M; i++)
    {
        allvisitors[N + i] = new visitor();
        allvisitors[N + i]->premium_ticket = false;
        allvisitors[N + i]->student_id = 2001 + i;
    }

    // Initialize semaphore with a max of w in gallery 1
    sem_init(&gallery1_capacity, 0, GALLERY1_CAPACITY);
    // Initialize the semaphore with a max of x in glass corridor DE
    sem_init(&glass_corridor, 0, GLASS_CORRIDOR_CAPACITY);

    // Initialize the print mutex, step 0, 1, 2 mutex
    pthread_mutex_init(&print_lock, NULL);
    pthread_mutex_init(&step0_lock, NULL);
    pthread_mutex_init(&step1_lock, NULL);
    pthread_mutex_init(&step2_lock, NULL);

    // Create standard_ticket_holder thread
    for (int i = 0; i < totalvisitors; i++)
    {
        int *student_id = new int(i + 1); // Dynamically allocate memory for student ID
        pthread_create(&visitors[i], NULL, visitor_activities, allvisitors[i]);
    }

    // Wait for all student threads to finish
    for (int i = 0; i < totalvisitors; i++)
    {
        pthread_join(visitors[i], NULL);
    }

    // Destroy semaphore and mutexes
    sem_destroy(&gallery1_capacity);
    sem_destroy(&glass_corridor);
    pthread_mutex_destroy(&print_lock);
    pthread_mutex_destroy(&step0_lock);
    pthread_mutex_destroy(&step1_lock);
    pthread_mutex_destroy(&step2_lock);

    for (int i = 0; i < totalvisitors; i++)
    {
        delete allvisitors[i];
    }
    return 0;
}
