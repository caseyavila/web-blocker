#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define HOSTS "/etc/hosts"
#define TEMP (HOSTS ".tmp")
#define BLOCK_IP "127.0.0.1"

struct entry {
    char *domain;
    size_t line_num;
    bool blocked;
};

char *read_line(FILE *file) {
    char *string = NULL;
    size_t size = 0;

    int character;
    while ((character = fgetc(file)) != EOF) {
        size++;
        string = realloc(string, size + 1);
        if (string == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        string[size - 1] = character;

        if (character == '\n') {
            break;
        }
    }

    if (size > 0) {
        string[size] = '\0';
    }

    return string;
}

bool is_comment(char *line) {
    return line[0] == '#';
}

/* allocates domain string */
char *get_domain(char *line) {
    if (is_comment(line)) {
        line++;
    }

    /* find start of domain */
    char *domain = line + sizeof(BLOCK_IP) - 1;
    while (isspace(*domain)) {
        domain++;
    }
    size_t domain_size = strlen(domain);
    /* find end of domain */
    while (isspace(domain[domain_size - 1])) {
        domain_size--;
    }

    char *buffer = malloc(domain_size + 1);
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strncpy(buffer, domain, domain_size);
    buffer[domain_size] = '\0';

    return buffer;
}

bool is_entry(char *line) {
    if (is_comment(line)) {
        line++;
    }

    /* ensure BLOCK_IP is the first thing and there's a space after */
    return strstr(line, BLOCK_IP) == line && isspace(line[sizeof(BLOCK_IP) - 1]);
}

struct entry *parse_entries(FILE *hosts_file, size_t *num_entries_out) {
    struct entry *entries = NULL;
    size_t num_entries = 0;
    size_t line_num = 0;

    char *line;
    while ((line = read_line(hosts_file)) != NULL) {
        if (is_entry(line)) {
            num_entries++;
            entries = realloc(entries, sizeof(struct entry) * num_entries);
            if (entries == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }

            entries[num_entries - 1].domain = get_domain(line);
            entries[num_entries - 1].line_num = line_num;
            entries[num_entries - 1].blocked = !is_comment(line);
        }

        line_num++;
        free(line);
    }

    *num_entries_out = num_entries;
    return entries;
}

void free_entries(struct entry *entries, size_t num_entries) {
    for (size_t i = 0; i < num_entries; i++) {
        free(entries[i].domain);
    }

    free(entries);
}

void show_entries(struct entry *entries, size_t num_entries) {
    puts("Found the following entries in your hosts file:");
    puts("");

    for (size_t i = 0; i < num_entries; i++) {
        char icon = entries[i].blocked ? 'x' : ' ';
        printf("  %ld.\t%c  %s\n", i + 1, icon, entries[i].domain);
    }

    puts("");
}

char *input(char *prompt) {
    fputs(prompt, stdout);

    char *answer = read_line(stdin);
    if (answer != NULL) {
        *strchr(answer, '\n') = '\0';
        return answer;
    } else {  /* got EOF or something */
        puts("");
        exit(0);
    }
}

void guessing() {
    char *guess = NULL;
    int password = rand() % 100;

    do {
        free(guess);
        guess = input("Password: ");
    } while (atoi(guess) != password);

    free(guess);
}

void save(FILE *hosts_file, struct entry *entries, size_t num_entries) {
    struct entry *curr_entry = entries;

    rewind(hosts_file);

    FILE *tmp_file = fopen(TEMP, "w");
    if (tmp_file == NULL) {
        perror("Could not open temporary file");
        exit(EXIT_FAILURE);
    }

    size_t line_num = 0;
    char *line;
    while ((line = read_line(hosts_file)) != NULL) {
        if (line_num == curr_entry->line_num) {
            if (!curr_entry->blocked) {
                fputs("#", tmp_file);
            }
            fputs(BLOCK_IP, tmp_file);
            fputs(" ", tmp_file);
            fputs(curr_entry->domain, tmp_file);
            fputs("\n", tmp_file);

            curr_entry++;
        } else {
            fputs(line, tmp_file);
        }

        free(line);
        line_num++;
    }

    if (fclose(tmp_file)) {
        perror("Could not close temporary file");
        exit(EXIT_FAILURE);
    }

    if (fclose(hosts_file)) {
        perror("Could not close hosts file");
        exit(EXIT_FAILURE);
    }

    if (rename(TEMP, HOSTS)) {
        perror("Could not rename temporary file");
        exit(EXIT_FAILURE);
    }
}

int main() {
    srand(time(NULL));

    FILE *hosts_file = fopen(HOSTS, "r+");
    if (hosts_file == NULL) {
        perror("Could not open hosts file");
        exit(EXIT_FAILURE);
    }

    size_t num_entries;
    struct entry *entries = parse_entries(hosts_file, &num_entries);
    if (num_entries == 0) {
        puts("Didn't find any entries in your hosts file...");
        exit(0);
    }

    show_entries(entries, num_entries);

    char *selection = input("Select domain to toggle (number): ");
    long selection_num = strtol(selection, NULL, 10) - 1;
    free(selection);
    if (selection_num < 0 || selection_num >= num_entries) {
        fputs("Not a valid entry index...\n", stdout);
        exit(EXIT_FAILURE);
    }
    struct entry *curr_entry = entries + selection_num;

    if (curr_entry->blocked) {
        guessing();
    }

    curr_entry->blocked = !curr_entry->blocked;

    save(hosts_file, entries, num_entries);

    fputs(curr_entry->domain, stdout);
    fputs(" is now ", stdout);
    fputs(curr_entry->blocked ? "blocked.\n" : "unblocked.\n", stdout);

    free_entries(entries, num_entries);

    return 0;
}
