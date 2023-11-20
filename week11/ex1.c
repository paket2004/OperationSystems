#include <stdio.h>
#include <dirent.h>

int main() {
    // Open the file for writing (this will replace stdout)
    FILE *file = freopen("ex1.txt", "w", stdout);
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Open the root directory
    DIR *dir = opendir("/");

    // Check if the directory was opened successfully
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    // Read the directory entries
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);

    // Close the file (optional, as fclose will also be called on program termination)
    fclose(file);

    return 0;
}

