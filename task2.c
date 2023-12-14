#define ERR_WRONG_PARAM_NUMBER 1
#define ERR_CANT_OPEN_FILE 2
#define ERR_CANT_CLOSE_FILE 3
#define ERR_CANT_WRITE_FILE 4
#define SUCCESS 0

#define EXIT_COMBINATION 6

#include <stdio.h>
#include <stdlib.h>

int main(int arc, char** argv) {

	int result_code = SUCCESS;

	if (arc != 2) {
		fprintf(stderr, "Wrong parameter count. Pass filename to program\n");
		return ERR_WRONG_PARAM_NUMBER;
	}

	FILE* output = fopen(argv[1], "w+");
	if (!output) {
		fprintf(stderr, "Can't open file or file doesn't exist\n");
		return ERR_CANT_OPEN_FILE;
	}

	fprintf(stdout, "Put letters (Write ctrl-F to finish)\n");

	char actual_char;
	while (1) {
		actual_char = getc(stdin);
		if (actual_char == EOF || actual_char == EXIT_COMBINATION) {
			break;
		}
		result_code = fputc(actual_char, output);
		if (result_code == EOF) {
			fprintf(stderr, "Can't write to passed file\n");
			result_code = ERR_CANT_WRITE_FILE;
			break;
		} else {
			result_code = SUCCESS;
		}
	}

	if (result_code) {
		return result_code;
	}

	result_code = fclose(output);

	if (result_code == EOF) {
		fprintf(stderr, "Can't close output fie\n");
		return ERR_CANT_CLOSE_FILE;
	}

	return SUCCESS;
}
