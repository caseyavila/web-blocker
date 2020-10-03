#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HOSTS "/etc/hosts"
#define HOSTS_TMP "/etc/hosts.tmp"

void guessing(int password);
void block();
void unblock();

int main() {
    char decision[255];
    char url[255];

    srand(time(NULL));

    printf("Do you want to block or unblock a website? [b/u] ");
    if (fgets(decision, sizeof(decision), stdin)) {
        decision[strcspn(decision, "\n")] = 0;
    } else {
        return 0;
    }

    switch (*decision) {
        case 'U':
        case 'u':
            guessing(rand() % 100);
            printf("Type the url you want to unblock: ");
            if (fgets(url, sizeof(url), stdin)) {
                url[strcspn(url, "\n")] = 0;
                unblock(url);
            }
            break;

        case 'B':
        case 'b':
            printf("Type the url you want to block: ");
            if (fgets(url, sizeof(url), stdin)) {
                url[strcspn(url, "\n")] = 0;
                block(url);
            }
            break;

        default:
            main();
            break;
    }

    return 0;
}

void guessing(int password) {
    char guess[255];
    
    printf("Password: ");
    if (fgets(guess, sizeof(guess), stdin)) {
        guess[strcspn(guess, "\n")] = 0;
    } else {
        exit(0);
    }

    if (atoi(guess) != password) {
        guessing(password);
    }
}

void block(char *url) {
    FILE *file;
    char line[255];

    file = fopen(HOSTS, "r");
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, url) != NULL && strstr(line, "127.0.0.1") != NULL && line[0] != '#') {
            printf("%s is already blocked!\n", url);
            return;
        }
    }

    fclose(file);

    file = fopen(HOSTS, "a");
    if (file == NULL) {
        perror("Could not edit hosts file");
    } else {
        fprintf(file, "127.0.0.1 %s\n", url);
        fclose(file);
    }
}

void unblock(char *url) {
    FILE *file;
    FILE *file_new;
    char line[255];

    file_new = fopen(HOSTS_TMP, "w");
    if (file_new == NULL) {
        perror("Could not edit hosts file");
    } else {
        file = fopen(HOSTS, "r");
        while (fgets(line, sizeof(line), file) != NULL) {
            if ((strstr(line, url) == NULL && strstr(line, "127.0.0.1") == NULL) || line[0] == '#') {
                fprintf(file_new, "%s", line);
            }
        }

        fclose(file_new);
        fclose(file);

        rename(HOSTS_TMP, HOSTS);
    }
}

