#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "crossplatform.h"
#include "stringutils.h"

#define BUFFER_SIZE 1024

int main(void) {
    process_t child;
    char line[BUFFER_SIZE];
    char childBuf[BUFFER_SIZE];
    int bytes;

    memset(&child, 0, sizeof(child));

    const char *childPath = CpGetChildProcessName("child");

    if (CpProcessCreate(&child, childPath) != 0) {
        fprintf(stderr, "Error: failed to create child process\n");
        return EXIT_FAILURE;
    }

    printf("Enter file name: ");
    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "Error: failed to read file name\n");
        CpProcessClose(&child);
        return EXIT_FAILURE;
    }
    TrimNewline(line);

    if (CpProcessWrite(&child, line, strlen(line)) < 0 ||
        CpProcessWrite(&child, "\n", 1) < 0) {
        fprintf(stderr, "Error: failed to send file name to child process\n");
        CpProcessClose(&child);
        return EXIT_FAILURE;
    }

    bytes = CpProcessRead(&child, childBuf, (int)sizeof(childBuf) - 1);
    if (bytes > 0) {
        if (bytes > (int)sizeof(childBuf) - 1) bytes = (int)sizeof(childBuf) - 1;
        childBuf[bytes] = '\0';
        printf("%s", childBuf);
        if (CpStringContains(childBuf, "Error:")) {
            CpProcessClose(&child);
            return EXIT_FAILURE;
        }
    }

    while (1) {
        printf("Enter string (empty string to exit): ");
        if (!fgets(line, sizeof(line), stdin)) break;
        TrimNewline(line);

        if (CpStringLength(line) == 0) {
            CpProcessWrite(&child, "\n", 1);
            break;
        }

        if (CpProcessWrite(&child, line, strlen(line)) < 0 ||
            CpProcessWrite(&child, "\n", 1) < 0) {
            fprintf(stderr, "Error: failed to send string to child process\n");
            break;
        }

        bytes = CpProcessRead(&child, childBuf, (int)sizeof(childBuf) - 1);
        if (bytes > 0) {
            if (bytes > (int)sizeof(childBuf) - 1) bytes = (int)sizeof(childBuf) - 1;
            childBuf[bytes] = '\0';
            printf("%s", childBuf);
        }
    }

    CpProcessClose(&child);
    printf("Parent process finished.\n");
    return EXIT_SUCCESS;
}
