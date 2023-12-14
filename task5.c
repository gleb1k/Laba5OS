#define SUCCESS 0
#define ERR_WRONG_PARAM_NUMBER 1
#define ERR_CANT_OPEN_DIR 2
#define ERR_CANT_CLOSE_DIR 3

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int checkDirectory(const char *path) {
	DIR* dir = opendir (path);
	if (dir == NULL) {
		fprintf(stderr, "Can't open directory:");
		printf("%s/n", path);
		return ERR_CANT_OPEN_DIR;
	}
	
	printf("Content of %s:\n",path);
	
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		printf("%s\n",entry->d_name);
	}
	
	if (closedir(dir) == -1) {
		fprintf(stderr, "Can't close directory");
		return ERR_CANT_CLOSE_DIR;
	}

	return SUCCESS;
}

int main (int arc, char** argv) {
	int result_code = SUCCESS;

	if (arc != 2) {
		fprintf(stderr, "Wrong parameter count\n");
		return ERR_WRONG_PARAM_NUMBER;
 	}

	result_code = checkDirectory(argv[1]);
	if (result_code) {
		return result_code;
	}

	result_code = checkDirectory(".");
	if (result_code) {
		return result_code;
	}

	printf("PROGRAM EXECUTED SUCCESSFULLY\n");
	return SUCCESS;
}

