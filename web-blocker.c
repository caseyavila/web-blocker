#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HOSTS "/etc/hosts"
#define HOSTS_TMP "/etc/hosts.tmp"

void user_input(char buffer[], size_t buffer_size, char *prompt);
void guessing(int password);
void block();
void unblock();

int main() {
    char decision[255];
    char domain[255];

    srand(time(NULL));

    user_input(decision, sizeof(decision), "Do you want to block or unblock a website? [b/u] ");

    switch (*decision) {
        case 'U':
        case 'u':
            guessing(rand() % 100);
            user_input(domain, sizeof(domain), "Type the domain you want to unblock: ");
            unblock(domain);
            break;

        case 'B':
        case 'b':
            user_input(domain, sizeof(domain), "Type the domain you want to block: ");
            block(domain);
            break;

        default:
            main();
            break;
    }

    return 0;
}

void user_input(char buffer[], size_t buffer_size, char *prompt) {
    printf("%s", prompt);
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        /* Remove trailing newline */
        buffer[strcspn(buffer, "\n")] = 0;
    } else {
        fprintf(stderr, "Invalid input, exiting...\n");
        exit(1);
    }
}

void guessing(int password) {
    char guess[255];
    
    user_input(guess, sizeof(guess), "Password: ");

    if (atoi(guess) != password) {
        guessing(password);
    }
}

void block(char *domain) {
    FILE *file;
    char line[255];

    file = fopen(HOSTS, "r");
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, domain) != NULL && strstr(line, "127.0.0.1") != NULL && line[0] != '#') {
            printf("%s is already blocked!\n", domain);
            return;
        }
    }

    fclose(file);

    file = fopen(HOSTS, "a");
    if (file == NULL) {
        perror("Could not edit hosts file");
    } else {
        fprintf(file, "127.0.0.1 %s\n", domain);
        fclose(file);
    }
}

void unblock(char *domain) {
    FILE *file;
    FILE *file_new;
    char line[255];

    file_new = fopen(HOSTS_TMP, "w");
    if (file_new == NULL) {
        perror("Could not edit hosts file");
    } else {
        file = fopen(HOSTS, "r");
        while (fgets(line, sizeof(line), file) != NULL) {
            if ((strstr(line, domain) == NULL && strstr(line, "127.0.0.1") != NULL) || line[0] == '#') {
                fprintf(file_new, "%s", line);
            }
        }

        fclose(file_new);
        fclose(file);

        rename(HOSTS_TMP, HOSTS);
    }
}

