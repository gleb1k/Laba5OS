#define SUCCESS 0
#define ERR_WRONG_PARAM_NUMBER 1
#define ERR_CANT_MAKE_DIR 2
#define ERR_CANT_CLOSE_DIR 3
#define ERR_CANT_GET_FILE_STAT 4
#define ERR_CANT_OPEN_FILE 5
#define ERR_CANT_CLOSE_FILE 6
#define ERR_ILLEGAL_ARGUMENT 7
#define ERR_CANT_OPEN_DIR 8


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_PATH_LENGTH 1024

typedef struct {
    char name[MAX_PATH_LENGTH];
    long size;
} FileInfo;

int compareBySize(const void *a, const void *b) {
    return ((FileInfo *)a)->size - ((FileInfo *)b)->size;
}

int compareByName(const void *a, const void *b) {
    return strcmp(((FileInfo *)a)->name, ((FileInfo *)b)->name);
}

void processDirectory(const char *dirPath, int sortBy, const char *outputDir) {
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    FileInfo *files = NULL;
    int fileCount = 0;

    dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Can't open directory");
        exit(ERR_CANT_OPEN_DIR);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char filePath[MAX_PATH_LENGTH];
            snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);

            if (stat(filePath, &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
                files = realloc(files, (fileCount + 1) * sizeof(FileInfo));
                strncpy(files[fileCount].name, filePath, sizeof(files[fileCount].name));
                files[fileCount].size = fileStat.st_size;
                fileCount++;
            }
        }
    }
    closedir(dir);

    if (sortBy == 1) {
        qsort(files, fileCount, sizeof(FileInfo), compareBySize);
    } else if (sortBy == 2) {
        qsort(files, fileCount, sizeof(FileInfo), compareByName);
    }

    if (mkdir(outputDir, 0777) != 0 && errno != EEXIST) {
        perror("Can't make directory");
        exit(ERR_CANT_OPEN_DIR);
    }

    for (int i = 0; i < fileCount; i++) {
        FILE *source = fopen(files[i].name, "rb");
            
        char destination[MAX_PATH_LENGTH];
        snprintf(destination, sizeof(destination), "%s/%s", outputDir, strrchr(files[i].name, '/') + 1);
        FILE *dest = fopen(destination, "wb");

        if (source == NULL || dest == NULL) {
            perror("Can't open file");
            exit(ERR_CANT_OPEN_FILE);
        }

        char buffer[1024];
        size_t bytesRead;
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
            fwrite(buffer, 1, bytesRead, dest);
        }

        printf("Path: %s, Name: %s, Size: %ld\n", dirPath, strrchr(files[i].name, '/') + 1, files[i].size);

        fclose(source);
        fclose(dest);
    }

    free(files);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Illegal argument count\n");
        return ERR_ILLEGAL_ARGUMENT;
    }

    const char *dirPath = argv[1];
    int sortBy = atoi(argv[2]);
    const char *outputDir = argv[3];

    if (sortBy != 1 && sortBy != 2) {
        printf("Illegal sort argument\n");
        return ERR_ILLEGAL_ARGUMENT;
    }

    processDirectory(dirPath, sortBy, outputDir);

    return SUCCESS;
}
