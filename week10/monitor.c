#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/inotify.h>
#include <time.h>
#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

const char* directoryPath;
int inotifyFd;

void handle_event(struct inotify_event *event) {
    char target[PATH_MAX];

    if (event->len > 0) {
        snprintf(target, sizeof(target), "%s/%s", directoryPath, event->name);
    } else {
        // Use the directory path itself when the event does not provide a name
        snprintf(target, sizeof(target), "%s/%s", directoryPath, event->name);
    }
    if (event->mask & IN_ACCESS) {
        printf("File accessed: %s\n", target);
    }

    if (event->mask & IN_CREATE) {
        printf("File/directory created: %s\n", target);
    }

    if (event->mask & IN_DELETE) {
        printf("File/directory deleted: %s\n", target);
    }

    if (event->mask & IN_MODIFY) {
        printf("File modified: %s\n", target);
    }

    if (event->mask & IN_OPEN) {
        printf("File or directory opened: %s\n", target);
    }

    if (event->mask & IN_ATTRIB) {
        printf("Metadata changed: %s\n", target);
    }
}

void sigint_handler(int signo) {
    printf("Received SIGINT. Printing stat info of all entries in the directory...\n");

    DIR *dir = opendir(directoryPath);
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char entryPath[PATH_MAX];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", directoryPath, entry->d_name);

        struct stat statInfo;
        if (stat(entryPath, &statInfo) == 0) {
            printf("Entry: %s\n", entryPath);
            printf("  Size: %ld bytes\n", statInfo.st_size);
            printf("  Inode: %lu\n", (unsigned long)statInfo.st_ino);
            printf("  Permissions: %o\n", statInfo.st_mode & 0777);
            printf("  Last access time: %s", ctime(&statInfo.st_atime));
            printf("  Last modification time: %s", ctime(&statInfo.st_mtime));
            printf("\n");
        } else {
            perror("stat");
        }
    }

    closedir(dir);

    // Clean up inotify and exit
    close(inotifyFd);
    exit(EXIT_SUCCESS);
}

int main (int argc, char* argv[]) {
	if (argc != 2) {
		printf("Error in input data\n");
	}
	inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }
	directoryPath = argv[1];
	int watchDirectory = inotify_add_watch(inotifyFd, directoryPath, IN_ACCESS | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN | IN_ATTRIB);
	
	if (watchDirectory == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }
	printf("Watching directory: %s\n", directoryPath);
	
	DIR *dir = opendir(directoryPath);
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char entryPath[PATH_MAX];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", directoryPath, entry->d_name);

        struct stat statInfo;
        if (stat(entryPath, &statInfo) == 0) {
            printf("Entry: %s\n", entryPath);
            printf("  Size: %ld bytes\n", statInfo.st_size);
            printf("  Inode: %lu\n", (unsigned long)statInfo.st_ino);
            printf("  Permissions: %o\n", statInfo.st_mode & 0777);
            printf("  Last access time: %s", ctime(&statInfo.st_atime));
            printf("  Last modification time: %s", ctime(&statInfo.st_mtime));
            printf("\n");
        } else {
            perror("stat");
        }
    }

    closedir(dir);
	
	
	signal(SIGINT, sigint_handler);
	while (1) {
        char buffer[BUF_LEN];
        ssize_t bytesRead = read(inotifyFd, buffer, BUF_LEN);

        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        for (char *ptr = buffer; ptr < buffer + bytesRead; ptr += EVENT_SIZE + ((struct inotify_event *)ptr)->len) {
            handle_event((struct inotify_event *)ptr);
        }
    }

    close(inotifyFd);
    
	return 0;
}
