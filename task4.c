#define SUCCESS 0
#define ERR_WRONG_PARAM_NUMBER 1
#define ERR_CANT_OPEN_FILE 2
#define ERR_CANT_CLOSE_FILE 3
#define ERR_CANT_WRITE_FILE 4
#define ERR_CANT_GET_FILE_STAT 5
#define ERR_CANT_SET_FILE_STAT 6

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int arc, char** argv) {
	int result_code = SUCCESS;

	if (arc != 3) {
		fprintf(stderr, "Wrong parameter count. Pass filenames to program\n");
		return ERR_WRONG_PARAM_NUMBER;
 	}

	FILE* fileFrom = fopen(argv[1], "r");
	if (!fileFrom){
		fprintf(stderr, "Can't open source file or file doesn't exist\n");
		return ERR_CANT_OPEN_FILE;
	}

	FILE* fileTo = fopen(argv[2], "w");
	if (!fileTo){
		fprintf(stderr, "Can't open destonation file or file doesn't exist\n");
		return ERR_CANT_OPEN_FILE;
	}

	char actual_char;

	while ((actual_char=fgetc(fileFrom))!= EOF) {
		if(fputc(actual_char, fileTo)==EOF) {
			fprintf(stderr, "Can't write data to file\n");
			return ERR_CANT_WRITE_FILE;
		}
	}

	if (result_code) {
		return result_code;
	}

	result_code = fclose(fileFrom);

	if (result_code == EOF) {
		fprintf(stderr, "Can't close source file \n");
		return ERR_CANT_CLOSE_FILE;
	}

	result_code = fclose(fileTo);

	if (result_code == EOF) {
		fprintf(stderr, "Can't close destonation file \n");
		return ERR_CANT_CLOSE_FILE;
	}

	// permissions
	struct stat statFile;

	if (stat(argv[1], &statFile) == -1) {
		fprintf(stderr, "Cant get source file permissions");
		return ERR_CANT_GET_FILE_STAT;
	}
	
	if (chmod(argv[2], statFile.st_mode) == -1){
		fprintf(stderr, "Cant set source file permissions");
		return ERR_CANT_SET_FILE_STAT;
	}

	printf("COPIED SUCCESSFULLY\n");
	return SUCCESS;
}

