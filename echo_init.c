// echo_init.c
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Echo Init Started\n");

    char buffer[128];
    while (1) {
        printf("> ");
        fflush(stdout);
        ssize_t n = read(0, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Echo: %s", buffer);
        }
    }
}

