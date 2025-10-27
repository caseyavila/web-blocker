#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define HOSTS "./hosts"
#define BLOCK_IP "127.0.0.1"

struct entry_t {
    char *domain;
    size_t line_no;
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

    char *domain = line + sizeof(BLOCK_IP) - 1;
    while (isspace(*domain)) {
        domain++;
    }
    size_t domain_size = strlen(domain);

    char *buffer = malloc(domain_size + 1);
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(buffer, domain);

    /* remove trailing newline */
    *strchr(buffer, '\n') = 0;

    return buffer;
}

bool is_entry(char *line) {
    if (is_comment(line)) {
        line++;
    }

    char *match = strstr(line, BLOCK_IP);
    /* ensure BLOCK_IP is the first thing and there's a space after */
    return match == line && isspace(line[sizeof(BLOCK_IP) - 1]);
}

struct entry_t *parse_entries(FILE *hosts_file, size_t *num_entries_out) {
    struct entry_t *entries = NULL;
    size_t num_entries = 0;
    size_t line_no = 0;

    char *line;
    while ((line = read_line(hosts_file)) != NULL) {
        if (is_entry(line)) {
            num_entries++;
            entries = realloc(entries, sizeof(struct entry_t) * num_entries);
            if (entries == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }

            entries[num_entries - 1].domain = get_domain(line);
            entries[num_entries - 1].line_no = line_no;
            entries[num_entries - 1].blocked = !is_comment(line);
        }

        line_no++;

        free(line);
    }

    *num_entries_out = num_entries;
    return entries;
}

void free_entries(struct entry_t *entries, size_t num_entries) {
    for (size_t i = 0; i < num_entries; i++) {
        free(entries[i].domain);
    }

    free(entries);
}

void show_entries(struct entry_t *entries, size_t num_entries) {
    puts("Found the following entries in your hosts file:");
    puts("");

    for (size_t i = 0; i < num_entries; i++) {
        char icon = entries[i].blocked ? 'x' : ' ';
        printf("    %c  %s\n", icon, entries[i].domain);
    }

    puts("");
}

int main() {
    FILE *hosts_file = fopen(HOSTS, "r+");
    if (hosts_file == NULL) {
        perror("Could not open hosts file");
        exit(EXIT_FAILURE);
    }

    FILE *tmp_file = tmpfile();
    if (tmp_file == NULL) {
        perror("Could not open temporary file");
        exit(EXIT_FAILURE);
    }

    size_t num_entries;
    struct entry_t *entries = parse_entries(hosts_file, &num_entries);
    if (num_entries == 0) {
        puts("Didn't find any entries in your hosts file...");
        exit(0);
    }

    /* TODO */

    show_entries(entries, num_entries);

    free_entries(entries, num_entries);
    fclose(hosts_file);
    fclose(tmp_file);

    return 0;
}
