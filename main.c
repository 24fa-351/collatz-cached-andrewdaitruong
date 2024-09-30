#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CACHE_SIZE 1000

typedef struct {
    int number;
    int steps;
} CacheEntry;

CacheEntry cache[CACHE_SIZE];  // Cache array

void initialize_cache() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache[i].number = -1;
        cache[i].steps = -1;
    }
}

// Simple hash function for the cache
int hash_function(int n) {
    return n % CACHE_SIZE;
}

int cached_collatz_steps(int n) {
    int hash = hash_function(n);
    
    // Checks the cache for similar results
    if (cache[hash].number == n) {
        return cache[hash].steps;
    }
    
    // Call the core function and store the result
    int steps = collatz_steps(n);
    cache[hash].number = n;
    cache[hash].steps = steps;
    
    return steps;
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
    
    initialize_cache();
    
    for (int i = 0; i < N; i++) {
        int RN = MIN + rand() % (MAX - MIN + 1);
        int steps = cached_collatz_steps(RN);

        printf("%d,%d\n", RN, steps);
    }

    return 0;
}