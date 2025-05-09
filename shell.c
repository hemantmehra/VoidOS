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
    char cwd[256];

    printf("Minimal Shell Init Started\n");

    // Set a default PATH environment variable
    setenv("PATH", "/bin:/sbin:/usr/bin:/usr/sbin", 1);

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s> ", cwd);
        } else {
            printf("> ");
        }
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // EOF or error
        }

        if (input[0] == '\n') {
            continue; // empty line
        }

        parse_input(input, args);

        if (args[0] == NULL) {
            continue;
        }

        // Built-in "exit" command
        if (strcmp(args[0], "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        // Built-in "cd" command
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "cd: missing argument\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("cd");
                }
            }
            continue; // don't fork
        }

        // Built-in "pwd"
        if (strcmp(args[0], "pwd") == 0) {
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("getcwd");
            }
            continue; // No need to fork
        }

        if (strcmp(args[0], "clear") == 0) {
            printf("\033[2J\033[H");
            fflush(stdout);
            continue; // No need to fork
        }

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
