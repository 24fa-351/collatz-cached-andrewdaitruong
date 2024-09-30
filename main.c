#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CACHE_SIZE 1000

typedef struct {
    int number;
    int steps;
    int frequency;
} CacheEntry;

CacheEntry cache[CACHE_SIZE];


int cache_hits = 0;          
int total_cache_accesses = 0;

void initialize_cache() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache[i].number = -1;
        cache[i].steps = -1;
        cache[i].frequency = 0;
    }
}

// Simple hash function for the cache
int hash_function(int n) {
    return n % CACHE_SIZE;
}

int get_Least_Used() {
    int lfu_index = 0;
    int min_frequency = cache[0].frequency;

     for (int i = 1; i < CACHE_SIZE; i++) {
        if (cache[i].frequency < min_frequency) {
            min_frequency = cache[i].frequency;
            lfu_index = i;
        }
    }
    return lfu_index;
}

// Function to perform Collatz conjecture and count steps
int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
    }
    return steps;
}

int cached_collatz_steps(int n) {
    total_cache_accesses++;
    int hash = hash_function(n);
    
    // Checks the cache for similar results
    if (cache[hash].number == n) {
        cache_hits++;
        cache[hash].frequency++;
        return cache[hash].steps;
    }
    
    // If the cache slot is occupied by a different number
    if (cache[hash].number != -1) {
        // Evict the least frequently used entry in the cache
        int lfu_index = get_Least_Used();
        hash = lfu_index;
    }

    // Call the core function and store the result
    int steps = collatz_steps(n);
    cache[hash].number = n;
    cache[hash].steps = steps;
    cache[hash].frequency = 1;
    
    return steps;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <N> <MIN> <MAX>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int MIN = atoi(argv[2]);
    int MAX = atoi(argv[3]);

    if (MIN > MAX) {
        fprintf(stderr, "Error: MIN should be less than or equal to MAX\n");
        return 1;
    }

    srand(time(NULL));
    
    for (int i = 0; i < N; i++) {
        int RN = MIN + rand() % (MAX - MIN + 1);
        int steps = cached_collatz_steps(RN);
        double cache_hit_percentage = (cache_hits / (double)total_cache_accesses) * 100.0;
        printf("%d,%d,%2f%%\n", RN, steps, cache_hit_percentage);
    }

    return 0;
}