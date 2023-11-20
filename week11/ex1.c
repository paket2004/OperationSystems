#include <stdio.h>
#include <dirent.h>

int main() {
    // Open the root directory
    DIR *dir = opendir("/");

    // Check if the directory was opened successfully
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    // Read the directory entries and print to the console
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);

    return 0;
}
