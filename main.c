#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    // Parse command-line arguments
    int N = atoi(argv[1]);
    int MIN = atoi(argv[2]);
    int MAX = atoi(argv[3]);

    if (MIN > MAX) {
        fprintf(stderr, "Error: MIN should be less than or equal to MAX\n");
        return 1;
    }

    srand(time(NULL));

    // Loop through N random numbers and apply the Collatz conjecture
    for (int i = 0; i < N; i++) {
        int RN = MIN + rand() % (MAX - MIN + 1);  // Random number between MIN and MAX
        int steps = collatz_steps(RN);

        printf("%d,%d\n", RN, steps);
    }

    return 0;
}