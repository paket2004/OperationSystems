#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
const char* directoryPath;
//find all hard links which refers to the same i-node of the source //in the path print them to stdout with their inode numbers and absolute paths 
void find_all_hlinks(const char* source) {
	struct stat sourceInfo;
	// Get information about the source file
    if (stat(source, &sourceInfo) == -1) {
        perror("Error getting source file information");
        return;
    }
    ino_t sourceInode = sourceInfo.st_ino;

    DIR* dir = opendir(directoryPath);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
            char* filePath = NULL;
            //printf("%s\n", entry->d_name);
            asprintf(&filePath, "%s/%s", directoryPath, entry->d_name);
			//printf("%s\n", filePath);
            struct stat fileInfo;
            if (stat(filePath, &fileInfo) == -1) {
                perror("Error getting file information");
                continue;
            }
			
            if (fileInfo.st_ino == sourceInode && strcmp(filePath, source) != 0) {
                // Print information about the hard link
                printf("Inode: %lu, Path: %s\n", (unsigned long)fileInfo.st_ino, filePath);
            }
        }
    }

    closedir(dir);
    
}

void Create_sym_link(const char* source, const char* link) {
	if (access(link, F_OK) == 0) {
        // Remove or rename the existing file
        if (remove(link) != 0) {
            perror("Error removing existing file");
        }
    }
	if (symlink(source, link) == 0) {
        printf("Symbolic link created successfully.\n");
    } else {
        perror("Error creating symbolic link");
    }
}

void unlink_all(const char* source) {
	struct stat sourceInfo;
	// Get information about the source file
    if (stat(source, &sourceInfo) == -1) {
        perror("Error getting source file information");
        return;
    }
    ino_t sourceInode = sourceInfo.st_ino;

    DIR* dir = opendir(directoryPath);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
            char* filePath = NULL;
            //printf("%s\n", entry->d_name);
            asprintf(&filePath, "%s/%s", directoryPath, entry->d_name);
			//printf("%s\n", filePath);
            struct stat fileInfo;
            if (stat(filePath, &fileInfo) == -1) {
                perror("Error getting file information");
                continue;
            }
			
            if (fileInfo.st_ino == sourceInode && strcmp(filePath, source) != 0) {
                if(remove(filePath) == 0) {
            		printf("Removed hard link: %s\n", filePath);
                } 
            } else {
            	// Print information about the hard link
                printf("Remain Inode: %lu, Path: %s\n", (unsigned long)fileInfo.st_ino, filePath);
            }
        }
    }

    closedir(dir);
    
}
int main (int argc, char* argv[]) {
	if (argc != 2) {
		printf("Error in input data\n");
	}
	directoryPath = argv[1];
	const char* fileName = "myfile1.txt";
	size_t fullPathLength = strlen(directoryPath) + strlen(fileName) + 2;
	char* fullPath = (char*)malloc(fullPathLength);
	
    if (fullPath == NULL) {
        printf("Error allocating memory.\n");
        return 1;
    }
    snprintf(fullPath, fullPathLength, "%s/%s", directoryPath, fileName);
	
	FILE* filePointer;
	filePointer = fopen(fullPath, "w");
	if (filePointer == NULL) {
        printf("Error opening the file.\n");
        free(fullPath); // Release the allocated memory
        return 1; // Exit the program with an error code
    }
    
    fprintf(filePointer, "Hello world.");
    //free(fullPath);
    
    fileName = "myfile11.txt\0";
    fullPathLength = strlen(directoryPath) + strlen(fileName) + 2;
    char* newPath = (char*)malloc(fullPathLength);
    snprintf(newPath, fullPathLength, "%s/%s", directoryPath, fileName);
    //printf("%s\n",newPath);
    if (access(newPath, F_OK) == 0) {
        // Remove or rename the existing file
        if (remove(newPath) != 0) {
            perror("Error removing existing file");
            return 1;
        }
    }
    
    if (link(fullPath, newPath) != 0) {
    	perror("Error");
    	return 1;
    }
    
    fileName = "myfile12.txt\0";
    fullPathLength = strlen(directoryPath) + strlen(fileName) + 2;
    char* newPath2 = (char*)malloc(fullPathLength);
    snprintf(newPath2, fullPathLength, "%s/%s", directoryPath, fileName);
    //printf("%s\n",newPath2);
   if (access(newPath2, F_OK) == 0) {
        // Remove or rename the existing file
        if (remove(newPath2) != 0) {
            perror("Error removing existing file");
            return 1;
        }
    }
    
    if (link(fullPath, newPath2) != 0) {
    	perror("Error");
    	return 1;
    }
    
    find_all_hlinks(fullPath);
    
    fileName = "tmp/myfile1.txt\0";
    fullPathLength = strlen(directoryPath) + strlen(fileName) + 2;
    char* path_to_remove = (char*)malloc(fullPathLength);
    snprintf(path_to_remove, fullPathLength, "%s/%s", directoryPath, fileName);
   
    
    if (rename(fullPath, path_to_remove) == 0) {
        printf("File moved successfully.\n");
    } else {
        perror("Error moving file");
    }
    fclose(filePointer);
	
	filePointer = fopen(newPath, "w");
	if (filePointer == NULL) {
        printf("Error opening the file.\n");
        free(fullPath); // Release the allocated memory
        return 1; // Exit the program with an error code
    }
    fprintf(filePointer, "Modyfying myfile11.txt");


	fileName = "tmp/myfile13.txt\0";
    fullPathLength = strlen(directoryPath) + strlen(fileName) + 2;
    char* symlink_path = (char*)malloc(fullPathLength);
    snprintf(symlink_path, fullPathLength, "%s/%s", directoryPath, fileName);
	Create_sym_link(path_to_remove, symlink_path);
	
    
    
    fclose(filePointer);
	filePointer = fopen(path_to_remove, "w");
	if (filePointer == NULL) {
        printf("Error opening the file.\n");
        free(fullPath); // Release the allocated memory
        return 1; // Exit the program with an error code
    }
    fprintf(filePointer, "Modyfying /tmp/myfile1.txt");
    unlink_all(newPath);
	return 0;
}
