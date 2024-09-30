#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Function prototypes
int collatz(int n);
int randomInRange(int min, int max);
void runWithNoCache(int N, int MIN, int MAX);
void runWithCache(int N, int MIN, int MAX, int cacheSize, const char *policy);

// LRU Cache structure (you can create similar structures for other cache policies)
typedef struct CacheEntry {
    int number;
    int steps;
    struct CacheEntry *prev, *next;
} CacheEntry;

typedef struct {
    CacheEntry *head, *tail;
    int capacity;
    int count;
    int hits, misses;
} LRUCache;

LRUCache* createLRUCache(int capacity);
void freeLRUCache(LRUCache *cache);
int getFromCache(LRUCache *cache, int number);
void putInCache(LRUCache *cache, int number, int steps);
void printCSV(int number, int steps);

// Collatz conjecture function: returns the number of steps
int collatz(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0)
            n /= 2;
        else
            n = 3 * n + 1;
        steps++;
    }
    return steps;
}

// Random number generator between MIN and MAX
int randomInRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Function to run without cache
void runWithNoCache(int N, int MIN, int MAX) {
    for (int i = 0; i < N; i++) {
        int randomNum = randomInRange(MIN, MAX);
        int steps = collatz(randomNum);
        printCSV(randomNum, steps);
    }
}

// LRU Cache functions
LRUCache* createLRUCache(int capacity) {
    LRUCache *cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->head = cache->tail = NULL;
    cache->capacity = capacity;
    cache->count = 0;
    cache->hits = 0;
    cache->misses = 0;
    return cache;
}

void freeLRUCache(LRUCache *cache) {
    CacheEntry *entry = cache->head;
    while (entry) {
        CacheEntry *temp = entry;
        entry = entry->next;
        free(temp);
    }
    free(cache);
}

int getFromCache(LRUCache *cache, int number) {
    CacheEntry *current = cache->head;
    while (current) {
        if (current->number == number) {
            // Move the found entry to the front (LRU behavior)
            if (current != cache->head) {
                if (current == cache->tail) {
                    cache->tail = current->prev;
                    cache->tail->next = NULL;
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                current->next = cache->head;
                current->prev = NULL;
                cache->head->prev = current;
                cache->head = current;
            }
            cache->hits++;
            return current->steps;
        }
        current = current->next;
    }
    cache->misses++;
    return -1;
}

void putInCache(LRUCache *cache, int number, int steps) {
    CacheEntry *newEntry = (CacheEntry*)malloc(sizeof(CacheEntry));
    newEntry->number = number;
    newEntry->steps = steps;
    newEntry->next = cache->head;
    newEntry->prev = NULL;

    if (cache->head != NULL) {
        cache->head->prev = newEntry;
    }
    cache->head = newEntry;

    if (cache->tail == NULL) {
        cache->tail = newEntry;
    }

    if (cache->count < cache->capacity) {
        cache->count++;
    } else {
        CacheEntry *temp = cache->tail;
        cache->tail = cache->tail->prev;
        cache->tail->next = NULL;
        free(temp);
    }
}

// Function to print results in CSV format
void printCSV(int number, int steps) {
    printf("%d,%d\n", number, steps);
}

// Function to run with caching
void runWithCache(int N, int MIN, int MAX, int cacheSize, const char *policy) {
    LRUCache *cache = createLRUCache(cacheSize);

    for (int i = 0; i < N; i++) {
        int randomNum = randomInRange(MIN, MAX);
        int steps = getFromCache(cache, randomNum);

        if (steps == -1) {
            steps = collatz(randomNum);
            putInCache(cache, randomNum, steps);
        }

        printCSV(randomNum, steps);
    }

    printf("Cache Hits: %d, Cache Misses: %d, Hit Rate: %.2f%%\n",
           cache->hits, cache->misses, (cache->hits * 100.0) / N);

    freeLRUCache(cache);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s N MIN MAX\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int MIN = atoi(argv[2]);
    int MAX = atoi(argv[3]);

    srand(time(NULL));

    // Run the program without caching
    printf("Running without cache...\n");
    runWithNoCache(N, MIN, MAX);

    // Run the program with caching (modify the cache policy as needed)
    int cacheSize = 100;  // Example cache size, modify as needed
    printf("Running with LRU cache...\n");
    runWithCache(N, MIN, MAX, cacheSize, "LRU");

    return 0;
}
