#include <stdio.h>
#include <pthread.h>

#define SIZE 100000
#define NUM_THREADS 15

long arr[SIZE];
long partialSums[NUM_THREADS] = {0}; // Array to store partial sums for each thread

// Entry function for each thread
void* sumPart(void* arg) {
    
    // Divide the work for each thread based on their id and let them compute partial sums
    int thread_id = *(int*)arg;  // convert the thread id back to integer
    int start = 0;
    int end = 0;
    if (SIZE % NUM_THREADS == 0 ){
        start = thread_id * (SIZE / NUM_THREADS);  // Starting index for this thread
        end = (thread_id + 1) * (SIZE / NUM_THREADS);  // Ending index for this thread equal to start of next thread
    } else {
        if (thread_id == NUM_THREADS-1) {
            start = thread_id * (SIZE / NUM_THREADS);
            end = SIZE;
        } else {
            start = thread_id * (SIZE / NUM_THREADS);  // Starting index for this thread
            end = (thread_id + 1) * (SIZE / NUM_THREADS);  // Ending index for this thread equal to start of next thread
        }
    }   

    long rollsum = 0;
    // Calc the partial sum of this thread from start to end
    for (int i = start; i < end; i++) {
        rollsum += arr[i];
    }
    partialSums[thread_id] = rollsum;
    printf("thread %d, sum is %ld\n",thread_id,rollsum);

    return NULL; //no need to return, return is in partial sum array
}

int main() {
    // Initialize the array
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create threads to compute partial sums
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sumPart, (void*)&thread_ids[i]);
    }

    // Wait for all threads to finish
     for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); //no return type
    }

    // Combine the partial sums from all threads
    long totalSum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        totalSum += partialSums[i];
    }

    printf("Total Sum: %ld\n", totalSum);

    return 0;
}
