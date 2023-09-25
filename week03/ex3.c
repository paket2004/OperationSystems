#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

struct File {
    int id;
    char name[64];
    int size;
    char data[1024];
    struct Directory *directory;
};

void overwrite_to_file(struct File* file, const char* str) {
    strcpy(file->data,"");
    file->size = 0;
    if (strlen(str) < sizeof(file->data)) {
        file->size = strlen(str);
        strcpy(file->data, str);
    } else {
        printf("Not enough space to append the string.\n");
    }
}
void append_to_file(struct File* file, const char* str) {
    size_t current_length = strlen(file->data);

    if (current_length + strlen(str) < sizeof(file->data)) {
        // Append the new string to the end of the existing string
        file->size += current_length;
        strcat(file->data," ");
        strcat(file->data, str);
    } else {
        printf("Not enough space to append the new string.\n");
    }
}
struct Directory {
    char name[64];
    struct File*  files;
    struct Directory** subdirectories;
    unsigned char nf;
    unsigned char nd;
    char path[100];
};

void show_file(struct File *file)
{
printf("%s ", file->name);
}

void show_dir(struct Directory *dir)
{
    printf("\nDIRECTORY\n");
    printf(" path: %s\n", dir->path);
    printf(" files:\n");
    printf("[ ");
    for (int i = 0; i < dir->nf; i++)
    {
        show_file(&(dir->files[i]));
    }
    printf("]\n");
    printf(" directories:\n");
    printf(" { ");
        for (int i = 0; i < dir->nd; i++)
        {
            show_dir(dir->subdirectories[i]);
        }
        printf("}\n");
}

#define MAX_PATH 2048
// Adds the subdirectory dir1 to the directory dir2
void add_dir(struct Directory *dir1, struct Directory *dir2) {
    if (dir1 && dir2) {
        if (dir2->nd < 100) { // Assuming a maximum of 100 subdirectories in a directory
            dir2->subdirectories[dir2->nd] = dir1;
            dir2->nd++;
            char temp_path[MAX_PATH];
            if (strcmp(dir2->path, "/") != 0) {
                strcpy(temp_path, dir2->path);
                strcat(temp_path, "/");
                strcat(temp_path, dir1->name);
                strcpy(dir1->path, temp_path);
            } else {
                strcpy(temp_path, "/");
                strcat(temp_path, dir1->name);
                strcpy(dir1->path, temp_path);
            }
        } else {
            printf("Maximum number of subdirectories reached in the directory.\n");
        }
    }
}

void add_file(struct File* file, struct Directory* dir) {
    if (dir->nf < 100) {
        char temp_path[MAX_PATH];
        dir->files[dir->nf++] = *file;
        // Set the directory of the added file to the target directory
        file->directory = dir;
//
    } else {
        printf("Maximum number of files reached in the directory.\n");
    }
}

void printp_file(struct File* file) {
    printf("%s\n", file->directory->path);
}

int main() {

    struct Directory root;
    strcpy(root.name,"root");
    root.nd = 0;
    root.nf = 0;
    strcpy(root.path, "/root");
    root.subdirectories = malloc(sizeof(struct Directory*) * MAX_PATH);
    root.files = malloc(sizeof (struct File) * MAX_PATH);

    struct Directory home;
    strcpy(home.name,"home");
    home.nd = 0;
    home.nf = 0;
    strcpy(home.path, "home");
    home.subdirectories = malloc(sizeof(struct Directory*) * MAX_PATH);
    home.files = malloc(sizeof (struct File) * MAX_PATH);

    struct Directory bin;
    strcpy(bin.name,"bin");
    bin.nd = 0;
    bin.nf = 0;
    strcpy(bin.path, "bin");
    bin.subdirectories = malloc(sizeof(struct Directory*) * MAX_PATH);
    bin.files = malloc(sizeof (struct File) * MAX_PATH);

    struct File bash;
    strcpy(bash.name,"bash");
    bash.size = 0;
    bash.id = 0;
    bash.directory = malloc(sizeof (struct File) * MAX_PATH);

    add_dir(&home,&root);
    add_dir(&bin,&root);

//    printf("%s\n", root.path);
//    printf("%s\n", home.path);
    struct File ex1;
    strcpy(ex1.name,"ex3_1.c");
    ex1.size = 0;
    ex1.id = 1;
    ex1.directory = malloc(sizeof (struct File) * MAX_PATH);

    struct File ex2;
    strcpy(ex2.name,"ex3_2.c");
    ex2.size = 0;
    ex2.id = 2;
    ex2.directory = malloc(sizeof (struct File) * MAX_PATH);



    overwrite_to_file(&ex1,"int printf(const char * format, ...);");
    overwrite_to_file(&ex2,"//This is a comment in C language");
    overwrite_to_file(&bash, "Bourne Again Shell!!");

    add_file(&bash,&bin);
    add_file(&ex1,&home);
    add_file(&ex2,&home);

    append_to_file(&ex1, "int main(){printf(”Hello World!”)}");

    show_dir(&root);
    return 0;
}
