#define SUCCESS 0
#define ERR_WRONG_PARAM_NUMBER 1
#define ERR_CANT_OPEN_FILE 2
#define ERR_CANT_CLOSE_FILE 3

#include <stdio.h>
#include <stdlib.h>

int main (int arc, char** argv) {
	int result_code = SUCCESS;

	if (arc != 3) {
		fprintf(stderr, "Wrong parameter count. Pass filename to program\n");
		return ERR_WRONG_PARAM_NUMBER;
 	}

	FILE* file = fopen(argv[1], "r");

	if (!file){
		fprintf(stderr, "Can't open file or file doesn't exist\n");
		return ERR_CANT_OPEN_FILE;
	}

	fprintf(stdout, "FIle content:\n");
	
	char actual_char;
	int count = 0;
	int groupSize = atoi(argv[2]);
	while ((actual_char=fgetc(file))!= EOF) {
		putchar(actual_char);
		if  (actual_char == '\n') {
			count++;
			
			if (groupSize > 0 && count == groupSize) {
			printf("PRESS ANY KEY TO CONTINUE...\n");
			getchar();
			count = 0;
			}
		}
	}

	if (result_code) {
		return result_code;
	}

	result_code = fclose(file);

	if (result_code == EOF) {
		fprintf(stderr, "Can't close file \n");
		return ERR_CANT_CLOSE_FILE;
	}

	return SUCCESS;
}

