#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void list_directory(const char *path) {
    DIR *dir = opendir(path);  // Open the directory
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {  // Read each entry
        printf("%s\n", entry->d_name);  // Print filename
    }

    closedir(dir);  // Close the directory
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        list_directory(".");  // Default to current directory
    } else {
        for (int i = 1; i < argc; i++) {
            list_directory(argv[i]);  // List files in provided directory
        }
    }

    return 0;
}
