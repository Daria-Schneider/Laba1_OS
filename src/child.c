#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringutils.h"

#define BUFFER_SIZE 1024

int main(void) {
    char buffer[BUFFER_SIZE];
    FILE* file = NULL;

    if (!fgets(buffer, sizeof(buffer), stdin)) {
        const char* err = "Error: failed to read file name\n";
        printf("%s", err); fflush(stdout);
        return EXIT_FAILURE;
    }
    TrimNewline(buffer);

    file = fopen(buffer, "w");
    if (file == NULL) {
        char err[BUFFER_SIZE];
        snprintf(err, sizeof(err), "Error: cannot open file '%s' for writing\n", buffer);
        printf("%s", err); fflush(stdout);
        return EXIT_FAILURE;
    }

    printf("File opened successfully\n");
    fflush(stdout);

    while (fgets(buffer, sizeof(buffer), stdin)) {
        TrimNewline(buffer);
        if (strlen(buffer) == 0) {
            break;
        }

        if (IsCapitalStart(buffer)) {
            fprintf(file, "%s\n", buffer);
            fflush(file);
            char ok[BUFFER_SIZE];
            snprintf(ok, sizeof(ok), "String written to file: '%s'\n", buffer);
            printf("%s", ok);
            fflush(stdout);
        } else {
            char err[BUFFER_SIZE];
            snprintf(err, sizeof(err), "Error: string must start with capital letter - '%s'\n", buffer);
            printf("%s", err);
            fflush(stdout);
        }
    }

    if (file) fclose(file);
    printf("Child process finished\n");
    fflush(stdout);
    return EXIT_SUCCESS;
}
