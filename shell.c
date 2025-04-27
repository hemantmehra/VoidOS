// echo_init.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 256
#define MAX_ARGS 16

void parse_input(char *input, char *args[]) {
    int i = 0;
    input[strcspn(input, "\n")] = 0; // remove newline
    char *token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

int main() {
    char input[MAX_LINE];
    char *args[MAX_ARGS];

    printf("Minimal Shell Init Started\n");

    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // EOF or error
        }

        if (input[0] == '\n') {
            continue; // empty line
        }

        parse_input(input, args);

        pid_t pid = fork();
        if (pid == 0) {
            // child
            execvp(args[0], args);
            perror("execvp"); // if exec fails
            exit(1);
        } else if (pid > 0) {
            // parent
            int status;
            waitpid(pid, &status, 0);
        } else {
            perror("fork");
        }
    }

    return 0;
}
