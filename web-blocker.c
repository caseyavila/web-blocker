#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void guessing(int password);
void block();
void unblock();

int main() {
    char *decision = NULL;
    size_t size = 0;

    srand(time(NULL));

    printf("Do you want to block or unblock a website? [B/U] ");
    getline(&decision, &size, stdin);

    switch (*decision) {
        case 'U':
        case 'u':
            guessing(rand() % 100);
            unblock();
            break;

        case 'B':
        case 'b':
            block();
            break;

        default:
            main();
            break;
    }

    free(decision);
    return 0;
}

void guessing(int password) {
    char *guess = NULL;
    size_t size = 0;
    
    printf("Password: ");
    getline(&guess, &size, stdin);

    if (atoi(guess) == password) {
        printf("CORRECT\n");
    } else {
        guessing(password);
    }

    free(guess);
}

void block() {
}

void unblock() {
}

