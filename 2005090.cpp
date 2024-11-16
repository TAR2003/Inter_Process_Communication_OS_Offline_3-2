#include <iostream>
#include <chrono>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <bits/stdc++.h>
using namespace std;

#define STEP_DELAY 2

#define SLEEP_MULTIPLIER 100000

#define GALLERY1_CAPACITY 5
#define GLASS_CORRIDOR_CAPACITY 3
#define GLASS_CORRIDOR_SLEEP_TIME 10
/// Semaphore to control the number of visitors in Gallery1
sem_t gallery1_capacity;
/// Semaphore to control the number of visitors in DE Glass Corridor
sem_t glass_corridor;
/// Semaphore to control the number of visitors in Gallery2
sem_t gallery2_capacity;

/// Mutex to control step 0
pthread_mutex_t step0_lock;
/// Mutex to control step 1
pthread_mutex_t step1_lock;
/// Mutex to control step 2
pthread_mutex_t step2_lock;

/// Mutex for synchronized printing
pthread_mutex_t print_lock;
/// Mutex for to determine if there is any premium ticket holder in the waiting list
pthread_mutex_t premium_ticket_holder_in_waiting_list;

/// @brief  to hold the current number of premium ticket holders in the waiting list
int premium_ticket_count = 0;
/// Mutex for locking the variable premium ticket count
pthread_mutex_t premium_ticket_count_lock;
/// to hold the current number of standard ticket holder
int standard_ticket_count = 0;
/// to hold the lock for standard ticket count
pthread_mutex_t standard_ticket_count_lock;
/// Mutex for locking the photo_booth
pthread_mutex_t photo_booth_mutex;

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
    int visitor_id;
    bool premium_ticket;
};

// Function to generate a Poisson-distributed random number
int get_random_number()
{
    // Creates a random device for non-deterministic random number generation
    std::random_device rd;
    // Initializes a random number generator using the random device
    std::mt19937 generator(rd());

    // Lambda value for the Poisson distribution
    double lambda = 10000.234;

    // Defines a Poisson distribution with the given lambda
    std::poisson_distribution<int> poissonDist(lambda);

    // Generates and returns a random number based on the Poisson distribution
    return poissonDist(generator);
}

// Function for each visitor thread
void *visitor_activities(void *arg)
{
    visitor v = *(visitor *)arg;
    int visitor_id = v.visitor_id;
    bool premium_ticket = v.premium_ticket;

    // Random delay before attempting to enter
    int randomNumber = get_random_number();
    usleep(randomNumber * 100);

    // Wait for the door
    safe_print("Visitor " + to_string(visitor_id) + " has arrived at A at timestamp " + to_string(get_time()));
    usleep(w * SLEEP_MULTIPLIER);

    /// Now entering at step 1
    pthread_mutex_lock(&step0_lock);
    safe_print("Visitor " + to_string(visitor_id) + " is at step 1 at timestamp " + to_string(get_time()));
    usleep(STEP_DELAY * SLEEP_MULTIPLIER);

    pthread_mutex_lock(&step1_lock);   /// visitor can't leave first step if it cant reach the second step
    pthread_mutex_unlock(&step0_lock); /// visitor is allowed to move to the second step, so he leaves the first

    /// Now entering at step 2

    safe_print("Visitor " + to_string(visitor_id) + " is at step 2 at timestamp " + to_string(get_time()));
    usleep(STEP_DELAY * SLEEP_MULTIPLIER);

    pthread_mutex_lock(&step2_lock);   /// Visitor cant move to the third if it is not unlocked
    pthread_mutex_unlock(&step1_lock); /// only after gaining access to the third step, visitor will leave

    /// Now entering at step 3

    safe_print("Visitor " + to_string(visitor_id) + " is at step 3 at timestamp " + to_string(get_time()));
    usleep(STEP_DELAY * SLEEP_MULTIPLIER);

    sem_wait(&gallery1_capacity);
    pthread_mutex_unlock(&step2_lock);

    safe_print("Visitor " + to_string(visitor_id) + " is at C(Gallery 1) at timestamp " + to_string(get_time()));
    usleep(x * SLEEP_MULTIPLIER);

    sem_wait(&glass_corridor);
    sem_post(&gallery1_capacity);

    safe_print("Visitor " + to_string(visitor_id) + " is at D(Glass Corridor) at timestamp " + to_string(get_time()));
    usleep(GLASS_CORRIDOR_SLEEP_TIME * SLEEP_MULTIPLIER);

    sem_post(&glass_corridor); /// leaving the glass corridor

    safe_print("Visitor " + to_string(visitor_id) + " is at E(Gallery 2) at timestamp " + to_string(get_time()));
    usleep(y * SLEEP_MULTIPLIER);
    safe_print("Visitor " + to_string(visitor_id) + " is about to enter the photo booth at timestamp " + to_string(get_time()));

    if (premium_ticket)
    {
        pthread_mutex_lock(&premium_ticket_count_lock);
        premium_ticket_count++;
        if (premium_ticket_count == 1)
        {
            pthread_mutex_lock(&premium_ticket_holder_in_waiting_list);
        }
        pthread_mutex_unlock(&premium_ticket_count_lock);

        pthread_mutex_lock(&photo_booth_mutex);
        safe_print("Visitor " + to_string(visitor_id) + " is inside the photo booth at timestamp " + to_string(get_time()));
        usleep(z * SLEEP_MULTIPLIER);

        pthread_mutex_lock(&premium_ticket_count_lock);
        premium_ticket_count--;
        if (premium_ticket_count == 0)
        {
            pthread_mutex_unlock(&premium_ticket_holder_in_waiting_list);
        }
        pthread_mutex_unlock(&premium_ticket_count_lock);
        
        pthread_mutex_unlock(&photo_booth_mutex);
    }
    else
    {
        pthread_mutex_lock(&premium_ticket_holder_in_waiting_list);

        pthread_mutex_lock(&standard_ticket_count_lock);
        standard_ticket_count++;
        if (standard_ticket_count == 1)
        {
            pthread_mutex_lock(&photo_booth_mutex);
        }
        pthread_mutex_unlock(&standard_ticket_count_lock);
        pthread_mutex_unlock(&premium_ticket_holder_in_waiting_list);

        safe_print("Visitor " + to_string(visitor_id) + " is at photo booth at timestamp " + to_string(get_time()));
        usleep(z * SLEEP_MULTIPLIER);

        pthread_mutex_lock(&standard_ticket_count_lock);
        standard_ticket_count--;
        if (standard_ticket_count == 0)
        {
            pthread_mutex_unlock(&photo_booth_mutex);
        }
        pthread_mutex_unlock(&standard_ticket_count_lock);
    }
    safe_print("Visitor " + to_string(visitor_id) + " is exited the path at timestamp " + to_string(get_time()));

    return NULL;
}

int main()
{
    int N, M;
    N = 10;
    M = 6;
    w = 2;
    x = 6;
    y = 6;
    z = 3;

    int totalvisitors = N + M;
    pthread_t visitors[totalvisitors];

    vector<visitor *> allvisitors(totalvisitors);
    for (int i = 0; i < N; i++)
    {
        allvisitors[i] = new visitor();
        allvisitors[i]->premium_ticket = false;
        allvisitors[i]->visitor_id = 1001 + i;
    }

    for (int i = 0; i < M; i++)
    {
        allvisitors[N + i] = new visitor();
        allvisitors[N + i]->premium_ticket = true;
        allvisitors[N + i]->visitor_id = 2001 + i;
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
    pthread_mutex_init(&premium_ticket_count_lock, NULL);
    pthread_mutex_init(&premium_ticket_holder_in_waiting_list, NULL);
    pthread_mutex_init(&standard_ticket_count_lock, NULL);
    pthread_mutex_init(&photo_booth_mutex, NULL);

    // Create standard_ticket_holder thread
    for (int i = 0; i < totalvisitors; i++)
    {
        int *visitor_id = new int(i + 1); // Dynamically allocate memory for visitor ID
        pthread_create(&visitors[i], NULL, visitor_activities, allvisitors[i]);
    }

    // Wait for all visitor threads to finish
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
    pthread_mutex_destroy(&premium_ticket_count_lock);
    pthread_mutex_destroy(&premium_ticket_holder_in_waiting_list);
    pthread_mutex_destroy(&standard_ticket_count_lock);
    pthread_mutex_destroy(&photo_booth_mutex);

    for (int i = 0; i < totalvisitors; i++)
    {
        delete allvisitors[i];
    }
    return 0;
}
